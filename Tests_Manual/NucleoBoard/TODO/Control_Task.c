#include "Control_Task.h"
#include "ScalarControl.h"
#include "Timer_Driver.h"
#include "qtasks.h"

qTask_t Control_Task;

const ControlStrategy_t scalar_strategy = {
    .before_launch = ScalarControl_BeforeLaunch,
    .execute       = ScalarControl_Task
};

static ControlStrategy_t activeStrategy = {0};
static void Control_Task_Notify(void);


void Control_TaskInit()
{
    PWM_RegisterCallback(Control_Task_Notify);

    activeStrategy.before_launch = NULL;
    activeStrategy.execute       = NULL;
}

void Control_Before_Launch()
{
    if(activeStrategy.before_launch != NULL)
        activeStrategy.before_launch();
}

void Control_Task_Callback(qEvent_t e)
{
    (void)e;

    if(activeStrategy.execute != NULL)
        activeStrategy.execute();
}

void Control_Task_SetStrategy(const ControlStrategy_t *strategy)
{
    if(strategy == NULL || strategy->before_launch == NULL || strategy->execute == NULL)
        return;

    activeStrategy = *strategy;
}

static void Control_Task_Notify(void)
{
    qTask_Notification_Send(&Control_Task, NULL);
}
