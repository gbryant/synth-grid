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


#include "scoreview.h"
#include "scoredocument.h"
#include <QPainter>
#include <QPaintEvent>

ScoreView::ScoreView(ScoreDocument *parent) :QWidget(parent)
{
    gridSize=10;
    scoreDocument=parent;
    setGeometry(0,0,scoreDocument->scoreData->getWidth()*gridSize+1,scoreDocument->scoreData->getHeight()*gridSize+1);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    xCursor=0;
    yCursor=scoreDocument->scoreData->getHeight()/2;
    addMode=false;
    spaceIsDown=false;
    noteArray.n = (Note*)malloc(sizeof(Note)*10000);
    entryMode = normal;
}

void ScoreView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Space:
        if(!event->isAutoRepeat()){spaceIsDown=false;}
        update();
        break;
    }
}

void ScoreView::keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_Up:
        if(yCursor>0)
        {yCursor--;}
        if(spaceIsDown){handleSpaceDown(event,false);}
        break;
    case Qt::Key_Down:
        if(yCursor<scoreDocument->scoreData->getHeight()-1)
        {yCursor++;}
        if(spaceIsDown){handleSpaceDown(event,false);}
        break;
    case Qt::Key_Left:
        if(xCursor>0)
        {xCursor--;}
        if(spaceIsDown){handleSpaceDown(event,false);}
        break;
    case Qt::Key_Right:
        if(xCursor<scoreDocument->scoreData->getWidth()-1)
        {xCursor++;}
        if(spaceIsDown){handleSpaceDown(event,false);}
        break;
    case Qt::Key_Space:
        handleSpaceDown(event,true);
        break;

    case Qt::Key_P:
        scoreDocument->play();
        return;//don't want the update(); if app is quitting during a play operation
        break;
    }
    update();
}

void ScoreView::handleSpaceDown(QKeyEvent *event, bool setAddMode)
{
    if(setAddMode)
    {
        spaceIsDown=true;
        if(!event->isAutoRepeat())
        {
            setAddNoteMode(xCursor,yCursor);
            scoreDocument->setIsDocumentEdited(true);
            scoreDocument->scoreData->toggleNote(xCursor,yCursor);
            if(entryMode==mirror)
            {
                int scoreHeight = scoreDocument->scoreData->getHeight();
                if(yCursor!=scoreHeight/2)
                {scoreDocument->scoreData->toggleNote(xCursor,scoreHeight-1-yCursor);}
            }
        }
    }
    else
    {
        scoreDocument->setIsDocumentEdited(true);
        notePress(xCursor,yCursor);
        if(entryMode==mirror)
        {
            int scoreHeight = scoreDocument->scoreData->getHeight();
            if(yCursor!=scoreHeight/2)
            {notePress(xCursor,scoreHeight-1-yCursor);}
        }
    }
}

void ScoreView::setEntryMode(EntryMode mode)
{
    entryMode=mode;
}

ScoreView::EntryMode ScoreView::getEntryMode()
{
    return entryMode;
}


void ScoreView::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x()/gridSize;
    int y = event->pos().y()/gridSize;

    setAddNoteMode(x,y);
    scoreDocument->setIsDocumentEdited(true);

    if(entryMode==normal)
    {
        scoreDocument->scoreData->toggleNote(x,y);
    }
    else if(entryMode==mirror)
    {
        scoreDocument->scoreData->toggleNote(x,y);
        int scoreHeight = scoreDocument->scoreData->getHeight();
        if(y!=scoreHeight/2)
        {scoreDocument->scoreData->toggleNote(x,scoreHeight-1-y);}

    }
    this->update();
}

void ScoreView::setAddNoteMode(int x, int y)
{
    if(scoreDocument->scoreData->isNoteSet(x,y))
    {addMode=false;}
    else
    {addMode=true;}
}

void ScoreView::notePress(int x, int y)
{
    scoreDocument->setIsDocumentEdited(true);
    if(scoreDocument->scoreData->isNoteSet(x,y))
    {
        if(!addMode)
        {scoreDocument->scoreData->unsetNote(x,y);}
    }
    else
    {
        if(addMode)
        {scoreDocument->scoreData->setNote(x,y);}
    }
}

void ScoreView::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x()/gridSize;
    int y = event->pos().y()/gridSize;

    if (!(event->buttons() & Qt::LeftButton)){return;}

    if(entryMode==normal)
    {notePress(x,y);}
    else if(entryMode==mirror)
    {
        notePress(x,y);
        int scoreHeight = scoreDocument->scoreData->getHeight();
        if(y!=scoreHeight/2)
        {notePress(x,scoreHeight-1-y);}

    }

    this->update();

}

void ScoreView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBackground(Qt::white);
    painter.eraseRect(event->rect());
    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);

    //---------------------------Draw Grid---------------------------

    for(int i=0;i<this->geometry().width();i+=gridSize)
    {
        painter.drawLine(i,0,i,this->geometry().height());
    }

    for(int i=0;i<this->geometry().height();i+=gridSize)
    {
        painter.drawLine(0,i,this->geometry().width(),i);
    }

    //---------------------------Draw Notes---------------------------

    for(int i=0;i<scoreDocument->scoreData->getWidth();i++)
    {
        scoreDocument->scoreData->getNotes(i,&noteArray);
        for(int j=0;j<noteArray.count;j++)
        {
            painter.fillRect(i*gridSize+1,noteArray.n[j].y*gridSize+1,gridSize-1,gridSize-1,Qt::blue);
        }
    }

    //---------------------------Draw Cursor---------------------------


    if(entryMode==normal)
    {
        QPen pen(Qt::red);
        pen.setWidth(4);
        painter.setBrush(Qt::transparent);
        painter.setPen(pen);
        painter.drawRect(xCursor*gridSize+1,yCursor*gridSize+1,gridSize-1,gridSize-1);
    }
    else if(entryMode==mirror)
    {
        int scoreHeight = scoreDocument->scoreData->getHeight();
        QPen pen(Qt::red);
        pen.setWidth(4);
        painter.setBrush(Qt::transparent);
        painter.setPen(pen);
        painter.drawRect(xCursor*gridSize+1,yCursor*gridSize+1,gridSize-1,gridSize-1);
        if(yCursor!=scoreHeight/2)
        {painter.drawRect(xCursor*gridSize+1,(scoreHeight-1-yCursor)*gridSize+1,gridSize-1,gridSize-1);}

    }
}
