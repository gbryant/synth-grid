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


#include "scoredata.h"
//#include "defaultinstrument.h"
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <sndfile.h>
#include <QString>
#include <iostream>

#define log(X) std::cout << X << std::endl;


ScoreData::ScoreData(int scoreWidthIn, int scoreHeightIn)
{
    scoreWidth = scoreWidthIn;
    scoreHeight = scoreHeightIn;

    notes=(bool*)malloc(sizeof(bool)*scoreWidth*scoreHeight);
    memset(notes,0,sizeof(bool)*scoreWidth*scoreHeight);

    bpm = 360;
}

void ScoreData::arrayToNotes(char *array, int size)
{
    free(notes);
    notes = (bool*)malloc(sizeof(bool)*size);
    for(int i=0;i<size;i++)
    {
        ((char*)notes)[i] = array[i];
    }
}

char* ScoreData::notesToArray(int *size)
{
    (*size)=scoreWidth*scoreHeight*sizeof(bool);
    return (char*)notes;
}

int ScoreData::getWidth()
{
    return scoreWidth;
}

int ScoreData::getHeight()
{
    return scoreHeight;
}

int ScoreData::getActualWidth()
{
    NoteArray notes;
    notes.n = (Note*)malloc(sizeof(Note)*100);//100 is for height current actual is 88--fix this

    for(int i=getWidth()-1;i>=0;i--)
    {
        getNotes(i,&notes);
        if(notes.count)
        {
            free(notes.n);
            return i+1;
        }
    }
    free(notes.n);
    return getWidth();
}

void ScoreData::getNotes(int xPos,NoteArray *noteArray)
{
    noteArray->count=0;

    for(int i=0;i<scoreHeight;i++)
    {
        if(notes[xPos*scoreHeight+i])
        {
            //440*(2^((n-49)/(12)))

            noteArray->n[noteArray->count].frequency = 440*(pow(2,((scoreHeight-i-49)/(12.0))));
            //noteArray->n[noteArray->count].frequency = 50+50*(scoreHeight-1-i);/*log("frequency: "+QString().number(noteArray->n[noteArray->count].frequency).toStdString());*/
            noteArray->n[noteArray->count].amplitude = .5;
            noteArray->n[noteArray->count].duration = 1.0/6.0;
            noteArray->n[noteArray->count].velocity = 1;
            noteArray->n[noteArray->count].y=i;

            noteArray->count++;
        }

    }
}

bool ScoreData::isNoteSet(int x, int y)
{
    if(notes[x*scoreHeight+y])
    {return true;}
    else
    {return false;}
}

bool ScoreData::isValidNote(int x, int y)
{
    if(x<scoreWidth && y<scoreHeight && x>=0 && y>=0)
    {return true;}
    else
    {return false;}
}

void ScoreData::setNote(int x, int y)
{
    if(isValidNote(x,y))
    {
        notes[x*scoreHeight+y]=true;
    }
}

void ScoreData::unsetNote(int x, int y)
{
    if(isValidNote(x,y))
    {
        notes[x*scoreHeight+y]=false;
    }
}

void ScoreData::toggleNote(int x, int y)
{

    if(isValidNote(x,y))
    {
        if(isNoteSet(x,y))
        {
            notes[x*scoreHeight+y]=false;
        }

        else
        {
            notes[x*scoreHeight+y]=true;
        }
    }
}

void ScoreData::selectNotes(QPoint start, QPoint end)
{

}

