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


#include "synthparameters.h"
#include "mainwindow.h"
#include "ui_synthparameters.h"
#include <QCloseEvent>
#include <QMdiArea>
#include <QString>
#include <iostream>

#define log(X) std::cout << X << std::endl;



void SynthParameters::closeEvent(QCloseEvent *closeEvent)
{
    log("void SynthParameters::closeEvent(QCloseEvent *closeEvent)");
    closeEvent->accept();
    mw->synthParameters=0;
}

SynthParameters::SynthParameters(QWidget *parent/*, std::vector<Instrument> *instrumentsIn*/) :QWidget(parent),ui(new Ui::SynthParameters)
{
    log("SynthParameters::SynthParameters()");
    ui->setupUi(this);
    mdiSubWindow=0;
    mw=0;

    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),SLOT(indexChange(QString)));
}

void SynthParameters::indexChange(QString text)
{
    if(mw->activeScoreWindow)
    {
        if(mw->activeScoreWindow->sd->instrument!=text)
        {
            mw->activeScoreWindow->sd->instrument=text;
            mw->activeScoreWindow->sd->setIsDocumentEdited(true);
        }
    }
}

SynthParameters::~SynthParameters()
{

    delete ui;
}

void SynthParameters::windowMenuAction()
{
    mdiSubWindow->mdiArea()->setActiveSubWindow(mdiSubWindow);
}

void SynthParameters::setInstrument(QString instrument)
{
    if(instrument=="default"){ui->comboBox->setCurrentIndex(1);}
    else
    {
        int index = ui->comboBox->findText(instrument);
        if(index<0){ui->comboBox->setCurrentIndex(0);}
        else{ui->comboBox->setCurrentIndex(index);}
    }
}

void SynthParameters::setInstruments(QMap<QString,Instrument*> instruments)
{

    QMapIterator<QString, Instrument*> iterator(instruments);
    while (iterator.hasNext())
    {
        iterator.next();
        if(iterator.key()!="default")
        {ui->comboBox->addItem(iterator.value()->name);}
    }
    /*
    for(int i=0;i<instruments.count();i++)
    {
        ui->comboBox->addItem(instruments.key());
    }
    */
}

