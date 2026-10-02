#include "ScalarControl.h"
#include "ScalarProfile.h"
#include "WaveGen.h"
#include "WaveGen_LUTs.h"
#include "PWM_Schemes.h"
#include "Timer_Driver.h"

#define SWITCHING_FREQUENCY 1000

static volatile uint8_t taskReady;

static WaveGen sin_lut;
static WaveGen cos_lut;
static float CurrentFrequency;
static uint32_t periodsSinceLastFrequencyChange;
static uint32_t periodsToChangeFrequency;
static float Setpoint = 10.0f;// 10 hz speed

typedef struct {
    float alpha;
    float beta;
} VoltageVector_t;

static VoltageVector_t GetVoltageVector(float magnitude);

void ScalarControl_Init()
{
    CurrentFrequency = 0.0f;
    periodsSinceLastFrequencyChange = 0;
    periodsToChangeFrequency = 1000;
    sin_lut = WaveGen_Create(SINE);
    cos_lut = WaveGen_Create(COSINE);
    WaveGen_SetFrequency(sin_lut,CurrentFrequency,SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cos_lut,CurrentFrequency,SWITCHING_FREQUENCY);
    ScalarProfile_Init(230.0f, 50.0f, 50.0f,  10.0f);
}

void ScalarControl_Task()
{
    if(taskReady == 0)
        return;

    //Get ADC injection values

    if(CurrentFrequency - Setpoint < 0 && periodsSinceLastFrequencyChange >= periodsToChangeFrequency)
    {
        CurrentFrequency += 1.0f;
        WaveGen_SetFrequency(sin_lut,CurrentFrequency,SWITCHING_FREQUENCY);
        WaveGen_SetFrequency(cos_lut,CurrentFrequency,SWITCHING_FREQUENCY);
        periodsSinceLastFrequencyChange = 0;
    }

    float voltageMagnitude = ScalarProfile_GetVoltage(CurrentFrequency);
    VoltageVector_t vector = GetVoltageVector(voltageMagnitude);

    Duty_t dutyCycles = SVPWM(vector.alpha,vector.beta,);
    PWM_SetDuty(dutyCycles.duty_a,dutyCycles.duty_b,dutyCycles.duty_c);

    periodsSinceLastFrequencyChange++;
    taskReady = 0;
}

static VoltageVector_t GetVoltageVector(float magnitude)
{
    int32_t sinValue = WaveGen_Get(sin_lut);
    int32_t cosValue = WaveGen_Get(cos_lut);

    float sinNorm = (float)sinValue / (float)INT32_MAX;
    float cosNorm = (float)cosValue / (float)INT32_MAX;

    VoltageVector_t result;
    result.alpha = cosNorm * magnitude;
    result.beta  = sinNorm * magnitude;

    return result;
}

void ScalarProfile_SetTaskReady()
{
    taskReady = 1;
}



