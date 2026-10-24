#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

#include "qtasks.h"

extern qTask_t Control_Task;

typedef struct{
    void(*before_launch)(void);
    void(*execute)(void);
} ControlStrategy_t;

extern const ControlStrategy_t scalar_strategy;

void Control_TaskInit();
void Control_Before_Launch();
void Control_Task_Callback(qEvent_t e);
void Control_Task_SetStrategy(const ControlStrategy_t *strategy);



#endif