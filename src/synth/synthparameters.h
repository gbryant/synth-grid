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


#ifndef SYNTHPARAMETERS_H
#define SYNTHPARAMETERS_H

#include "instrument.h"

#include <QWidget>
#include <vector>
#include <QMdiSubWindow>
#include <QList>

class MainWindow;


namespace Ui {
    class SynthParameters;
}

class SynthParameters : public QWidget
{
    Q_OBJECT

public:
    explicit SynthParameters(QWidget *parent = 0/*, std::vector<Instrument> *instrumentsIn=0*/);
    ~SynthParameters();
     QAction *menuAction;
     QMdiSubWindow *mdiSubWindow;
     void setInstruments(QMap<QString,Instrument*> instruments);
     void setInstrument(QString instrument);
     MainWindow *mw;
     void closeEvent(QCloseEvent *closeEvent);


public:
    Ui::SynthParameters *ui;

public slots:
    void windowMenuAction(void);
    void indexChange(QString text);


};

#endif // SYNTHPARAMETERS_H
