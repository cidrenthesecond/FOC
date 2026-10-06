#include "Thermistor.h"
#include "Logger.h"
#include <stdio.h>

static volatile uint8_t taskReady = 0;

static int16_t maxTemp_mC;
static int16_t minTemp_mC;

uint8_t NTC_IsTaskReady(void)
{
    return taskReady;
}

void NTC_Task(void)
{
    int16_t HeatSinkTemp_mC = Thermistor_GetHeatsinkTemp();

    if(HeatSinkTemp_mC < minTemp_mC || HeatSinkTemp_mC > maxTemp_mC)
        ;
        //Throw Error

    char log[60];
    sprintf(log, "[NTC] : HT Temp %d mC",HeatSinkTemp_mC);
    LOG(log);
}

void NTC_SetTaskReady(void)
{
    taskReady = 1;
}


