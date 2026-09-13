#ifndef SYS_TICK_DISPATCHER_H
#define SYS_TICK_DISPATCHER_H

#include "stdint.h"

enum {
    SYSTICKDISPATCHER_NO_INIT,
    SYSTICKDISPATCHER_NO_TASKS,
    SYSTICKDISPATCHER_FAIL,
    SYSTICKDISPATCHER_SUCCESS,
};

#define SYS_TICK_DISPATCHER_MAX_TASKS 10

void SysTickDispatcher_Init();
int SysTickDispatcher_Wakeup();
int SysTickDispatcher_Subscribe(void (*Task)(void),uint32_t period_ms);
int SysTickDispatcher_UnSubscribe(void (*Task)(void),uint32_t period_ms);
void SysTickDispatcher_Destroy();


#endif