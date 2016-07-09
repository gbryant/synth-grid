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


#include "scoredocument.h"
#include "scoredocumentparser.h"
#include "scoreview.h"
#include "synthesizer.h"
#include "mymdisubwindow.h"
#include <iostream>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiArea>
#include <QActionGroup>
#include <QXmlStreamWriter>
#include <QString>

#include "mainwindow.h"

//#include "defaultinstrument.h"

extern Synthesizer *synth;

#define log(X) std::cout << X << std::endl;

ScoreDocument::ScoreDocument(QWidget *parent) :QScrollArea(parent)
{
    scoreData = new ScoreData(200,88);
    scoreView = new ScoreView(this);
    setWidget(scoreView);
    setWidgetResizable(false);
    isDocumentEdited = false;
    neverBeenSaved = true;
    noteArray.n = (Note*)malloc(sizeof(Note)*10000);
    instrument = "default";
}

ScoreDocument::~ScoreDocument()
{
    delete scoreData;
    delete scoreView;
    free(noteArray.n);
}


void ScoreDocument::play()
{
    synth->play(scoreData,instrument);
}

void ScoreDocument::windowMenuAction()
{
    QMdiSubWindow *window = qobject_cast<QMdiSubWindow *>(this->parentWidget());
    window->mdiArea()->setActiveSubWindow(window);
    scoreView->setFocus();
}

bool ScoreDocument::save()
{
    if(neverBeenSaved)
    {
        return saveAs();
    }
    else
    {
        return saveFile(filePath);
    }
}

bool ScoreDocument::saveAs()
{
#ifdef Q_WS_X11
    QString path="/home/gbryant/Documents/ScoreFiles";
#endif
#ifdef Q_WS_MAC
    QString path="/Users/gbryant/Documents/ScoreFiles";
#endif
#ifdef Q_WS_WIN
    QString path=("C:/Users/golgi/Music/ScoreFiles");
#endif
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save Score File", path,
                                                    "Score files (*.msd)");

    if (fileName.isEmpty())
    {return false;}

    filePath=fileName;

    QFileInfo fi(filePath);
    QString name = fi.fileName();
    name.append(" [*]");

    QMdiSubWindow *window = qobject_cast<QMdiSubWindow *>(this->parentWidget());

    window->setWindowTitle(name);

    return saveFile(fileName);
}

bool ScoreDocument::saveFile(const QString &filePathIn)
{
    QFile file;
    file.setFileName(filePathIn);
    if(!file.open(QIODevice::WriteOnly)){return false;}

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("score");

    xmlWriter.writeTextElement("instrument",instrument);

    xmlWriter.writeStartElement("notes");


    QString widthStr;
    widthStr = widthStr.number(scoreData->getWidth());
    xmlWriter.writeAttribute("width",widthStr);

    QString heightStr;
    heightStr = heightStr.number(scoreData->getHeight());
    xmlWriter.writeAttribute("height",heightStr);

    QString xStr,yStr;

    for(int i=0;i<scoreData->getWidth();i++)
    {
        scoreData->getNotes(i,&noteArray);
        for(int j=0;j<noteArray.count;j++)
        {
            xStr = xStr.number(i);
            yStr = yStr.number(noteArray.n[j].y);
            xmlWriter.writeStartElement("note");
            xmlWriter.writeAttribute("x",xStr);
            xmlWriter.writeAttribute("y",yStr);
            xmlWriter.writeEndElement();
        }
    }


    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();

    neverBeenSaved = false;

    return true;


}



int ScoreDocument::openFile(const QString &filePathIn)
{

    ScoreDocumentParser handler;
    QFile file(filePathIn);
    if(!file.open(QIODevice::ReadOnly)){return -1;}
    QXmlInputSource source(&file);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    bool ok=reader.parse(source);
    if(!ok){file.close();return -1;}
    file.close();

    delete scoreData;
    delete scoreView;

    scoreData = new ScoreData(handler.width,handler.height);
    scoreView = new ScoreView(this);
    setWidget(scoreView);
    setWidgetResizable(false);

    for(int i=0;i<handler.notes.size();i++)
    {
        scoreData->setNote(handler.notes[i].x,handler.notes[i].y);
    }

    instrument = handler.instrument;

    filePath = filePathIn;
    QFileInfo fi(filePathIn);
    fileName=fi.fileName();
    neverBeenSaved=false;

    //for(int i=0;i<handler.notes.size();i++)
    //{
    //    log("x:"+QString().number(handler.notes[i].x).toStdString()+" y:"+QString().number(handler.notes[i].y).toStdString());
    //}
    return 1;
}

void ScoreDocument::closeEvent(QCloseEvent *event)
{
    if(okToClose())
    {
        MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(this->parentWidget());
        if(window==window->mw->activeScoreWindow)
        {
            log("mw->activeScoreWindow=0;");
            window->mw->activeScoreWindow=0;
        }
        event->accept();
        delete menuAction;
        return;
    }
    else
    {event->ignore();return;}
}

void ScoreDocument::setIsDocumentEdited(bool isEdited)
{
    QMdiSubWindow *window = qobject_cast<QMdiSubWindow *>(this->parentWidget());
    if(isDocumentEdited==isEdited){return;}
    isDocumentEdited=isEdited;
    if(isDocumentEdited)
    {
        window->setWindowModified(true);
    }
    else
    {
        window->setWindowModified(false);
    }

    return;
}

bool ScoreDocument::okToClose()
{
    if(isDocumentEdited)
    {
        QMdiSubWindow *window = qobject_cast<QMdiSubWindow *>(this->parentWidget());
        QMessageBox msgBox;
        msgBox.setParent(window);
        msgBox.setWindowModality(Qt::WindowModal);
        msgBox.setText("The document has been modified.\n" "Do you want to save your changes?");
        msgBox.addButton(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.addButton(QMessageBox::Cancel);
        int r = msgBox.exec();
        if (r == QMessageBox::Yes)
        {return save();}
        else if (r == QMessageBox::Cancel)
        {return false;}
        else
        {return true;}
    }
    else
    {
        return true;
    }
}
