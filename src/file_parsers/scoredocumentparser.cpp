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


#include "scoredocumentparser.h"
#include <iostream>


#define log(X) std::cout << X << std::endl;

ScoreDocumentParser::ScoreDocumentParser()
{
    std::cout << "parse!" << std::endl;
    width=0;
    height=0;
    instrument = "default";
}

bool ScoreDocumentParser::characters(const QString &ch)
{
    if(inInstrument)
    {
        instrument=ch;
    }

    return true;
}

bool ScoreDocumentParser::startDocument()
{
    inScore=false;
    inNotes=false;
    inInstrument=false;
    return true;
}

bool ScoreDocumentParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    if(localName=="score")
    {inScore=false;log("score endElement");}
    else if(localName=="notes")
    {inNotes=false;log("notes endElement");}
    else if(localName=="instrument")
    {inInstrument=false;log("instrument endElement");}

    return true;
}


bool ScoreDocumentParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    ScoreNote noteIn;
    if( inScore && localName == "notes" )
    {
        inNotes=true;
        log("notes startElement");
        log("read note attributes");
        for(int i=0;i<atts.count();i++)
        {
            if(atts.localName(i)=="width"){width=atts.value(i).toInt();}
            else if(atts.localName(i)=="height"){height=atts.value(i).toInt();}
        }
    }
    else if(inNotes && localName=="note")
    {
        log("read note data");
        for(int i=0;i<atts.count();i++)
        {
            if(atts.localName(i)=="x"){noteIn.x=atts.value(i).toInt();}
            else if(atts.localName(i)=="y"){noteIn.y=atts.value(i).toInt();}
        }
         notes.push_back(noteIn);

    }
    else if(localName=="score")
    {inScore=true;log("score startElement");}
    else if(localName=="instrument"){inInstrument=true;log("instrument startElement");}
    else{log(localName.toStdString()+" not handled");}

    //log("width:"+QString().number(width).toStdString()+ " height:"+QString().number(height).toStdString());

    return true;
}
