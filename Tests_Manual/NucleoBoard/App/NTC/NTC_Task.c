#include "NTC_Task.h"
#include "Thermistor.h"
#include "Logger.h"

#include <stdio.h>
#include "qkernel.h"
#include "qtasks.h"
#include "qtypes.h"

static int16_t maxTemp_mC;
static int16_t minTemp_mC;
static void OnTemperatureOutOfRange(void);
static uint16_t repetitions = 0;

static qTask_t NTC_Task;
static uint8_t isInitialized = 0;
static uint8_t isActive      = 0;

void NTC_TaskInit(void)
{
    qOS_Add_Task(&NTC_Task, NTC_TaskCallback, qLowest_Priority, NTC_TASK_PERIOD, qIndefinite, qDisabled, NULL);
    isActive      = 0;
    isInitialized = 1;
}

void NTC_TaskCallback(qEvent_t e)
{
    (void)e;

    int16_t HeatSinkTemp_mC = Thermistor_GetHeatsinkTemp();

    if(HeatSinkTemp_mC < minTemp_mC || HeatSinkTemp_mC > maxTemp_mC)
        ;
        //Throw Error

    repetitions++;

    if(repetitions >= 60)
    {
        char log[60];
        sprintf(log, "[NTC] : HT Temp %d mC",HeatSinkTemp_mC);
        LOG(log);
        
        repetitions = 0;
    }
}

void NTC_SetActive(bool set)
{
    if(!isInitialized)
        return;

    if(set){
        qTask_Enable(&NTC_Task);
        isActive = 1;
    }
    else {
        qTask_Disable(&NTC_Task);
        isActive = 0;
    }
}



