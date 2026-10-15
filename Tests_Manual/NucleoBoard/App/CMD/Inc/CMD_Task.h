#ifndef CMD_TASK_H
#define CMD_TASK_H

#include "qtasks.h"

extern qTask_t CMD_Task;

void CMD_Init();
void CMD_TaskCallback(qEvent_t e);



#endif