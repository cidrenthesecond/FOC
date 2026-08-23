#include "GPIO_Driver.h"
#include "gpio.h"

#define GPIO_RELAY_PORT RELAY_GPIO_Port
#define GPIO_RELAY_PIN  RELAY_Pin

void GPIO_Relay_TurnOn()
{
	LL_GPIO_SetOutputPin(GPIO_RELAY_PORT, GPIO_RELAY_PIN);
}

void GPIO_Relay_TurnOff()
{
	LL_GPIO_ResetOutputPin(GPIO_RELAY_PORT, GPIO_RELAY_PIN);
}

int  GPIO_Relay_GetState()
{
	return LL_GPIO_IsOutputPinSet(GPIO_RELAY_PORT, GPIO_RELAY_PIN);
}
