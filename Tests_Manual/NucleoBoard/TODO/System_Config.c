#include "System_Config.h"

#include"Logger.h"
#include"Log_Transport.h"
#include"CMD_Task.h"
#include"Timer_Driver.h"
#include"ADC_Service.h"
#include"FPU.h"
#include "Relay.h"
#include "EmergencyHandler.h"

typedef enum {
    INIT_FPU,
    INIT_PWM,
    INIT_CMD,
    INIT_RELAY,

} SystemInitState_t;

void System_Init()
{   
    LogTransport_Init();
    LOG_Init(Send_USART_DMA_LL, 10);
    LOG("[LOG] : Init");

    FPU_enable();
    LOG("[FPU] : Init");

    EmergencyHandler_Init();
    LOG("Emergency handler init");

    PWM_Init();
    LOG("[PWM] : Init");

    CMD_Init();
    LOG("[CMD] : Init");

    if(ADC_Init() == ADC_FAIL)
        LOG("ADC Init Failed");
    
    LOG("[ADC] : Init");

    Relay_Init();
    LOG("[RELAY] : Init");
    Relay_SetThreshold(8000);
    
    while(!Relay_IsOn())
        Relay_SM();

}