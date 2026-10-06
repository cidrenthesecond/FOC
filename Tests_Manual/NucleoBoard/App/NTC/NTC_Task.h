#ifndef NTC_TASK_H
#define NTC_TASK_H

#include "stdint.h"

uint8_t NTC_IsTaskReady(void);
void    NTC_Task(void);
void    NTC_SetTaskReady(void);

#endif