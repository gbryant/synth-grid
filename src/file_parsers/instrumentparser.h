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


#ifndef INSTRUMENTPARSER_H
#define INSTRUMENTPARSER_H

#include <QXmlDefaultHandler>
#include "instrument.h"
#include <vector>

class InstrumentParser : public QXmlDefaultHandler
{
public:
    InstrumentParser();
    bool startDocument();
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool characters(const QString &ch);
private:
    bool inInstrument;
    bool inOscillator;
public:
    Instrument *instrument;
};

#endif // INSTRUMENTPARSER_H
