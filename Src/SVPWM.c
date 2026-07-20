#include "SVPWM.h"
#include "PWM.h"

#include "arm_math.h"

static SVPWM_InitStruct SVPWM;

void SVPWM_Create(SVPWM_InitStruct set)
{   
    PWM_SetPhaseA(0);
    PWM_SetPhaseB(0);
    PWM_SetPhaseC(0);

    SVPWM.DC_LinkVoltage = set.DC_LinkVoltage;
    SVPWM.timerCountingPeriod = set.timerCountingPeriod;
}

void SVPWM_Proccess(float alpha, float beta)
{
    float a;
    float b;
    float c;

}

uint32_t SVPWM_GetTimerCountingPeriod()
{
    return SVPWM.timerCountingPeriod;
}

int SVPWM_SetTimerCountingPeriod(uint32_t set)
{

}

void SVPWM_Destroy()
{
    PWM_SetPhaseA(0);
    PWM_SetPhaseB(0);
    PWM_SetPhaseC(0);
}