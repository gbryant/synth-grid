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


#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H
#include <Phonon>
#include "notation.h"
#include "instrument.h"
#include "scoredata.h"


#include <QList>
#include <QMap>



class Synthesizer: public QObject
{
    Q_OBJECT
public:
    Synthesizer(QObject *parent = 0);
    int loadInstruments(void);
    void play(ScoreData *scoreData,QString instrumentName);
    void play_Phonon(QString path);
    int saveAsWave(ScoreData *scoreData,QString instrumentName,QString path);
    FloatData* getFloatData(ScoreData *scoreData,QString instrumentName);
    int getDataLength(QString instrumentName, ScoreData *scoreData);


    void playInstrument(QString instrumentName,FloatData *data,NoteArray *notes);
    void playInstrumentSilence(QString instrumentName,FloatData *data,int noteLength);

    void applyOscillators(QString instrumentName, FloatData *data, float amplitude, float frequency, int noteLength);
    void applyOperations(QString instrumentName, FloatData *data, float amplitude, float frequency, int noteLength);
    void applyAmplitudeEnvelope(QString instrumentName, FloatData *data,Note *n);

    QMap<QString,Instrument*> instruments;

private:
    //Phonon::MediaObject *media;
    //Phonon::AudioOutput *audioOutput;
    //Phonon::Path audioPath;
    int sampleRate;
    int attackLength;
    int decayLength;
    int sustainLength;
    int releaseLength;

private slots:
    void soundFinishedPlaying(void);
};

#endif // SYNTHESIZER_H
