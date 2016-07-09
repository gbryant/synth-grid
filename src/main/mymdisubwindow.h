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


#ifndef MYMDISUBWINDOW_H
#define MYMDISUBWINDOW_H

#include <QMdiSubWindow>

#include "scoredocument.h"
class MainWindow;

class MyMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    MyMdiSubWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    ScoreDocument *sd;
    MainWindow *mw;
    //void closeEvent(QCloseEvent *closeEvent);
};

#endif // MYMDISUBWINDOW_H
