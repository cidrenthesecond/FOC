#include "CMD_Task.h"
#include "Cmd_Transport.h"
#include "CommandExecute.h"
#include "CMD_Indicies.h"
#include "CMD_Commands.h"

void CMD_Init()
{
    CommandExecute_Init();

    CommandExecute_Register(ADC_RAW_MEASURE_IDX, ADC_Measure);
    CommandExecute_Register(NTC_GET_TEMP_IDX,    NTC_Measure);

    SetUpMonitoring();
}

int  CMD_IsTaskReady(void)
{
    return IsDataReady();
}

void CMD_Task(void)
{
    CommandExecute(GetData());
    SetUpMonitoring();
}