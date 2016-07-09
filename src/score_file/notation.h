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


#ifndef NOTATION_H
#define NOTATION_H

struct FloatData
{
    float *data;
    int count;
    int size;
    int pos;
};
typedef struct FloatData FloatData;

struct ADSR
{
    float initialLevel;
    float attackLength;
    float attackLevel;
    float decayLength;
    float sustainLevel;
    float releaseLength;
    float releaseLevel;
};
typedef struct ADSR ADSR;

struct Note
{
    int y;
    float frequency;
    float amplitude;
    float duration; //time instrument key is held
    float velocity;
};
typedef struct Note Note;

struct NoteArray
{
    Note *n;
    int count;
};
typedef struct NoteArray NoteArray;



struct ScoreNote
{
    int x;
    int y;
};
typedef struct ScoreNote ScoreNote;









#endif // NOTATION_H
