#include "ScalarControl.h"
#include "ADC_Service.h"
#include "ScalarProfile.h"
#include "WaveGen.h"
#include "WaveGen_LUTs.h"
#include "PWM_Schemes.h"
#include "Timer_Driver.h"
#include "main.h"
#include "stdbool.h"

#define SWITCHING_FREQUENCY 1000.0f

static WaveGen sin_wave;
static WaveGen cos_wave;
static float CurrentFrequency;
static float FrequencyStep;
static uint32_t periodsSinceLastFrequencyChange;
static uint32_t periodsToChangeFrequency;
static float Setpoint = -5.0f;

typedef struct {
    float alpha;
    float beta;
} VoltageVector_t;

static bool IsSetPointReached();
static float Absolute_F(float x);
static VoltageVector_t GetVoltageVector(float magnitude);
static Duty_t          SwapPhases      (Duty_t duty);

Analog_Feedback_t test;

void ScalarControl_Init()
{
    CurrentFrequency = 0.0f;
    FrequencyStep = 1.0f;
    periodsSinceLastFrequencyChange = 0;
    periodsToChangeFrequency = 2000;
    sin_wave = WaveGen_Create(&SINE);
    cos_wave = WaveGen_Create(&COSINE);
    WaveGen_SetFrequency(sin_wave,CurrentFrequency,SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cos_wave,CurrentFrequency,SWITCHING_FREQUENCY);
    ScalarProfile_Init(230.0f, 0.0f, 50.0f,  0.0f);
    ADC_PrepareForControlLoop();
}

void ScalarControl_BeforeLaunch()
{
    CurrentFrequency = 0.0f;
    periodsSinceLastFrequencyChange = 0;
    periodsToChangeFrequency = 2000;  // needs config
    WaveGen_Reset(sin_wave);
    WaveGen_Reset(cos_wave);
    WaveGen_SetFrequency(sin_wave, CurrentFrequency, SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cos_wave, CurrentFrequency, SWITCHING_FREQUENCY);
    ADC_PrepareForControlLoop();
}

void ScalarControl_Task()
{
    LL_GPIO_SetOutputPin(WCET_GPIO_Port, WCET_Pin);
    Analog_Feedback_t measurements = ADC_GetAnalogFeedback();

    if(!IsSetPointReached() && periodsSinceLastFrequencyChange >= periodsToChangeFrequency)
    {
        if(Setpoint > CurrentFrequency)
            CurrentFrequency += FrequencyStep;
        else if (Setpoint < CurrentFrequency)
            CurrentFrequency -= FrequencyStep;

        WaveGen_SetFrequency(sin_wave,Absolute_F(CurrentFrequency),SWITCHING_FREQUENCY);
        WaveGen_SetFrequency(cos_wave,Absolute_F(CurrentFrequency),SWITCHING_FREQUENCY);
        periodsSinceLastFrequencyChange = 0;
    }

    float voltageMagnitude = ScalarProfile_GetVoltage(Absolute_F(CurrentFrequency)); //In need of phase RMS to line Amplitude
    VoltageVector_t vector = GetVoltageVector(voltageMagnitude);

    Duty_t dutyCycles = SPWM(vector.alpha,vector.beta,(float)measurements.DcBus/1000.0f);

    if(CurrentFrequency < 0)
        dutyCycles = SwapPhases(dutyCycles);

    PWM_SetDuty(dutyCycles.duty_a,dutyCycles.duty_b,dutyCycles.duty_c);

    periodsSinceLastFrequencyChange++;
    LL_GPIO_ResetOutputPin(WCET_GPIO_Port, WCET_Pin);
}

static bool IsSetPointReached()
{
    if(Setpoint > 0.0f)
        return CurrentFrequency > Setpoint;
    else if(Setpoint < 0.0f)
        return CurrentFrequency < Setpoint;
    else
        return true;
}

static float Absolute_F(float x)
{
    if(x >= 0.0f)
        return x;
    else
        return -x;
}

static VoltageVector_t GetVoltageVector(float magnitude)
{
    int32_t sinValue = WaveGen_Get(sin_wave);
    int32_t cosValue = WaveGen_Get(cos_wave);

    float sinNorm = (float)sinValue / (float)INT32_MAX;
    float cosNorm = (float)cosValue / (float)INT32_MAX;

    VoltageVector_t result;
    result.alpha = cosNorm * magnitude;
    result.beta  = sinNorm * magnitude;

    return result;
}

static Duty_t SwapPhases (Duty_t duty)
{
    float helper = duty.duty_a;

    duty.duty_a = duty.duty_b;
    duty.duty_b = helper;

    return duty;
}



