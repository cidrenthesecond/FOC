#include "Logger.h"
#include "MainSM.h"
#include "Timer_Driver.h"
#include "Control_Task.h"
#include "qfsm.h"
#include "qtasks.h"

qTask_t Main_Task;
qSM_t MainSM;
qSM_State_t INIT, IDLE, RUN;

uint8_t firstRun = 1;

qSM_Status_t INIT_Callback(qSM_Handler_t h)
{
    qSM_Status_t retVal = qSM_STATUS_EXIT_SUCCESS;
    switch(h->Signal)
    {
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
            PWM_Stop();
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