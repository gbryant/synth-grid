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


#include "oscillator.h"
#include <cmath>
#define M_PI 3.14159265358979323846

Oscillator::Oscillator()
{
}

void Oscillator::writeSilence(FloatData *data, int noteLength)
{
    for(int i=0;i<noteLength;i++)
    {
        data->data[data->pos] = 0;data->pos++;
    }
}
void Oscillator::addSineWave(FloatData *data, float amplitude, float frequency, int noteLength)
{
    float phase=0;

    for(int i=0;i<noteLength;i++)
    {
        data->data[data->pos] += amplitude*sin(phase);data->pos++;
        phase += 2*M_PI*frequency/44100;
        if(phase>M_PI*2){phase-=M_PI*2;}
    }
}

void Oscillator::addSawToothWave(FloatData *data, float amplitude, float frequency, int noteLength)
{
    float phase=0;

    for(int i=0;i<noteLength;i++)
    {
        data->data[data->pos] += amplitude-(amplitude/M_PI*phase);data->pos++;
        phase += 2*M_PI*frequency/44100;
        if(phase>M_PI*2){phase-=M_PI*2;}
    }
}

void Oscillator::addTriangleWave(FloatData *data, float amplitude, float frequency, int noteLength)
{
    float phase=0;

    for(int i=0;i<noteLength;i++)
    {
        if(phase<M_PI)
        {data->data[data->pos] += -amplitude+(2*amplitude/M_PI)*phase;data->pos++;}
        else
        {data->data[data->pos] += 3*amplitude-(2*amplitude/M_PI)*phase;data->pos++;}
        phase += 2*M_PI*frequency/44100;
        if(phase>M_PI*2){phase-=M_PI*2;}
    }
}

void Oscillator::addSquareWave(FloatData *data,float amplitude, float frequency, int noteLength)
{
    float phase=0;

    for(int i=0;i<noteLength;i++)
    {
        if(phase < M_PI)
        {
            data->data[data->pos] += amplitude;data->pos++;
        }
        else
        {
            data->data[data->pos] += -amplitude;data->pos++;
        }
        phase += 2*M_PI*frequency/44100;
        if(phase>M_PI*2){phase-=M_PI*2;}
    }
}

void Oscillator::addSquareWaveP(FloatData *data,float amplitude1, float amplitude2, float frequency, int noteLength)
{
    float phase=0;
    float stepSize=.02;
    float amp=amplitude1;
    bool rising=true;
    for(int i=0;i<noteLength;i++)
    {
        if(phase < M_PI)
        {
            data->data[data->pos] += amp;data->pos++;
        }
        else
        {
            data->data[data->pos] += -amp;data->pos++;
        }
        phase += 2*M_PI*frequency/44100;
        if(phase>M_PI*2){phase-=M_PI*2;}
        if(rising)
        {
            amp+=stepSize;
            if(amp>amplitude2)
            {
                rising=false;
                amp-=stepSize;
            }
        }
        else
        {
            amp-=stepSize;
            if(amp<amplitude1)
            {
                rising=true;
                amp+=stepSize;
            }
        }
    }
}
