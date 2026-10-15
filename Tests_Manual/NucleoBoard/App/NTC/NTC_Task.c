#include "NTC_Task.h"
#include "Thermistor.h"
#include "Logger.h"
#include <stdio.h>

static int16_t maxTemp_mC;
static int16_t minTemp_mC;
static void OnTemperatureOutOfRange(void);
static uint16_t repetitions = 0;

qTask_t NTC_Task;

void NTC_TaskInit(void)
{

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



