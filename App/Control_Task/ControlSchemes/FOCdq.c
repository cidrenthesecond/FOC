#include "FOCdq.h"
#include "arm_math.h"
#include "PID.h"
#include "SpeedFeedback.h"
#include "qstimers.h"
#include "ADC_Service.h"
#include "PWM_Schemes.h"
#include "PWM_Driver.h"

typedef struct {
    float d;
    float q;
} Currents_Setpoint_t;

static Currents_Setpoint_t currentSetpoint;

static PID current_d_regulator;
static PID current_q_regulator;
//PIDs parameters

static Speed_Feedback_Strategy_t speedMeasurementStrategy;


void FOCdq_Init()
{

}

void FOCdq_BeforeLaunch()
{

}

void FOCdq_Task()
{
    Speed_Feedback_t speedFeedback = speedMeasurementStrategy.getFeedback();

    qSTimer_t adcTimeout = QSTIMER_INITIALIZER;
    qSTimer_Set(&adcTimeout, 1);
    uint32_t ADC_isDataReady = 0;

    do{
        ADC_isDataReady = ADC_IsFeedbackReady();
    } while(!qSTimer_Expired(&adcTimeout) && !ADC_isDataReady);

    if(!ADC_isDataReady)
        ;//throw expection

    Analog_Feedback_t analogFeedback = ADC_GetAnalogFeedback();

    float currentAlpha, currentBeta;
    arm_clarke_f32(analogFeedback.Currents.Current_A,analogFeedback.Currents.Current_B,&currentAlpha,&currentBeta);
    float measuredCurrent_d,measuredCurrent_q;
    float sin_of_angle,cos_of_angle;
    //get sin/cos
   // arm_park_f32(currentAlpha,currentBeta,&measuredCurrent_d,&measuredCurrent_q,speedFeedback.angle)

    float currentSetpoint_d = currentSetpoint.d - measuredCurrent_d;
    float currentSetpoint_q = currentSetpoint_q - measuredCurrent_q;

    float currentCommand_d = PID_Compute(&current_d_regulator,currentSetpoint_d);
    float currentCommand_q = PID_Compute(&current_q_regulator,currentSetpoint_q);

    float currentCommandAlpha, currentCommandBeta;
    //use of angles
    //arm_inv_park_f32(current_d_regulator,current_q_regulator,&currentCommandAlpha,&currentCommandBeta,)

    Duty_t duty = SPWM(currentCommandAlpha,currentCommandBeta,analogFeedback.DcBus);

    //if qsetpoint < 0 swap phases??

    PWM_SetDuty(duty.duty_a,duty.duty_b,duty.duty_c);
}

