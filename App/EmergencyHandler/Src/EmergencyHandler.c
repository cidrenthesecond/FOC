#include "PWM_Driver.h"
#include "ADC_Service.h"
#include "NTC_Task.h"
#include "MainSM.h"

void OnEmergency(void);

void EmergencyHandler_Init(void)
{
    ADC_Set_OCP_ISR(OnEmergency);
    NTC_SetOnTempOutOfRangeCallback(OnEmergency);
}

void OnEmergency(void)
{
    PWM_ESTOP();
    Main_RequestERROR();
}