#include "Control_Task.h"
#include "ScalarControl.h"
#include "Timer_Driver.h"
#include "qtasks.h"
#include "qkernel.h"


const ControlStrategy_t scalar_strategy = {
    .before_launch = ScalarControl_BeforeLaunch,
    .execute       = ScalarControl_Task
};

static ControlStrategy_t activeStrategy = {0};
static void Control_Task_Notify(void);

static uint8_t isActive      = 0;
static uint8_t isInitialized = 0;
static uint8_t isStrategySet = 0;
static qTask_t Control_Task;

void Control_TaskInit()
{
    PWM_RegisterCallback(Control_Task_Notify);
    qOS_Add_EventTask(&Control_Task, Control_Task_Callback, qHigh_Priority, NULL);
    activeStrategy.before_launch = NULL;
    activeStrategy.execute       = NULL;
    isInitialized = 1;
    isActive      = 0;
    isStrategySet = 0;
}

void Control_Before_Launch()
{
    if(!isInitialized || !isActive || !isStrategySet)
        return;
    
    if(activeStrategy.before_launch != NULL)
        activeStrategy.before_launch();
}

void Control_Task_Callback(qEvent_t e)
{
    (void)e;

    if(!isInitialized || !isActive || !isStrategySet)
        return;

    if(activeStrategy.execute != NULL)
        activeStrategy.execute();
}

void Control_Task_SetStrategy(const ControlStrategy_t *strategy)
{
    if(strategy == NULL || strategy->before_launch == NULL || strategy->execute == NULL)
        return;

    activeStrategy = *strategy;
    isStrategySet  = 1;
}

void Control_Task_SetActive(bool set)
{
    if(!isInitialized)
        return;

    if(set) isActive = 1;
    else    isActive = 0;
}

static void Control_Task_Notify(void)
{
    qTask_Notification_Send(&Control_Task, NULL);
}
