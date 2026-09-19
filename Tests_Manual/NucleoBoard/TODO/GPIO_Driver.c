#include "GPIO_Driver.h"
#include "stm32h533xx.h"
#include "stm32h5xx_ll_gpio.h"

#define GPIO_RELAY_PORT GPIOB
#define GPIO_RELAY_PIN  LL_GPIO_PIN_6

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
