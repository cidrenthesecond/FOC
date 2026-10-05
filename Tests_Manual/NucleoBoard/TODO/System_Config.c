#include "System_Config.h"

#include"Logger.h"
#include"Log_Transport.h"
#include"CMD_Task.h"
#include"Timer_Driver.h"
#include"ADC_Service.h"
#include"FPU.h"
#include "Relay.h"

void System_Init()
{   
    LogTransport_Init();
    LOG_Init(Send_USART_DMA_LL, 10);
    LOG("[LOG] : Init");

    FPU_enable();
    LOG("[FPU] : Init");

    ADC_Init();
    LOG("[ADC] : Init");

    PWM_Init();
    LOG("[PWM] : Init");

    CMD_Init();
    LOG("[CMD] : Init");

    Relay_Init();
    LOG("[RELAY] : Init");
    Relay_SetThreshold(8000);
    
    while(!Relay_IsOn())
        Relay_SM();

}