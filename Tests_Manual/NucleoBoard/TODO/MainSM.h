#ifndef MAIN_SM_H
#define MAIN_SM_H

#include "qfsm.h"
#include "qtasks.h"

#define SIG_TOGGLE ((qSM_SigId_t)(1))

extern qTask_t Main_Task;
extern qSM_t MainSM;
extern qSM_State_t IDLE, RUN;

qSM_Status_t IDLE_Callback(qSM_Handler_t h);
qSM_Status_t RUN_Callback(qSM_Handler_t h);


#endif