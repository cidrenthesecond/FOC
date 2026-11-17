#include "ScalarControlSensored.h"
#include "PID.h"
#include "ScalarProfile.h"
#include "WaveGen.h"
#include "WaveGen_LUTs.h"
#include "stdint.h"
#include "PWM_Schemes.h"
#include "PWM_Driver.h"
#include "ADC_Service.h"
#include "SpeedFeedback.h"

#define SWITCHING_FREQUENCY 1000

typedef struct {
    float alpha;
    float beta;
} VoltageVector_t;

typedef struct {
    float Ki;
    float Kp;
    float Ts;
} SpeedRegulator_Params_t;

static int32_t MotorSpeedSetpoint = 3600;//rpm


static PID*                                   speedRegulator;
static SpeedRegulator_Params_t speedRegulator_Params;
// filtr predkosci

static WaveGen sinWave;
static WaveGen cosWave;

static Speed_Feedback_Strategy_t speedMeasurementStrategy;

static VoltageVector_t GetVoltageVector(float magnitude);
static Duty_t          SwapPhases(Duty_t duty);
static float             Absolute_F(float x);

void ScalarControlSensored_Init()
{
    sinWave = WaveGen_Create(&SINE);
    cosWave = WaveGen_Create(&COSINE);
    WaveGen_SetFrequency(sinWave, 0.0f, SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cosWave, 0.0f, SWITCHING_FREQUENCY);
    ScalarProfile_Init(230.0f, 0.0f, 50.0f,  0.0f);

    PID_Init(speedRegulator);
    PID_SetKp(speedRegulator, speedRegulator_Params.Kp);
    PID_SetKi(speedRegulator, speedRegulator_Params.Ki);
    //PID_SetTs
    //PID_SetSaturation(PID *this, float _saturation)

    //speedMeasurementStrategy = cos
    ADC_PrepareForControlLoop();
}

void ScalarControlSensored_BeforeLaunch()
{   
    PID_Reset(speedRegulator);
    speedMeasurementStrategy.begin();
    //PID_SetTs
    WaveGen_Reset(sinWave);
    WaveGen_Reset(cosWave);
    ADC_PrepareForControlLoop();
}

void ScalarControlSensored_Task()
{
    Speed_Feedback_t feedback = speedMeasurementStrategy.getFeedback();

    int32_t error = feedback.velocity - MotorSpeedSetpoint;
    float slip    = PID_Compute(speedRegulator, (float)error);

    float command          = slip + (float)MotorSpeedSetpoint;
    float frequencySet     = 50.0f/command;
    float voltageMagnitude = ScalarProfile_GetVoltage(Absolute_F(frequencySet));//convert rpm to hz // does pole number have difference here
    WaveGen_SetFrequency(sinWave, Absolute_F(frequencySet), SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cosWave, Absolute_F(frequencySet), SWITCHING_FREQUENCY);

    VoltageVector_t vector = GetVoltageVector(voltageMagnitude);
    Analog_Feedback_t measurements = ADC_GetAnalogFeedback();

    Duty_t duty = SVPWM(vector.alpha, vector.beta, (float)measurements.DcBus/1000.0f);

    if(frequencySet < 0)
        duty = SwapPhases(duty);

    PWM_SetDuty(duty.duty_a, duty.duty_b, duty.duty_c);
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

static Duty_t SwapPhases (Duty_t duty)
{
    float helper = duty.duty_a;

    duty.duty_a = duty.duty_b;
    duty.duty_b = helper;

    return duty;
}

static float Absolute_F(float x)
{
    if(x >= 0.0f)
        return x;
    else
        return -x;
}