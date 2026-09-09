#include "SysTickDispatcher.h"

static uint8_t isInitialised = 0;
static uint8_t tasksNum = 0;

typedef struct{
    void (*taskToDispatch)(void);
    uint32_t period_ms;
    volatile uint32_t lastTimeCalled_TimeStamp;
} Task;

static Task tasksList[SYS_TICK_DISPATCHER_MAX_TASKS];

static volatile uint32_t tick = 0;

void SysTickDispatcher_Init()
{
    isInitialised = 1;
    tick = 0;

    tasksNum = 0;

    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
    {
        tasksList[index].taskToDispatch = NULL;
        tasksList[index].period_ms = 0;
        tasksList[index].lastTimeCalled_TimeStamp = 0;
    }
}

int SysTickDispatcher_Wakeup()
{
    if(isInitialised == 0)
        return SYSTICKDISPATCHER_NO_INIT;

    tick++;

    if(tasksNum == 0)
        return SYSTICKDISPATCHER_NO_TASKS;

    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
    {
        if(tasksList[index].taskToDispatch == NULL)
            continue;

        if(tick - tasksList[index].lastTimeCalled_TimeStamp == tasksList[index].period_ms)
        {
            tasksList[index].taskToDispatch();
            tasksList[index].lastTimeCalled_TimeStamp = tick;
        }
            
    }

    return SYSTICKDISPATCHER_SUCCESS;
}

int SysTickDispatcher_Subscribe(void (*Task)(void),uint32_t period_ms)
{
    if(period_ms == 0)
        return SYSTICKDISPATCHER_FAIL;

    tasksNum++;

    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
    {
        if(tasksList[index].taskToDispatch == NULL)
        {
            tasksList[index].taskToDispatch = Task;
            tasksList[index].period_ms = period_ms;
            tasksList[index].lastTimeCalled_TimeStamp = tick;
            return SYSTICKDISPATCHER_SUCCESS;
        }
    }

    return SYSTICKDISPATCHER_SUCCESS;
}

int SysTickDispatcher_UnSubscribe(void (*Task)(void),uint32_t period_ms)
{
    if(isInitialised == 0)
        return SYSTICKDISPATCHER_NO_INIT;

    if(tasksNum == 0)
        return SYSTICKDISPATCHER_NO_TASKS;

    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
    {
        if(tasksList[index].taskToDispatch == Task && tasksList[index].period_ms == period_ms)
        {
            tasksList[index].taskToDispatch = NULL;
            tasksList[index].period_ms = 0;
            tasksList[index].lastTimeCalled_TimeStamp = 0;
            return SYSTICKDISPATCHER_SUCCESS;
        }
    }

    return SYSTICKDISPATCHER_FAIL;
}

void SysTickDispatcher_Destroy()
{
    isInitialised = 0;
}