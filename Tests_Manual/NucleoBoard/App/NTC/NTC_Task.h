#ifndef NTC_TASK_H
#define NTC_TASK_H

#include "stdint.h"
#include "qtasks.h"

extern qTask_t NTC_Task;

#define NTC_TASK_PERIOD 1000u

void    NTC_TaskInit(void);
void    NTC_TaskCallback(qEvent_t e);

#endif