#include "CMD_Task.h"
#include "Cmd_Transport.h"
#include "CommandExecute.h"
#include "CMD_Indicies.h"
#include "CMD_Commands.h"

#include "qtasks.h"

qTask_t CMD_Task;
static void CMD_TaskNotify();

void CMD_Init()
{
    InitCmdTransport();
    Set_OnDataReady_Callback(CMD_TaskNotify);
    CommandExecute_Init();
    CommandExecute_Register(CMD_ESTOP, CMD_ESTOP_Command);
    CommandExecute_Register(ADC_RAW_MEASURE_IDX, ADC_Measure);
    CommandExecute_Register(NTC_GET_TEMP_IDX,    NTC_Measure);
    CommandExecute_Register(PWM_GET_SWITCHING_FREQUENCY, CMD_GetSwitchingFrequency);

    SetUpMonitoring();
}

void CMD_TaskCallback(qEvent_t e)
{
    (void)e;

    CommandExecute(GetData());
    SetUpMonitoring();
}

static void CMD_TaskNotify()
{
    qTask_Notification_Send(&CMD_Task, NULL);
}