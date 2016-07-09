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


#ifndef SCOREVIEW_H
#define SCOREVIEW_H


#include <QWidget>
#include "notation.h"



class ScoreDocument;

class ScoreView : public QWidget
{
    Q_OBJECT
public:
    enum EntryMode
    {
        normal=1,
        mirror
    };
    explicit ScoreView(ScoreDocument *parent);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void notePress(int x, int y);
    void setAddNoteMode(int x, int y);
    void setEntryMode(EntryMode mode);
    EntryMode getEntryMode(void);

    ScoreDocument *scoreDocument;



private:
    bool addMode;
    int xCursor;
    int yCursor;
    bool spaceIsDown;
    NoteArray noteArray;
    EntryMode entryMode;
    void handleSpaceDown(QKeyEvent *event,bool setAddMode);
public:
    int gridSize;

signals:

public slots:

};

#endif // SCOREVIEW_H
