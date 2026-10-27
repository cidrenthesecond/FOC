#include "NTC_Task.h"
#include "Logger.h"
#include "MainSM.h"
#include "Timer_Driver.h"
#include "Control_Task.h"
#include "qfsm.h"
#include "qkernel.h"
#include <stdbool.h>
#include <stdio.h>
#include "System_Config.h"
#include "qtasks.h"

#define NEXT_STATE_SCHEDULED ((qSM_SigId_t)(1))

typedef struct {
    uint8_t systemInitalised : 1;
    uint8_t miau             : 1;
    uint8_t reserved         : 6;
} MainSM_Status_t;

static MainSM_Status_t mainSM_Status;

static qTask_t Main_Task;
static qSM_t MainSM;
static qSM_State_t INIT, IDLE, RUN, ERROR;
static qSM_State_t *CurrentState;
static qSM_State_t *ScheduledState;

qSM_Status_t INIT_Callback(qSM_Handler_t h);
qSM_Status_t IDLE_Callback(qSM_Handler_t h);
qSM_Status_t RUN_Callback(qSM_Handler_t h);
qSM_Status_t ERROR_Callback(qSM_Handler_t h);

uint8_t firstRun = 1;

static void RequestNextState(qSM_State_t *next);
static void ProccessScheduledState(qSM_Handler_t h);

void Enter_MainStateMachine()
{
    qStateMachine_Setup(&MainSM, NULL, &INIT, NULL, NULL);

    qStateMachine_StateSubscribe(&MainSM, &IDLE, NULL, IDLE_Callback, NULL, NULL);  
    qStateMachine_StateSubscribe( &MainSM, &RUN, NULL, RUN_Callback, NULL, NULL);
    qStateMachine_StateSubscribe(&MainSM, &INIT, NULL, INIT_Callback, NULL, NULL);
    qStateMachine_StateSubscribe(&MainSM, &ERROR, NULL, ERROR_Callback, NULL, NULL);

    /* Task reaguje na zdarzenia asynchroniczne */
    qOS_Add_StateMachineTask(&Main_Task,&MainSM,qHigh_Priority,1,qEnabled,NULL);

    // /* Jednorazowe wejście do INIT */
    // qSM_Signal_t initSignal = {.id = QSM_SIGNAL_NONE,.sigData = NULL};  
    // qStateMachine_Run(&MainSM, initSignal);
    CurrentState = &INIT;
}

qSM_Status_t INIT_Callback(qSM_Handler_t h)
{
    qSM_Status_t retVal = qSM_STATUS_EXIT_SUCCESS;
    SystemInitStatus_t initStatus;
    
    switch (h-> Signal)
    {
        case QSM_SIGNAL_ENTRY:
            LOG("Entered INIT state");
            break;
        case NEXT_STATE_SCHEDULED:
            ProccessScheduledState(h);
            break;
        case QSM_SIGNAL_NONE:
            initStatus = System_Init();

            if(initStatus == INIT_SUCCESS)
            {
                mainSM_Status.systemInitalised = 1;
                RequestNextState(&IDLE);
            }
            else if(initStatus == INIT_FAIL)
            {
                RequestNextState(&ERROR);
            }

            retVal = qSM_STATUS_SIGNAL_HANDLED;
        default:
            break;  
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
        case NEXT_STATE_SCHEDULED:
            LOG("MainSM: SIG_TOGGLE received in IDLE");
            ProccessScheduledState(h);
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
        case NEXT_STATE_SCHEDULED:
            ProccessScheduledState(h);
            retVal = qSM_STATUS_SIGNAL_HANDLED;
            break;
        default:
            break;
    }
    return retVal;
}

qSM_Status_t ERROR_Callback(qSM_Handler_t h)
{
    qSM_Status_t retVal = qSM_STATUS_EXIT_SUCCESS;
    switch (h->Signal)
    {
        case QSM_SIGNAL_ENTRY:
            LOG("Entered ERROR");
            break;
        case NEXT_STATE_SCHEDULED:
            ProccessScheduledState(h);
            retVal = qSM_STATUS_SIGNAL_HANDLED;
            break;
        default:
            break;
    }
    return retVal;
}

void ToggleState()
{
    if(CurrentState == &ERROR || mainSM_Status.systemInitalised == 0)
        return; 

    if(CurrentState == &IDLE)
        RequestNextState(&RUN);
    else if(CurrentState == &RUN)
        RequestNextState(&IDLE);
}

void Main_RequestErrorReset()
{
    if(CurrentState != &ERROR)
        return;

    if(mainSM_Status.systemInitalised)
        RequestNextState(&IDLE);
    else
        RequestNextState(&INIT);
}


static void RequestNextState(qSM_State_t *next)
{
    if(ScheduledState == &ERROR)
        return;

    ScheduledState = next;
    qStateMachine_SendSignal(&MainSM, NEXT_STATE_SCHEDULED, NULL, qTrue);
}

static void ProccessScheduledState(qSM_Handler_t h)
{
    h -> NextState = ScheduledState;
    CurrentState   = ScheduledState;
    ScheduledState = NULL;
}