#ifndef NTC_TASK_H
#define NTC_TASK_H

#include "stdint.h"
#include "stdbool.h"

#include "qtasks.h"

#define NTC_TASK_PERIOD 1000u

void NTC_TaskInit(void);
void NTC_TaskCallback(qEvent_t e);
void NTC_SetActive(bool set);

#endif