#include "ScalarControlSensored.h"
#include "PID.h"
#include "ScalarControl.h"
#include "ScalarProfile.h"
#include "WaveGen.h"
#include "WaveGen_LUTs.h"
#include "stdint.h"
#include "PWM_Schemes.h"
#include "Timer_Driver.h"
#include "ADC_Service.h"

#define SWITCHING_FREQUENCY 1000

static volatile uint8_t taskReady;

static int32_t MotorSpeedSetpoint = 3600;//rpm
static PID*    Regulator;
static WaveGen sinWave;
static WaveGen cosWave;

typedef struct {
    float alpha;
    float beta;
} VoltageVector_t;

static VoltageVector_t GetVoltageVector(float magnitude);

void ScalarControlSensored_Init()
{
    sinWave = WaveGen_Create(&SINE);
    cosWave = WaveGen_Create(&COSINE);
    WaveGen_SetFrequency(sinWave, 0.0f, SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cosWave, 0.0f, SWITCHING_FREQUENCY);
    ScalarProfile_Init(230.0f, 0.0f, 50.0f,  0.0f);
    PWM_RegisterCallback(ScalarControl_SetTaskReady);
    ADC_PrepareForControlLoop();
}

void ScalarControlSensored_Task()
{
    if(taskReady == 0)
        return;

    int32_t ActualMotorSpeed; // getMotorspeed method
                              // observer
                              // encoder
    int32_t error = ActualMotorSpeed - MotorSpeedSetpoint;
    float slip    = PID_Compute(Regulator, (float)error);

    float command          = slip + (float)MotorSpeedSetpoint;
    float voltageMagnitude = ScalarProfile_GetVoltage(60.0f/command);//convert rpm to hz
    VoltageVector_t vector = GetVoltageVector(voltageMagnitude);

    Analog_Feedback_t measurements = ADC_GetAnalogFeedback();

    Duty_t duty = SVPWM(vector.alpha, vector.beta, (float)measurements.DcBus/1000.0f);
    PWM_SetDuty(duty.duty_a, duty.duty_b, duty.duty_c);

    taskReady = 0;
}

static VoltageVector_t GetVoltageVector(float magnitude)
{
    int32_t sinValue = WaveGen_Get(sinWave);
    int32_t cosValue = WaveGen_Get(cosWave);

    float sinNorm = (float)sinValue / (float)INT32_MAX;
    float cosNorm = (float)cosValue / (float)INT32_MAX;

    VoltageVector_t result;
    result.alpha = cosNorm * magnitude;
    result.beta  = sinNorm * magnitude;

    return result;
}

void ScalarProfileSensored_SetTaskReady()
{
    taskReady = 1;
}