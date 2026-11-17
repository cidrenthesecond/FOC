#include "LED_Task.h"
#include "qkernel.h"
#include "qtasks.h"
#include "stdint.h"

static void LED_TaskCallback(qEvent_t e);

void LED_Task_Init(LED_Task_t *self, const LED_Task_Interface_t *interface, qTime_t period)
{
    if((self               == NULL) ||
       (interface          == NULL) ||
       (interface->Led_ON  == NULL) ||
       (interface->Led_OFF == NULL) ||
       (period             <= 0))
    {
        return;
    }

    self->Status.isInitialised = 1;
    self->Status.isActive      = 0;
    self->Status.LedImage      = 0;
    self->Status.iterations    = 0;
    self->interface = *interface;
    self->interface.Led_OFF();
    qOS_Add_Task(&self->Task, LED_TaskCallback, qLowest_Priority, period, qIndefinite, qDisabled, self);
}

void LED_Task_SetActive(LED_Task_t *self,bool set, uint8_t iterations)
{
    if(self == NULL || !self->Status.isInitialised)
       return;

    if(set)
    {
        if(iterations == 0 ||
           iterations > ITERATIONS_INDEFINITE)
           return;
        
        qTask_Enable(&self->Task);
        self->Status.isActive   = 1;
        self->Status.iterations = iterations;
    }
    else
    {
        qTask_Disable(&self->Task);
        self->Status.isActive = 0;
        self->Status.LedImage = 0;
        self->interface.Led_OFF();
    }
}

static void LED_TaskCallback(qEvent_t e)
{
    LED_Task_t *self = (LED_Task_t *)e->TaskData;

    if(self == NULL)
        return;

    if(!self->Status.isActive || !self->Status.isInitialised)
        return;

    if(self->Status.LedImage == 1)
    {
        self->interface.Led_OFF();
        self->Status.LedImage = 0;
        if(self->Status.iterations != ITERATIONS_INDEFINITE)
        {
            self->Status.iterations--;
        }
    }
    else 
    {
        self->interface.Led_ON();
        self->Status.LedImage = 1;
    }

    if(self->Status.iterations == 0)
    {
        self->Status.isActive = 0;
        self->Status.LedImage = 0;
        qTask_Disable(&self->Task);
        self->interface.Led_OFF();
    }
}
