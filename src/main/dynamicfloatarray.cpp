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


#include "dynamicfloatarray.h"

DynamicFloatArray::DynamicFloatArray(int sizeIn)
{
    size=sizeIn;
    data = (float*)malloc(sizeof(float)*size);
    pos=0;
    oldPos=0;
}



int DynamicFloatArray::writeRepeating(float value, int length)
{
    if((pos+1+length)>size)
    {
        int result=growArray(length);
        if(result<0){return -1;}
    }
    oldPos=pos;

    for(int i=0;i<length;i++)
    {
        data[pos]=value;
        pos++;
    }

}

int DynamicFloatArray::growArray(int amount)
{
    float *tmp_ptr = (float*)realloc(data, size+amount);
    if (tmp_ptr == NULL)
    {
        // realloc failed, orig_ptr still valid so you can clean up
        return -1;
    }
    else
    {
        // Only overwrite orig_ptr once you know the call was successful
        data = tmp_ptr;
    }

}
