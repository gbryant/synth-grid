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


#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "notation.h"

class Oscillator
{
public:
    Oscillator();
    void writeSilence(FloatData *data,int noteLength);
    void addSquareWave(FloatData *data,float amplitude, float frequency,int noteLength);
    void addSquareWaveP(FloatData *data,float amplitude1,float amplitude2, float frequency,int noteLength);
    void addSineWave(FloatData *data,float amplitude, float frequency,int noteLength);
    void addTriangleWave(FloatData *data,float amplitude, float frequency,int noteLength);
    void addSawToothWave(FloatData *data,float amplitude, float frequency,int noteLength);
};

#endif // OSCILLATOR_H
