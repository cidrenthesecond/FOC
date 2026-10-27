#include "System_Config.h"

#include"Logger.h"
#include"Log_Transport.h"
#include"CMD_Task.h"
#include"Timer_Driver.h"
#include"ADC_Service.h"
#include"FPU.h"
#include "Relay.h"
#include "EmergencyHandler.h"
#include "NTC_Task.h"
#include "ScalarControl.h"
#include "Control_Task.h"

typedef enum {
    INIT_LOGGER,
    INIT_FPU,
    INIT_PWM,
    INIT_CMD,
    INIT_NTC,
    INIT_EMERGENCY,
    INIT_ADC,
    INIT_RELAY,
    INIT_SCALAR_CONTROL,
    INIT_CONTROL_TASK,
    INIT_FINISHED,
    INIT_ERROR
} SystemInitState_t;


static SystemInitState_t currentState    = 0;

static SystemInitStatus_t ProceedInitialisation();


SystemInitStatus_t System_Init()
{   
    SystemInitStatus_t result = INIT_PENDING;

    switch(currentState)
    {
        case INIT_LOGGER:
            LogTransport_Init();
            LOG_Init(Send_USART_DMA_LL, 10);
            LOG("[LOG] : Init");
            result = ProceedInitialisation();
            break;
        
        case INIT_FPU:
            FPU_enable();
            LOG("[FPU] : Init");
            result = ProceedInitialisation();
            break;

        case INIT_PWM:
            PWM_Init();
            LOG("[PWM] : Init");
            result = ProceedInitialisation();
            break;

        case INIT_CMD:
            CMD_Init();
            CMD_SetActive(true);
            LOG("[CMD] : Init");
            result = ProceedInitialisation();
            break;

        case INIT_NTC:
            NTC_TaskInit();
            LOG("[NTC] : Init");
            result = ProceedInitialisation();
            break;

        case INIT_SCALAR_CONTROL:
            ScalarControl_Init();
            LOG("Scalar Control init");
            result = ProceedInitialisation();
            break;

        case INIT_CONTROL_TASK:
            Control_TaskInit();
            Control_Task_SetStrategy(&scalar_strategy);
            result = ProceedInitialisation();
            break;

        case INIT_EMERGENCY:
            EmergencyHandler_Init();
            LOG("Emergency handler init");
            result = ProceedInitialisation();
            break;

        case INIT_ADC:
            if(ADC_Init() == ADC_FAIL)
            {
                LOG("ADC Init Failed");
                result = INIT_FAIL;
                break;
            }
                
            LOG("[ADC] : Init");
            result = ProceedInitialisation();
            break;

        case INIT_RELAY:
            Relay_Init();
            LOG("[RELAY] : Init");
            Relay_SetThreshold(8000);
    
            while(!Relay_IsOn())
                Relay_SM();

            result = ProceedInitialisation();
            break;
        default:
            LOG("ERROR : init entered case out of bounds");
            break;
    }

    return result;
}

static SystemInitStatus_t ProceedInitialisation()
{
    SystemInitStatus_t result = INIT_PENDING;
    currentState += 1;

    if(currentState == INIT_FINISHED)
        result = INIT_SUCCESS;

    return result;
}