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


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "scoredocument.h"
#include "scoreview.h"
#include "synthparameters.h"
#include "synthesizer.h"
#include "mymdisubwindow.h"


#include <QScrollArea>
#include <QCloseEvent>
#include <QFileDialog>
#include <QList>
#include <QMdiSubWindow>

#include <QXmlStreamWriter>

#include <iostream>

#define log(X) std::cout << X << std::endl;

extern Synthesizer *synth;


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),this, SLOT(updateActions(QMdiSubWindow*)));
    actionGroup = new QActionGroup(ui->menuWindow);

    entryModeActionGroup = new QActionGroup(ui->menuEntry_Mode);
    entryModeActionGroup->addAction(ui->actionNormal);
    entryModeActionGroup->addAction(ui->actionMirror);

    documentCounter=1;
    activeScoreWindow=0;

    synthParameters = 0;






}

void MainWindow::actionNormal()
{
    MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(ui->mdiArea->activeSubWindow());
    if(window)
    {
        window->sd->scoreView->setEntryMode(ScoreView::normal);
    }
}

void MainWindow::actionMirror()
{
    MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(ui->mdiArea->activeSubWindow());
    if(window)
    {
        window->sd->scoreView->setEntryMode(ScoreView::mirror);
    }
}

void MainWindow::actionZoomIn()
{
    MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(ui->mdiArea->activeSubWindow());
    if(window)
    {
        window->sd->scoreView->gridSize++;
        window->sd->scoreView->setGeometry(0,0,window->sd->scoreData->getWidth()*window->sd->scoreView->gridSize+1,window->sd->scoreData->getHeight()*window->sd->scoreView->gridSize+1);
        window->sd->scoreView->update();
    }
}

void MainWindow::actionZoomOut()
{
    MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(ui->mdiArea->activeSubWindow());
    if(window)
    {
        if(window->sd->scoreView->gridSize>2)
        {
            window->sd->scoreView->gridSize--;
            window->sd->scoreView->setGeometry(0,0,window->sd->scoreData->getWidth()*window->sd->scoreView->gridSize+1,window->sd->scoreData->getHeight()*window->sd->scoreView->gridSize+1);
            window->sd->scoreView->update();
        }
    }
}

MainWindow::~MainWindow()
{
    //delete synth;
    delete ui;
}

void MainWindow::updateActions(QMdiSubWindow *mdiSubWindow)
{
    MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(mdiSubWindow);
    if(window)
    {
        activeScoreWindow = window;
        if(window->sd)
        {
            switch(window->sd->scoreView->getEntryMode())
            {
            case ScoreView::normal:
                ui->actionNormal->setChecked(true);
                break;
            case ScoreView::mirror:
                ui->actionMirror->setChecked(true);
                break;
            }
            if(synthParameters)
            {
                synthParameters->setInstrument(activeScoreWindow->sd->instrument);
                synthParameters->update();
            }
        }
    }
}

void MainWindow::insertScoreDocument(ScoreDocument *sd)
{
    MyMdiSubWindow *window = new MyMdiSubWindow;
    window->sd=sd;
    window->mw=this;
    window->setWidget(sd);
    window->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(window);


    //window->setMaximumSize(sd->scoreView->geometry().width()+10,sd->scoreView->geometry().height()+30);
    //window->resize(window->maximumSize());


    sd->menuAction = ui->menuWindow->addAction(sd->fileName);
    connect(sd->menuAction,SIGNAL(triggered()),sd,SLOT(windowMenuAction()));

    QString str = sd->fileName;
    str.append(" [*]");
    window->setWindowTitle(str);

    sd->setIsDocumentEdited(false);

    actionGroup->addAction(sd->menuAction);
    sd->menuAction->setCheckable(true);
    sd->menuAction->setChecked(true);

    window->show();

    sd->scoreView->setFocus();
}

void MainWindow::actionOpen(void)
{
    ScoreDocument *sd;

#ifdef Q_WS_X11
    QString path="/home/gbryant/Documents/ScoreFiles";
#endif
#ifdef Q_WS_MAC
    QString path="/Users/gbryant/Documents/ScoreFiles";
#endif
#ifdef Q_WS_WIN
    QString path="C:/Users/golgi/Music/ScoreFiles";
#endif

    QString fileName = QFileDialog::getOpenFileName(this,"Open File",path,"Score Document(*.msd)");
    if (fileName.isEmpty())
    {return;}

    sd = new ScoreDocument();

    if(sd->openFile(fileName)<0)
    {delete sd;return;}


    insertScoreDocument(sd);



}

void MainWindow::actionNew(void)
{
    ScoreDocument *sd;
    sd = new ScoreDocument();
    QString str("SynthGrid Document ");
    str.append(QString().number(documentCounter));documentCounter++;
    sd->fileName=str;

    insertScoreDocument(sd);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();

    QList<QMdiSubWindow*> list = ui->mdiArea->subWindowList();

    if (!ui->mdiArea->subWindowList().isEmpty())
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::actionSave(void)
{
    MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(ui->mdiArea->activeSubWindow());
    if(window)
    {
        if(window->sd->save()){window->sd->setIsDocumentEdited(false);}
    }
}

void MainWindow::actionSaveAs(void)
{
    MyMdiSubWindow *window = qobject_cast<MyMdiSubWindow *>(ui->mdiArea->activeSubWindow());
    if(window)
    {
        if(window->sd->saveAs()){window->sd->setIsDocumentEdited(false);}
    }
}

void MainWindow::actionClose(void)
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::actionExit(void)
{
    this->close();
}

void MainWindow::enableActionScoreParameters()
{
    ui->actionScoreParameters->setEnabled(true);
}

void MainWindow::enableActionSynthParameters()
{
    ui->actionSynthParameters->setEnabled(true);
}

void MainWindow::actionScoreParameters()
{
    /*
    sp = new ScoreParameters();
    sp->mdiSubWindow = ui->mdiArea->addSubWindow(sp);
    sp->mdiSubWindow->setWindowTitle("Score Parameters");
    sp->mdiSubWindow->show();
    ui->actionScoreParameters->setDisabled(true);
    connect(sp->mdiSubWindow,SIGNAL(destroyed()),this,SLOT(enableActionScoreParameters()));

    sp->menuAction = ui->menuWindow->addAction("Score Parameters");
    connect(sp->menuAction,SIGNAL(triggered()),sp,SLOT(windowMenuAction()));
    actionGroup->addAction(sp->menuAction);
    sp->menuAction->setCheckable(true);
    sp->menuAction->setChecked(true);
    */

}

void MainWindow::actionSynthParameters()
{


    synthParameters = new SynthParameters();
    synthParameters->mw=this;

    synthParameters->setInstruments(synth->instruments);

    synthParameters->mdiSubWindow = ui->mdiArea->addSubWindow(synthParameters);
    synthParameters->mdiSubWindow->setWindowTitle("Synth Parameters");
    synthParameters->mdiSubWindow->show();
    ui->actionSynthParameters->setDisabled(true);
    connect(synthParameters->mdiSubWindow,SIGNAL(destroyed()),this,SLOT(enableActionSynthParameters()));

    synthParameters->menuAction = ui->menuWindow->addAction("Synth Parameters");
    connect(synthParameters->menuAction,SIGNAL(triggered()),synthParameters,SLOT(windowMenuAction()));
    actionGroup->addAction(synthParameters->menuAction);
    synthParameters->menuAction->setCheckable(true);
    synthParameters->menuAction->setChecked(true);
    if(activeScoreWindow)
    {
        synthParameters->setInstrument(activeScoreWindow->sd->instrument);
    }



}
