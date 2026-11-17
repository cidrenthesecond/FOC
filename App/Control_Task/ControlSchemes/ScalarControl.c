#include "ScalarControl.h"
#include "ADC_Service.h"
#include "ScalarProfile.h"
#include "WaveGen.h"
#include "WaveGen_LUTs.h"
#include "PWM_Schemes.h"
#include "PWM_Driver.h"
#include "qstimers.h"
#include "Logger.h"
#include "stdbool.h"
#include "SystemParameters.h"
#include "ControlUtility.h"

#define SWITCHING_FREQUENCY 1000.0f
#define FREQUENCY_TOLERANCE 0.001f

typedef struct {
    float target_rpm; //niekoniecznie rpm  to silnik asynchroniczny!!!
    float ramp_rpm_s; 
} MotionProfile_t;

#define DEFAULT_BENCH_PROFILE { \
    .target_rpm = 300,          \
    .ramp_rpm_s = 30            \
}                               \

static MotionProfile_t motionProfile = DEFAULT_BENCH_PROFILE;

static WaveGen sin_wave;
static WaveGen cos_wave;
static float CurrentFrequency;
static uint32_t periodsSinceLastFrequencyChange;

static float FrequencyStep;                                  // ramp_rpm_s*poles*periodstochangeFrueqncy/mainsFrequency/switchingFrequency
static uint32_t periodsToChangeFrequency;         // equal to 1 next update now stays as is because its easier to understand
static float setpointFrequency = 5.0f;            // target_rpm/mainsFrequency //ale slip jeszcze trzeba pamietac i poles silnika

static float switchingFrequency_Cached;

static bool IsSetPointReached();
static float Absolute_F(float x);

static uint8_t TimeoutAnalogFeedback();
static void    CalculateMotionProfileParams(MotionProfile_t profile,Drive_Parameters_t driveParams, Motor_params_t motorParams);

void ScalarControl_Init()
{
    CurrentFrequency = 0.0f;
    FrequencyStep = 1.0f;
    periodsSinceLastFrequencyChange = 0;
    periodsToChangeFrequency = 2000;
    sin_wave = WaveGen_Create(&SINE);
    cos_wave = WaveGen_Create(&COSINE);
    ScalarProfile_Init(230.0f, 0.0f, 50.0f,  0.0f);
    ADC_PrepareForControlLoop();
}

void ScalarControl_BeforeLaunch()
{
    CurrentFrequency = 0.0f;
    periodsSinceLastFrequencyChange = 0;
    //periodsToChangeFrequency = 2000;
    CalculateMotionProfileParams(motionProfile, GetDriveParameters(), GetMotorParameters());
    WaveGen_Reset(sin_wave);
    WaveGen_Reset(cos_wave);
    WaveGen_SetFrequency(sin_wave, CurrentFrequency, SWITCHING_FREQUENCY);
    WaveGen_SetFrequency(cos_wave, CurrentFrequency, SWITCHING_FREQUENCY);
    ADC_PrepareForControlLoop();
}

void ScalarControl_Task()
{   
    float error = setpointFrequency - CurrentFrequency;

    if(!IsSetPointReached() && periodsSinceLastFrequencyChange >= periodsToChangeFrequency)
    {
        if(error > FrequencyStep)
            CurrentFrequency += FrequencyStep;
        else if (error < -FrequencyStep)
            CurrentFrequency -= FrequencyStep;
        else
            CurrentFrequency = setpointFrequency;

        WaveGen_SetFrequency(sin_wave,Absolute_F(CurrentFrequency),SWITCHING_FREQUENCY);
        WaveGen_SetFrequency(cos_wave,Absolute_F(CurrentFrequency),SWITCHING_FREQUENCY);
        periodsSinceLastFrequencyChange = 0;
    }

    float voltageMagnitude = ScalarProfile_GetVoltage(Absolute_F(CurrentFrequency)); //In need of phase RMS to line Amplitude
    VoltageVector_t vector = GetVoltageVector(WaveGen_Get(sin_wave),WaveGen_Get(cos_wave),voltageMagnitude);

    Analog_Feedback_t measurements;

    if(TimeoutAnalogFeedback())
        measurements = ADC_GetAnalogFeedback();
    else
    {
        LOG("ADC : Err : NoData");
    }

    Duty_t dutyCycles = SPWM(vector.alpha,vector.beta,(float)measurements.DcBus/1000.0f);

    if(CurrentFrequency < 0)
        dutyCycles = SwapPhases(dutyCycles);

    PWM_SetDuty(dutyCycles.duty_a,dutyCycles.duty_b,dutyCycles.duty_c);

    periodsSinceLastFrequencyChange++;
}

static uint8_t TimeoutAnalogFeedback()
{
    static qSTimer_t ADC_Timeout = QSTIMER_INITIALIZER;
    qSTimer_Set(&ADC_Timeout, 1);
    uint8_t isFeedbackReady;

    do{
        isFeedbackReady = ADC_IsFeedbackReady();
    } while (!qSTimer_Expired(&ADC_Timeout) && !isFeedbackReady);

    return isFeedbackReady;
}

void ScalarControl_SetSetpoint(float target)
{
    setpointFrequency = target;
}

static bool IsSetPointReached()
{
    return Absolute_F(setpointFrequency - CurrentFrequency) <= FREQUENCY_TOLERANCE;
}

static float Absolute_F(float x)
{
    if(x >= 0.0f) return x;
    else          return -x;    
}

static void CalculateMotionProfileParams(MotionProfile_t profile,Drive_Parameters_t driveParams, Motor_params_t motorParams)
{
    (void)motorParams; // workaround for now we need to include poles
    periodsToChangeFrequency = 1;
    setpointFrequency = profile.target_rpm/(float)driveParams.mainsFrequency; //5.0f; // target_rpm/mainsFrequency //ale slip jeszcze trzeba pamietac i poles silnika
    FrequencyStep     = profile.ramp_rpm_s*periodsToChangeFrequency/(float)driveParams.mainsFrequency/(float)driveParams.switchingFrequency; // dodac poles silnika
    // ramp_rpm_s*poles*periodstochangeFrueqncy/mainsFrequency/switchingFrequency
}



