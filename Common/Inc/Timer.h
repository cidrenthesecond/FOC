#ifndef TIMER_H
#define TIMER_H

#include "stdint.h"

enum {
    TIMER_NO_INIT,
    TIMER_NO_TASKS,
    TIMER_FAIL,
    TIMER_SUCCESS,
};

void Timer_Init();
int Timer_Wakeup();
int Timer_Subscribe(void (*Task)(void),uint32_t period_ms);
void Timer_Destroy();


#endif