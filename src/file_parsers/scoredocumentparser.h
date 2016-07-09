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


#ifndef SCOREDOCUMENTPARSER_H
#define SCOREDOCUMENTPARSER_H

#include "notation.h"
#include <QXmlDefaultHandler>
#include <vector>


class ScoreDocumentParser : public QXmlDefaultHandler
{
public:
    ScoreDocumentParser();
    bool startDocument();
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool characters(const QString &ch);
//private:
public:
    bool inScore;
    bool inNotes;
    bool inInstrument;
    int width,height;
    std::vector<ScoreNote> notes;
    QString instrument;
};

#endif // SCOREDOCUMENTPARSER_H
