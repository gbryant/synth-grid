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


#include "instrumentparser.h"
#include <iostream>


#define log(X) std::cout << X << std::endl;

InstrumentParser::InstrumentParser()
{
    instrument=0;
}

//void InstrumentParser::setInstruments(std::vector<Instrument> *instrumentsIn){instruments=instrumentsIn;}

bool InstrumentParser::startDocument()
{
    inInstrument=false;
    inOscillator=false;

    return true;
}

bool InstrumentParser::characters(const QString &ch)
{
    if(inOscillator)
    {
        log(ch.toStdString());
        instrument->oscillators.append(ch);
    }
    return true;
}

bool InstrumentParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
     if(localName=="instrument")
     {inInstrument=false;log("instrument endElement");}
     else if(localName=="oscillator")
     {inOscillator=false;log("oscillator endElement");}
     else{log(localName.toStdString()+" endElement not handled");}

     return true;
}

bool InstrumentParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    if(inInstrument && localName=="oscillator")
    {
        inOscillator=true;
        log("oscillator startElement");

    }
    else if(localName=="instrument")
    {inInstrument=true;log("instrument startElement");}
    else{log(localName.toStdString()+" startElement not handled");}

    return true;
}


