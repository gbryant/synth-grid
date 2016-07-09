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


#ifndef SCOREDATA_H
#define SCOREDATA_H

#include "notation.h"
//#include "instrument.h"
#include <QPoint>

class ScoreData
{
public:
    ScoreData(int scoreWidthIn, int scoreHeightIn);
    void arrayToNotes(char *array, int size);
    char* notesToArray(int *size);
    void setNote(int x, int y);
    void unsetNote(int x, int y);
    void toggleNote(int x, int y);
    bool isNoteSet(int x, int y);
    bool isValidNote(int x, int y);
    void selectNotes(QPoint start, QPoint end);
    void getNotes(int xPos,NoteArray *noteArray);
    int getWidth(void);
    int getActualWidth(void);
    int getHeight(void);

private:
    int scoreWidth;
    int scoreHeight;
    bool *notes;
public:
    float bpm;
};

#endif // SCOREDATA_H
