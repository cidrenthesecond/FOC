#include "Timer_Driver.h"
#include "ADC_Service.h"

void OnEmergency(void);

void EmergencyHandler_Init(void)
{
    ADC_Set_OCP_ISR(OnEmergency);
}

void OnEmergency(void)
{
    PWM_ESTOP();
}