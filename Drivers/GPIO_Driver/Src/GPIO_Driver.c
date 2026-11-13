#include "bsp.h"
#include "GPIO_Driver.h"
#include "stm32h5xx_ll_gpio.h"



static void (*Encoder_Power_Fault_Callback)(void);
static void (*Error_Fixed_Button_Callback)(void);
static void (*Start_Stop_Button_Callback)(void);
static void (*ESTOP__Button_Callback)(void);


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

void GPIO_RUN_LED_TurnOn()
{
	LL_GPIO_SetOutputPin(RUN_LED_GPIO_PORT, RUN_LED_GPIO_PIN);
}

void GPIO_RUN_LED_TurnOff()
{
	LL_GPIO_ResetOutputPin(RUN_LED_GPIO_PORT, RUN_LED_GPIO_PIN);
}

void GPIO_Error_LED_TurnOn()
{
	LL_GPIO_SetOutputPin(ERROR_LED_GPIO_PORT, ERROR_LED_GPIO_PIN);
}

void GPIO_Error_LED_TurnOff()
{
	LL_GPIO_ResetOutputPin(ERROR_LED_GPIO_PORT, ERROR_LED_GPIO_PIN);
} 

void GPIO_ESTOP_LED_TurnOn()
{
	LL_GPIO_SetOutputPin(ESTOP_LED_GPIO_PORT,ESTOP_LED_GPIO_PIN);
}

void GPIO_ESTOP_LED_TurnOff()
{
	LL_GPIO_ResetOutputPin(ESTOP_LED_GPIO_PORT, ESTOP_LED_GPIO_PIN);
}


void GPIO_Enable_LineReceiver()
{

}

void GPIO_Disable_LineReceiver()
{

}

void MUX_Select_Encoder_Source(Encoder_Signal_Source_t source)
{
	if(source == DIRECT){
		
	}
	else if (source == DIFERENTIAL) {
	
	}
}

void GPIO_Set_EncoderPowerFault_Callback(void (*callback)(void))
{
	if(callback == NULL)
		return;

	Encoder_Power_Fault_Callback = callback;
}

void GPIO_Set_ErrorFixed_Callback(void (*callback)(void))
{
	if(callback == NULL)
		return;

	Error_Fixed_Button_Callback = callback;
}

void GPIO_Set_StartStop_Callback(void (*callback)(void))
{
	if(callback == NULL)
		return;

	Start_Stop_Button_Callback = callback;
}

void GPIO_Set_ESTOP_Callback(void (*callback)(void))
{
	if(callback == NULL)
		return;

	ESTOP__Button_Callback = callback;
}
