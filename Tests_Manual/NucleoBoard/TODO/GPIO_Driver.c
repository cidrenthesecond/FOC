#include "GPIO_Driver.h"
#include "stm32h533xx.h"
#include "stm32h5xx_ll_gpio.h"

#define GPIO_RELAY_PORT GPIOB
#define GPIO_RELAY_PIN  LL_GPIO_PIN_6

#define MUX_ENCODER_A_PORT 1
#define MUX_ENCODER_A_PIN  2
#define MUX_ENCODER_B_PORT 3
#define MUX_ENCODER_B_PIN  4
#define MUX_ENCODER_0_PORT 5
#define MUX_ENCODER_0_PIN  6
#define LINE_RECEIVER_EN_PORT      7
#define LINE_RECEIVER_EN_PIN       8
#define ENCODER_PWR_SWITCH_EN_PORT 9
#define ENCODER_PWR_SWITCH_EN_PIN  10

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
