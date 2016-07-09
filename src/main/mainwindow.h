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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QMdiSubWindow>

#include "scoredocument.h"
#include "scoreparameters.h"
#include "synthparameters.h"
#include "mymdisubwindow.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QActionGroup *actionGroup;
    QActionGroup *entryModeActionGroup;
    int documentCounter;
    void insertScoreDocument(ScoreDocument *sd);
public:
    SynthParameters *synthParameters;
    ScoreParameters *scoreParameters;

    MyMdiSubWindow *activeScoreWindow;

private slots:
    void actionExit(void);
    void actionNew(void);
    void actionOpen(void);
    void actionClose(void);
    void actionSave(void);
    void actionSaveAs(void);
    void updateActions(QMdiSubWindow *mdiSubWindow);
    void actionNormal(void);
    void actionMirror(void);
    void actionScoreParameters(void);
    void actionSynthParameters(void);
    void enableActionScoreParameters(void);
    void enableActionSynthParameters(void);
    void actionZoomIn(void);
    void actionZoomOut(void);
};

#endif // MAINWINDOW_H
