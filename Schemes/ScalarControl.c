#include "ScalarControl.h"
#include "ScalarProfile.h"
#include "WaveGen.h"

//TO DO: timer task observer

static WaveGen SINE;
static float CurrentFrequency;

void ScalarControl_Init()
{
    SINE = WaveGen_Create(SINE);
    ScalarProfile_Init(230.0f, 50.0f, 50.0f,  10.0f);
    CurrentFrequency = 0.0f;
}

void ScalarControl_Task()
{
    //OCP

    //if(CurrentFrequency - Setpoint <= 0 && timer_task_completed()) increase frequency;

    int32_t Voltage = WaveGen_Get(SINE);
    float VoltageFraction = (float)Voltage / (float)INT32_MAX;
    float VoltageScaleing = ScalarProfile_GetVoltage(CurrentFrequency);
    float setVoltage = VoltageFraction*VoltageScaleing;

    //float alpha
    //float beta 
    //PWM method
}