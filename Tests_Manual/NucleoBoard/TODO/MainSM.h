#ifndef MAIN_SM_H
#define MAIN_SM_H

#include "qfsm.h"

#define SIG_TOGGLE ((qSM_SigId_t)(1))

qSM_Status_t IDLE_Callback(qSM_Handler_t h);
qSM_Status_t RUN_Callback(qSM_Handler_t h);


#endif