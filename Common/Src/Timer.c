#include "Timer.h"

static uint8_t isInitialised = 0;
static uint8_t tasksNum = 0;

static void (*task_scheduled)(void);
static uint32_t task_period_ms;

static uint32_t tick = 0;

void Timer_Init()
{
    isInitialised = 1;
    tick = 0;
}

int Timer_Wakeup()
{
    if(isInitialised == 0)
        return TIMER_NO_INIT;

    tick++;

    if(tasksNum == 0)
        return TIMER_NO_TASKS;

    if(tick - task_period_ms == 0)
        task_scheduled();

    return TIMER_SUCCESS;
}

int Timer_Subscribe(void (*Task)(void),uint32_t period_ms)
{
    if(period_ms == 0)
        return TIMER_FAIL;

    task_scheduled = Task;
    task_period_ms = period_ms;

    return TIMER_SUCCESS;
}

void Timer_Destroy()
{
    isInitialised = 0;
}