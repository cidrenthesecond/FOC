#include "Logger.h"
#include "MainSM.h"

qTask_t Main_Task;
qSM_t MainSM;
qSM_State_t IDLE, RUN;

qSM_Status_t IDLE_Callback(qSM_Handler_t h)
{
    switch (h->Signal) 
    {
        case QSM_SIGNAL_ENTRY:
            LOG("Entered IDLE State");
            break;
        case SIG_TOGGLE:
            h->NextState = &RUN;
            break;
        default:
            break;
    }
    return qSM_STATUS_EXIT_SUCCESS;
}

qSM_Status_t RUN_Callback(qSM_Handler_t h)
{
    switch (h->Signal)
    {
        case QSM_SIGNAL_ENTRY:
            LOG("Entered RUN State");
            break;
        case SIG_TOGGLE:
            h->NextState = &IDLE;
            break;
        default:
            break;
    }
    return qSM_STATUS_EXIT_SUCCESS;
}