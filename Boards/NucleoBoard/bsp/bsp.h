#ifndef BSP_H
#define BSP_H

#include "stm32h533xx.h"
#include "stm32h5xx_ll_dma.h"
#include "stm32h5xx_ll_adc.h"

//______________GPIO__________________

#define GPIO_RELAY_PORT (GPIOB)
#define GPIO_RELAY_PIN  (LL_GPIO_PIN_6)

//only for this board
extern GPIO_TypeDef fake_gpio_instance;
#define FAKE_GPIO_PORT (&fake_gpio_instance)
#define FAKE_GPIO_PIN  1

#define RUN_LED_GPIO_PORT   (FAKE_GPIO_PORT)
#define RUN_LED_GPIO_PIN    (FAKE_GPIO_PIN)
#define ERROR_LED_GPIO_PORT (FAKE_GPIO_PORT) 
#define ERROR_LED_GPIO_PIN  (FAKE_GPIO_PIN)
#define ESTOP_LED_GPIO_PORT (FAKE_GPIO_PORT)
#define ESTOP_LED_GPIO_PIN  (FAKE_GPIO_PIN)


#define MUX_ENCODER_A_PORT (FAKE_GPIO_PORT)
#define MUX_ENCODER_A_PIN  (FAKE_GPIO_PIN)
#define MUX_ENCODER_B_PORT (FAKE_GPIO_PORT)
#define MUX_ENCODER_B_PIN  (FAKE_GPIO_PIN)
#define MUX_ENCODER_0_PORT (FAKE_GPIO_PORT)
#define MUX_ENCODER_0_PIN  (FAKE_GPIO_PIN)

#define LINE_RECEIVER_EN_PORT (FAKE_GPIO_PORT)      
#define LINE_RECEIVER_EN_PIN  (FAKE_GPIO_PIN)      

#define ENCODER_PWR_SWITCH_EN_PORT (FAKE_GPIO_PORT)
#define ENCODER_PWR_SWITCH_EN_PIN  (FAKE_GPIO_PIN)

//_______________PWM__________________________

#define PWM_TIMER (TIM1)

//_____________LOG_UART________________________

#define LOG_UART        (USART2)
#define LOG_DMA         (GPDMA1)
#define LOG_DMA_CHANNEL (LL_DMA_CHANNEL_0)

//_____________CMD_UART_____________________

#define CMD_UART        (USART2)
#define CMD_DMA         (GPDMA1)
#define CMD_DMA_CHANNEL (LL_DMA_CHANNEL_1)

//____________________ADC___________________

#define ADC_USED                (ADC1)
#define ADC_CHANNEL_PHASE_A     (LL_ADC_CHANNEL_3)
#define ADC_CHANNEL_PHASE_B     (LL_ADC_CHANNEL_4)
#define ADC_CHANNEL_PHASE_C     (LL_ADC_CHANNEL_7)
#define ADC_CHANNEL_BUS_VOLTAGE (LL_ADC_CHANNEL_1)
#define ADC_CHANNEL_NTC         (LL_ADC_CHANNEL_0)

#endif