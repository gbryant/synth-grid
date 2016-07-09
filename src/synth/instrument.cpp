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


#include "instrument.h"

Instrument::Instrument()
{
    amplitudeEnvelope.initialLevel=0.0;
    amplitudeEnvelope.attackLevel=1.0;
    amplitudeEnvelope.attackLength=.0325;
    amplitudeEnvelope.decayLength=.0125;
    amplitudeEnvelope.releaseLength=.25;
    amplitudeEnvelope.sustainLevel=.60;
    amplitudeEnvelope.releaseLevel=0.0;
}
