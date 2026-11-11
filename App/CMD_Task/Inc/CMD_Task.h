#ifndef CMD_TASK_H
#define CMD_TASK_H

#include "qtasks.h"
#include "stdbool.h"

void CMD_Init();
void CMD_TaskCallback(qEvent_t e);
void CMD_SetActive(bool state);

#endif