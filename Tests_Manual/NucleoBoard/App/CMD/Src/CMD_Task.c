#include "CMD_Task.h"
#include "Cmd_Transport.h"
#include "CommandExecute.h"
#include "CMD_Indicies.h"
#include "CMD_Commands.h"

#include "qtasks.h"
#include "qkernel.h"


static qTask_t CMD_Task;
static void CMD_TaskNotify();

static uint8_t isInitialized = 0;
static uint8_t isActive      = 0;

void CMD_Init()
{
    if(isInitialized)
        return;

    isActive = 0;
    InitCmdTransport();
    Set_OnDataReady_Callback(CMD_TaskNotify);
    CommandExecute_Init();

    CommandExecute_Register(CMD_ESTOP, CMD_ESTOP_Command);
    CommandExecute_Register(ADC_RAW_MEASURE_IDX, ADC_Measure);
    CommandExecute_Register(NTC_GET_TEMP_IDX,    NTC_Measure);
    CommandExecute_Register(PWM_GET_SWITCHING_FREQUENCY, CMD_GetSwitchingFrequency);
    CommandExecute_Register(SYSTEM_TOGGLE_STATE, System_ChangeMainSM);

    qOS_Add_EventTask(&CMD_Task, CMD_TaskCallback, qLowest_Priority, NULL);

    SetUpMonitoring();
    isInitialized = 1;
}

void CMD_TaskCallback(qEvent_t e)
{
    (void)e;

    if(isInitialized && isActive)
        CommandExecute(GetData());
    
    SetUpMonitoring();
}

void CMD_SetActive(bool state)
{
    if(isInitialized)
       isActive = state; 
}

static void CMD_TaskNotify()
{
    qTask_Notification_Send(&CMD_Task, NULL);
}

