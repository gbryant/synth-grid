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


#ifndef SCOREDOCUMENT_H
#define SCOREDOCUMENT_H

#include <QScrollArea>
#include <QMdiSubWindow>
#include <QXmlStreamReader>

#include "scoredata.h"



class ScoreView;


class ScoreDocument : public QScrollArea
{
    Q_OBJECT

public:
    explicit ScoreDocument(QWidget *parent = 0);
    ~ScoreDocument();
    void closeEvent(QCloseEvent *event);
    void setIsDocumentEdited(bool isEdited);

public:
    ScoreView *scoreView;
    ScoreData *scoreData;


    bool isDocumentEdited;
    bool neverBeenSaved;
    QString filePath;
    QString fileName;
    QString instrument;

    bool okToClose(void);
    bool save(void);
    bool saveAs(void);
    bool saveFile(const QString &filePathIn);
    int openFile(const QString &filePathIn);
    void play(void);


    QAction *menuAction;

private:
    NoteArray noteArray;

public slots:
    void windowMenuAction(void);
};

#endif // SCOREDOCUMENT_H
