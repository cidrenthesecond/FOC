#include "ScalarControl.h"
#include "ADC_Service.h"
#include "ScalarProfile.h"
#include "WaveGen.h"
#include "WaveGen_LUTs.h"
#include "PWM_Schemes.h"
#include "Timer_Driver.h"
#include "main.h"
#include "stm32h5xx_ll_gpio.h"

#define SWITCHING_FREQUENCY 1000

static volatile uint8_t taskReady;

static WaveGen sin_wave;
static WaveGen cos_wave;
static float CurrentFrequency;
static uint32_t periodsSinceLastFrequencyChange;
static uint32_t periodsToChangeFrequency;
static float Setpoint = 6.0f;// 10 hz speed



typedef struct {
    float alpha;
    float beta;
} VoltageVector_t;

static VoltageVector_t GetVoltageVector(float magnitude);

void ScalarControl_Init()
{
    CurrentFrequency = 0.0f;
    periodsSinceLastFrequencyChange = 0;
    periodsToChangeFrequency = 2000;
    sin_wave = WaveGen_Create(&SINE);
    cos_wave = WaveGen_Create(&COSINE);
    WaveGen_SetFrequency(sin_wave,CurrentFrequency,SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cos_wave,CurrentFrequency,SWITCHING_FREQUENCY);
    ScalarProfile_Init(230.0f, 0.0f, 50.0f,  0.0f);
    PWM_RegisterCallback(ScalarControl_SetTaskReady);
    ADC_PrepareForControlLoop();
}

void ScalarControl_Task()
{
    if(taskReady == 0)
        return;

    LL_GPIO_SetOutputPin(WCET_GPIO_Port, WCET_Pin);
    Analog_Feedback_t measurements = ADC_GetAnalogFeedback();

    if(CurrentFrequency < Setpoint && periodsSinceLastFrequencyChange >= periodsToChangeFrequency)
    {
        CurrentFrequency += 1.0f;
        WaveGen_SetFrequency(sin_wave,CurrentFrequency,SWITCHING_FREQUENCY);
        WaveGen_SetFrequency(cos_wave,CurrentFrequency,SWITCHING_FREQUENCY);
        periodsSinceLastFrequencyChange = 0;
    }

    float voltageMagnitude = ScalarProfile_GetVoltage(CurrentFrequency);
    VoltageVector_t vector = GetVoltageVector(voltageMagnitude);

    Duty_t dutyCycles = SPWM(vector.alpha,vector.beta,(float)measurements.DcBus/1000.0f);
    PWM_SetDuty(dutyCycles.duty_a,dutyCycles.duty_b,dutyCycles.duty_c);

    periodsSinceLastFrequencyChange++;
    taskReady = 0;
    LL_GPIO_ResetOutputPin(WCET_GPIO_Port, WCET_Pin);
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

void ScalarControl_SetTaskReady()
{
    taskReady = 1;
}



