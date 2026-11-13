#include "RUN_LED_Task.h"
#include "qkernel.h"
#include "qtasks.h"
#include "GPIO_Driver.h"
#include "stdint.h"

typedef struct {
    uint8_t isInitialised : 1;
    uint8_t isActive      : 1;
    uint8_t LedImage      : 1;
    uint8_t Reserved      : 5; //period?
} Status_t;

static Status_t status;
static qTask_t  RUN_LED_Task;
static void RUN_LED_TaskCallback(qEvent_t e);

void RUN_LED_Task_Init()
{
    status.isInitialised = 1;
    status.isActive      = 0;
    status.LedImage      = 0;
    GPIO_RUN_LED_TurnOff();
    qOS_Add_Task(&RUN_LED_Task, RUN_LED_TaskCallback, qLowest_Priority, 1000, qIndefinite, qDisabled, NULL);
}

void RUN_LED_Task_SetActive(bool set)
{
    if(set)
    {
        qTask_Enable(&RUN_LED_Task);
        status.isActive = 1;
    }
    else
    {
        qTask_Disable(&RUN_LED_Task);
        status.isActive = 0;
        GPIO_RUN_LED_TurnOff();
    }
}

static void RUN_LED_TaskCallback(qEvent_t e)
{
    if(!status.isActive || !status.isInitialised)
        return;

    (void)e;

    if(status.LedImage == 1)
    {
        GPIO_RUN_LED_TurnOff();
        status.LedImage = 0;
    }
    else 
    {
        GPIO_Relay_TurnOn();
        status.LedImage = 1;
    }
}
