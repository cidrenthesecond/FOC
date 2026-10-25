#include "CMD_Task.h"
#include "NTC_Task.h"
#include "Logger.h"
#include "MainSM.h"
#include "Timer_Driver.h"
#include "Control_Task.h"
#include "qkernel.h"
#include <stdbool.h>
#include <string.h>

#include "System_Config.h"
#include "qtasks.h"

#define SIG_TOGGLE ((qSM_SigId_t)(1))

static qTask_t Main_Task;
static qSM_t MainSM;
static qSM_State_t INIT, IDLE, RUN;

static qQueue_t MainSM_Queue;
static qSM_Signal_t MainSM_QueueStorage[4];

qSM_Status_t INIT_Callback(qSM_Handler_t h);
qSM_Status_t IDLE_Callback(qSM_Handler_t h);
qSM_Status_t RUN_Callback(qSM_Handler_t h);

uint8_t firstRun = 1;

void Enter_MainStateMachine()
{
    qStateMachine_Setup(&MainSM, NULL, &INIT, NULL, NULL);

    qStateMachine_StateSubscribe(&MainSM, &IDLE, NULL, IDLE_Callback, NULL, NULL);  
    qStateMachine_StateSubscribe( &MainSM, &RUN, NULL, RUN_Callback, NULL, NULL);
    qStateMachine_StateSubscribe(&MainSM, &INIT, NULL, INIT_Callback, NULL, NULL);

    qQueue_Setup( &MainSM_Queue,MainSM_QueueStorage, sizeof(qSM_Signal_t),4);

    qStateMachine_InstallSignalQueue(&MainSM, &MainSM_Queue);   
    
    /* Task reaguje na zdarzenia asynchroniczne */
    qOS_Add_StateMachineTask(&Main_Task,&MainSM,qHigh_Priority,1,qEnabled,NULL);

    // /* Jednorazowe wejście do INIT */
    // qSM_Signal_t initSignal = {.id = QSM_SIGNAL_NONE,.sigData = NULL};  
    // qStateMachine_Run(&MainSM, initSignal); 
}

qSM_Status_t INIT_Callback(qSM_Handler_t h)
{
    qSM_Status_t retVal = qSM_STATUS_EXIT_SUCCESS;

    SystemInitStatus_t initStatus = System_Init();
    if(initStatus == INIT_SUCCESS)
    {
        h->NextState = &IDLE;        
    }

    return retVal;
}

qSM_Status_t IDLE_Callback(qSM_Handler_t h)
{
    qSM_Status_t retVal = qSM_STATUS_EXIT_SUCCESS;
    switch (h->Signal) 
    {
        case QSM_SIGNAL_ENTRY:
            qTask_Disable(&Main_Task); //now only signals
            NTC_SetActive(true);
            PWM_Stop();
            Control_Task_SetActive(false);
            LOG("Entered IDLE State");
            break;
        case SIG_TOGGLE:
            LOG("MainSM: SIG_TOGGLE received in IDLE");
            h->NextState = &RUN;
            retVal = qSM_STATUS_SIGNAL_HANDLED;
            break;
        default:
            break;
    }

    return retVal;
}

qSM_Status_t RUN_Callback(qSM_Handler_t h)
{
    qSM_Status_t retVal = qSM_STATUS_EXIT_SUCCESS;
    switch (h->Signal)
    {
        case QSM_SIGNAL_ENTRY:
            LOG("Entered RUN State");
            Control_Task_SetActive(true);
            Control_Before_Launch();
            PWM_Start();
            break;
        case SIG_TOGGLE:
            h->NextState = &IDLE;
            retVal = qSM_STATUS_SIGNAL_HANDLED;
            break;
        default:
            break;
    }
    return retVal;
}

void ToggleState()
{
    if(MainSM.qPrivate.current == &INIT)
        return;

    qStateMachine_SendSignal(&MainSM, SIG_TOGGLE, NULL, qTrue);
}