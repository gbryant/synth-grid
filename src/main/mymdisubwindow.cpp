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


#include "mymdisubwindow.h"
#include "mainwindow.h"
#include <QCloseEvent>
#include <iostream>

#define log(X) std::cout << X << std::endl;


MyMdiSubWindow::MyMdiSubWindow(QWidget * parent, Qt::WindowFlags flags) : QMdiSubWindow(parent,flags)
{
    sd=0;
    mw=0;
}

/*
void MyMdiSubWindow::closeEvent(QCloseEvent *closeEvent)
{
    sd->closeEvent(closeEvent);
    if(closeEvent->isAccepted())
    {
        if(this==mw->activeScoreWindow)
        {
            log("mw->activeScoreWindow=0;");
            mw->activeScoreWindow=0;
        }
    }
}
*/
