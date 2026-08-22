#include "GPIO_Mock.h"

static int GPIO_Relay_State;

void GPIO_Mock_init()
{
    GPIO_Relay_State = GPIO_STATE_UNDEFINED;
}

void GPIO_Relay_TurnOn()
{
    GPIO_Relay_State = GPIO_STATE_ON;
}

void GPIO_Relay_TurnOff()
{
    GPIO_Relay_State = GPIO_STATE_OFF;
}

int  GPIO_Relay_GetState()
{
    return GPIO_Relay_State;
}