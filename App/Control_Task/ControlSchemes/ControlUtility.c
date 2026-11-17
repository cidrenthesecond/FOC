#include "ControlUtility.h"

VoltageVector_t GetVoltageVector(int32_t sinValue,int32_t cosValue,float magnitude)
{
    //int32_t sinValue = WaveGen_Get(sinWave);
    //int32_t cosValue = WaveGen_Get(cosWave);

    float sinNorm = (float)sinValue / (float)INT32_MAX;
    float cosNorm = (float)cosValue / (float)INT32_MAX;

    VoltageVector_t result;
    result.alpha = cosNorm * magnitude;
    result.beta  = sinNorm * magnitude;

    return result;
}