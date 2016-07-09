Copyright 2016 Gregory Bryant

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
/***********************************************************************/


#include "synthesizer.h"
#include "oscillator.h"
#include "instrumentparser.h"
#include <sndfile.h>
#include <QXmlStreamWriter>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

#include <cmath>

#include <iostream>
#include <iomanip>

//std::cout << std::fixed;
//std::cout << std::setprecision(4) << amp << std::endl;
#define log(X) std::cout << X << std::endl;

Synthesizer::Synthesizer(QObject *parent):QObject(parent)
{
    //media = new Phonon::MediaObject(this);
    //audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    //audioPath = Phonon::createPath(media, audioOutput);
    //connect(media, SIGNAL(finished()), SLOT(soundFinishedPlaying()));
    sampleRate = 44100;
}

int Synthesizer::saveAsWave(ScoreData *scoreData, QString instrumentName, QString path)
{
    SNDFILE* mySoundFile;
    SF_INFO mySoundInfo;

    mySoundInfo.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
    mySoundInfo.samplerate = 44100;
    mySoundInfo.channels = 1;

    mySoundFile = sf_open(path.toAscii().data(),SFM_RDWR,&mySoundInfo);

    if(!mySoundFile){return -1;}

    FloatData *data;

    data = getFloatData(scoreData,instrumentName);

    sf_seek(mySoundFile,0,SEEK_SET);
    sf_writef_float(mySoundFile,data->data,data->count);

    sf_count_t  frames = data->count ;
    sf_command (mySoundFile, SFC_FILE_TRUNCATE, &frames, sizeof (frames)) ;

    sf_close(mySoundFile);

    free(data->data);
    free(data);

    return 0;
}

void Synthesizer::applyOscillators(QString instrumentName, FloatData *data, float amplitude, float frequency, int noteLength)
{
    Oscillator osc;
    Instrument *instrument = instruments[instrumentName];
    int oldPos,lastPos;

    for(int j=0;j<instrument->oscillators.count();j++)
    {
        oldPos=data->pos;
        if(instrument->oscillators.at(j)=="sine"){osc.addSineWave(data,amplitude,frequency,noteLength);}
        else if(instrument->oscillators.at(j)=="square"){osc.addSquareWave(data,amplitude,frequency,noteLength);}
        else if(instrument->oscillators.at(j)=="triangle"){osc.addTriangleWave(data,amplitude,frequency,noteLength);}
        else if(instrument->oscillators.at(j)=="sawtooth"){osc.addSawToothWave(data,amplitude,frequency,noteLength);}
        //lastPos=(*pos);
        data->pos=oldPos;
    }
    //(*pos)=lastPos; //this responsibility has moved to the caller...ie (*pos)+=bpm step size

}

void Synthesizer::applyOperations(QString instrumentName, FloatData *data, float amplitude, float frequency, int noteLength)
{
    int oldPos;

    oldPos=data->pos;
    for(int i=0;i<noteLength;i++)
    {
        data->data[data->pos] = tan(data->data[data->pos])*.8;
        data->pos++;
    }
    data->pos=oldPos;
}

void Synthesizer::applyAmplitudeEnvelope(QString instrumentName, FloatData *data, Note *n)
{
    int oldPos,spos,epos;
    int noteLength,loopLength;
    Instrument *instrument = instruments[instrumentName];

    noteLength=n->duration*sampleRate+
               instrument->amplitudeEnvelope.attackLength*sampleRate+
               instrument->amplitudeEnvelope.decayLength*sampleRate+
               instrument->amplitudeEnvelope.releaseLength*sampleRate;


    float x1,x2,y1,y2,m,b,numerator,denominator;

    int i;
    oldPos=data->pos;
    //ATTACK--------------------------------------------------------------------
    x1 = 0;
    y1 = instrument->amplitudeEnvelope.initialLevel;
    x2 = instrument->amplitudeEnvelope.attackLength*sampleRate;
    y2 = instrument->amplitudeEnvelope.attackLevel;
    m = (y1-y2)/(x1-x2);
    b = y1-(m*x1);
    loopLength=instrument->amplitudeEnvelope.attackLength*sampleRate;
    spos=data->pos;
    for(int i=0,j=x1;i<loopLength;i++,j++)
    {
        float amp = (m*(float)j)+b;
        //std::cout << "attack amp: ";
        //std::cout << std::fixed;
        //std::cout << std::setprecision(4) << amp << std::endl;
        data->data[data->pos] *= amp;
        data->pos++;
    }
    epos=data->pos;
    std::cout << "ATTACK     amount written: "<<epos-spos<<"amount expected: "<<loopLength << std::endl;
    //DECAY---------------------------------------------------------------------
    x1 = instrument->amplitudeEnvelope.attackLength*sampleRate;
    y1 = instrument->amplitudeEnvelope.attackLevel;
    x2 = x1+instrument->amplitudeEnvelope.decayLength*sampleRate;
    y2 = instrument->amplitudeEnvelope.sustainLevel;
    m = (y1-y2)/(x1-x2);
    b = y1-(m*x1);
    loopLength=instrument->amplitudeEnvelope.decayLength*sampleRate;
    spos=data->pos;
    for(int i=0,j=x1;i<loopLength;i++,j++)
    {
        float amp = m*(float)j+b;
        //std::cout << "decay amp: ";
        //std::cout << std::fixed;
        //std::cout << std::setprecision(4) << amp << std::endl;
        data->data[data->pos] *= amp;
        data->pos++;
    }
    epos=data->pos;
    std::cout << "DECAY     amount written: "<<epos-spos<<"amount expected: "<<loopLength << std::endl;
    //SUSTAIN-------------------------------------------------------------------
    loopLength=n->duration*sampleRate;
    spos=data->pos;
    for(int i=0;i<loopLength;i++)
    {
         data->data[data->pos] *= instrument->amplitudeEnvelope.sustainLevel;
         data->pos++;
    }
    epos=data->pos;
    std::cout << "SUSTAIN     amount written: "<<epos-spos<<"amount expected: "<<loopLength << std::endl;
    //RELEASE-------------------------------------------------------------------
    x1 = instrument->amplitudeEnvelope.attackLength*sampleRate +
         instrument->amplitudeEnvelope.decayLength*sampleRate +
         n->duration*sampleRate;
    x2 = x1+instrument->amplitudeEnvelope.releaseLength*sampleRate;
    y1 = instrument->amplitudeEnvelope.sustainLevel;
    y2 = instrument->amplitudeEnvelope.releaseLevel;
    m = (y1-y2)/(x1-x2);
    b = y1-(m*x1);
    loopLength = instrument->amplitudeEnvelope.releaseLength*sampleRate;
    spos=data->pos;
    for(int i=0,j=x1;i<loopLength;i++,j++)
    {
        float amp = (m*j)+b;
        //std::cout << "release amp: ";
        //std::cout << std::fixed;
        //std::cout << std::setprecision(4) << amp << std::endl;
        if(data->pos>=data->count)
        {
            std::cout << "barf!" << std::endl;
            exit(-1);
        }
        data->data[data->pos] *= amp;
        data->pos++;
    }
    epos=data->pos;
    std::cout << "RELEASE     amount written: "<<epos-spos<<"amount expected: "<<loopLength << std::endl;
    data->pos=oldPos;
}


void Synthesizer::playInstrument(QString instrumentName, FloatData *data, NoteArray *notes)
{
    Oscillator osc;
    Instrument *instrument = instruments[instrumentName];
    int oldPos,lastPos;

    int noteLength;

    if(notes->count)
    {
        for(int i=0;i<notes->count;i++)
        {
            noteLength=notes->n[i].duration*sampleRate+
                       instrument->amplitudeEnvelope.attackLength*sampleRate+
                       instrument->amplitudeEnvelope.decayLength*sampleRate+
                       instrument->amplitudeEnvelope.releaseLength*sampleRate;
            applyOscillators(instrumentName,data,/*notes->n[i].amplitude*/1.0,notes->n[i].frequency,noteLength);
            //applyOperations(instrumentName,data,pos,notes->n[i].amplitude,notes->n[i].frequency,noteLength);
            //(*pos)+=noteLength;
            applyAmplitudeEnvelope(instrumentName,data,&notes->n[i]);
        }
    }


}

void Synthesizer::playInstrumentSilence(QString instrumentName, FloatData *data, int noteLength)//this is so an instrument can be noisy...constant hum...weird background frequencies....etc
{
    Oscillator osc;
    osc.writeSilence(data,noteLength);
}

//length in samples
int Synthesizer::getDataLength(QString instrumentName, ScoreData *scoreData)
{

    //make use of the scoreData field bpm for stepsize....


    Instrument *instrument = instruments[instrumentName];
    NoteArray notes;
    notes.n = (Note*)malloc(sizeof(Note)*100);//100 is for height current actual is 88--fix this
    int width = scoreData->getActualWidth();
    int dataLength=0;
    int dataPos=0;
    int noteLength=0;
    int longestNote=0;

    for(int i=0;i<width;i++)
    {
        scoreData->getNotes(i,&notes);
        if(notes.count)
        {
            longestNote=0;
            for(int j=0;j<notes.count;j++)
            {
                noteLength = notes.n[j].duration*sampleRate+
                        instrument->amplitudeEnvelope.attackLength*sampleRate+
                        instrument->amplitudeEnvelope.decayLength*sampleRate+
                        instrument->amplitudeEnvelope.releaseLength*sampleRate;
                if(noteLength>longestNote){longestNote=noteLength;}
                attackLength = instrument->amplitudeEnvelope.attackLength*sampleRate;
                decayLength = instrument->amplitudeEnvelope.decayLength*sampleRate;
                sustainLength = notes.n[j].duration*sampleRate;
                releaseLength = instrument->amplitudeEnvelope.releaseLength*sampleRate;
            }
            if((dataPos+longestNote)>dataLength){dataLength=dataPos+longestNote;}
        }
        dataPos+=(60/scoreData->bpm)*sampleRate;
    }

    free(notes.n);
    return dataLength;
}

FloatData* Synthesizer::getFloatData(ScoreData *scoreData, QString instrumentName)
{
    FloatData *data;
    NoteArray notes;
    notes.n = (Note*)malloc(sizeof(Note)*100);//100 is for height current actual is 88--fix this
    int width = scoreData->getActualWidth();
    //float noteLength = 44100/6;
    //int sampleCount = noteLength*width;
    //int byteCount = sizeof(float)*sampleCount;

    data = (FloatData*)malloc(sizeof(FloatData));

    int dataSize = getDataLength(instrumentName,scoreData);

    log(dataSize);

    data->data = (float*)malloc(dataSize*sizeof(float));
    memset(data->data,0,dataSize*sizeof(float));
    data->pos=0;
    data->count=dataSize;
    data->size=dataSize*sizeof(float);



    for(int i=0;i<width;i++)
    {
        scoreData->getNotes(i,&notes);
        if(notes.count)
        {
            playInstrument(instrumentName,data,&notes);
            //instrument->play(data,&pos,&notes,noteLength);
        }
        else
        {
            //playInstrumentSilence(instrumentName,data,&pos,(float)sampleRate/6.0);
            //instrument->silence(data,&pos,noteLength);
        }
        data->pos+=(60.0/scoreData->bpm)*sampleRate;
    }

    for(int i=0;i<dataSize;i++)
    {
        //data[i] *= .4;
    }

    free(notes.n);
    return data;
}

void Synthesizer::play(ScoreData *scoreData, QString instrumentName)
{
#ifdef Q_WS_X11
    QString path="/home/gbryant/newsound.wav";
#endif
#ifdef Q_WS_MAC
    QString path="/Users/gbryant/newsound.wav";
#endif
#ifdef Q_WS_WIN
    QString path="C:/Users/golgi/Music/newsound.wav";
#endif
    saveAsWave(scoreData,instrumentName,path);
    play_Phonon(path);
}


int Synthesizer::loadInstruments()
{
#ifdef Q_WS_X11
    QDir dir("/home/gbryant/Documents/Instruments/");
#endif
#ifdef Q_WS_MAC
    QDir dir("/Users/gbryant/Documents/Instruments/");
#endif
#ifdef Q_WS_WIN
    QDir dir("C:/Users/golgi/Music/Instruments/");
#endif

    QStringList filters;
    filters << "*.inst";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    QStringList files = dir.entryList();


    instruments.clear();
    for(int i=0;i<files.count();i++)
    {
        InstrumentParser *handler = new InstrumentParser();
        QString fileName = dir.absoluteFilePath(files.at(i));
        QFileInfo fileInfo(fileName);
        log(fileName.toStdString());
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)){return -1;}
        QXmlInputSource source(&file);
        QXmlSimpleReader reader;
        handler->instrument = new Instrument();
        handler->instrument->name = fileInfo.baseName();
        reader.setContentHandler(handler);
        reader.setErrorHandler(handler);
        bool ok=reader.parse(source);
        if(!ok){file.close();return -1;}
        instruments[handler->instrument->name]=handler->instrument;
        file.close();
        delete handler;
    }

    QMapIterator<QString, Instrument*> iterator(instruments);
    //while (iterator.hasNext())
    //{
    //    iterator.next();
    //    ui->comboBox->addItem(iterator.value()->name);
    //}


    iterator.next();
    instruments["default"]=iterator.value();

    return files.count();
}

void Synthesizer::play_Phonon(QString path)
{
    //media->setCurrentSource(Phonon::MediaSource(path));
    //media->play();
}

void Synthesizer::soundFinishedPlaying(void)
{
    //media->clear();
}

