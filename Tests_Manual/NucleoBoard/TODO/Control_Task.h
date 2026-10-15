#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

#include "qtasks.h"

extern qTask_t Control_Task;
//extern void(*Control_Task_Callback)(qEvent_t e);

void Control_TaskInit();
void Control_Task_Callback(qEvent_t e);
void Control_Task_SetCallback();



#endif