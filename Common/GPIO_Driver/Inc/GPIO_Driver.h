#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

void GPIO_Relay_TurnOn();
void GPIO_Relay_TurnOff();
int  GPIO_Relay_GetState();

void GPIO_RUN_LED_TurnOff();
void GPIO_RUN_LED_TurnOn();
void GPIO_Error_LED_TurnOn();
void GPIO_Error_LED_TurnOff();
void GPIO_ESTOP_LED_TurnOn();
void GPIO_ESTOP_LED_TurnOff();

typedef enum {
    DIRECT,
    DIFERENTIAL
} Encoder_Signal_Source_t;

void GPIO_Enable_LineReceiver();
void GPIO_Disable_LineReceiver();
void MUX_Select_Encoder_Source(Encoder_Signal_Source_t source);

void GPIO_Set_ESTOP_Callback(void (*callback)(void));
void GPIO_Set_StartStop_Callback(void (*callback)(void));
void GPIO_Set_ErrorFixed_Callback(void (*callback)(void));
void GPIO_Set_EncoderPowerFault_Callback(void (*callback)(void));





#endif