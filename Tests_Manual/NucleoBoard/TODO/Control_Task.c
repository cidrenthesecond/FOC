#include "Control_Task.h"
#include "ScalarControl.h"
#include "Timer_Driver.h"
#include "qtasks.h"

qTask_t Control_Task;

static void Control_Task_Notify(void);

void Control_TaskInit()
{
    PWM_RegisterCallback(Control_Task_Notify);
    ScalarControl_Init();
}

void Control_Task_Callback(qEvent_t e)
{
    (void)e;
    ScalarControl_Task(); // to replace with function pointer
}

static void Control_Task_Notify(void)
{
    qTask_Notification_Send(&Control_Task, NULL);
}
