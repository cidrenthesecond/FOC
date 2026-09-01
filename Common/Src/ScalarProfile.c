#include "ScalarProfile.h"

static float ratedVoltage;
static float boostVoltage;
static float ratedFrequency;
static float boostFrequency;
static float slope;
static int isInitialized = 0;

void ScalarProfile_Init(float set_ratedVoltage, float set_boostVoltage,float set_ratedFrequency, float set_boostFrequency)
{
    ratedVoltage = set_ratedVoltage;
    boostVoltage = set_boostVoltage;
    ratedFrequency = set_ratedFrequency;
    boostFrequency = set_boostFrequency;
    slope = (ratedVoltage - boostVoltage) / (ratedFrequency - boostFrequency);
    isInitialized = 1;
}

float ScalarProfile_GetVoltage(float frequency)
{
    if(!isInitialized)
        return 0.0f;

    if(frequency <= boostFrequency)
        return boostVoltage;

    if(frequency >= ratedFrequency)
        return ratedVoltage;

    float offset = slope * (frequency - boostFrequency);
    return boostVoltage + offset;
}

void ScalarProfile_Destroy()
{
    isInitialized = 0;
}