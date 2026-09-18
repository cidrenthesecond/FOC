/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "gpdma.h"
#include "icache.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <arm_math.h>
#include "Logger.h"
#include "Relay.h"
#include <stdint.h>
#include <stdio.h>
#include "Thermistor.h"
#include "FPU.h"
#include "ADC_Service.h"
#include "CommandExecute.h"
#include "CMD_Manager.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint32_t phaseAccumulator = 0;
uint32_t  increment = UINT32_MAX/36;
float target = 1.0f;

// void OpenLoop_LUT(float * alpha, float *beta)
// {
// 	uint16_t index = phaseAccumulator >> 22;

// 	float sin = (float)lut[index] / (float)INT32_MAX;//INT32_MAX;

// 	uint32_t cosIndex = (index + 256) & 1023;
// 	float cos = (float)lut[cosIndex] / (float)INT32_MAX;

// 	*alpha = target * cos;
// 	*beta  = target * sin;

// 	phaseAccumulator += increment;
// }



void UART_PrintPolling(const char *pData,uint8_t length)
{
	  for(uint8_t index = 0; index < length; index++)
	  {
		  while(!LL_USART_IsActiveFlag_TXE(USART2))
			  ;

		  LL_USART_TransmitData8(USART2, pData[index]);
		  LOG_WakeUp();
	  }
}

void Send_USART_DMA_LL(const char *pData, uint8_t Size)
{
    // 1. Wyłącz kanał DMA przed konfiguracją
    LL_DMA_DisableChannel(GPDMA1, LL_DMA_CHANNEL_0);

    // 2. Ustaw adres źródłowy (pamięć RAM)
    LL_DMA_SetSrcAddress(GPDMA1, LL_DMA_CHANNEL_0, (uint32_t)pData);

    // 3. Ustaw adres docelowy (rejestr nadawczy USART TDR)
    LL_DMA_SetDestAddress(GPDMA1, LL_DMA_CHANNEL_0, LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT));

    // 4. Ustaw liczbę bajtów do przesłania
    LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_0, Size);

    // 5. Wyczyść flagę zakończenia transferu (Transfer Complete) na kanale 0
    LL_DMA_ClearFlag_TC(GPDMA1, LL_DMA_CHANNEL_0);

    // 6. Włącz kanał GPDMA
    LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_0);

    // 7. Włącz żądanie transmisji DMA w peryferium USART
    LL_USART_EnableDMAReq_TX(USART2);
}

char rx_buffer[64];
volatile uint16_t rx_bytes_received = 0;
volatile uint8_t data_ready_flag = 0;

void Start_USART_RX_DMA(void)
{
    // 1. Wyłącz kanał DMA przed konfiguracją
    LL_DMA_DisableChannel(GPDMA1, LL_DMA_CHANNEL_1);

    // 2. Adres źródłowy (rejestr odbiorczy USART RDR)
    LL_DMA_SetSrcAddress(GPDMA1, LL_DMA_CHANNEL_1, LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_RECEIVE));

    // 3. Adres docelowy (bufor w RAM)
    LL_DMA_SetDestAddress(GPDMA1, LL_DMA_CHANNEL_1, (uint32_t)rx_buffer);

    // 4. Maksymalny rozmiar oczekiwanego bufora
    LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_1, 64);

    // 5. Włącz kanał DMA i zgłoszenie DMA w USART
    LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_1);
    LL_USART_EnableDMAReq_RX(USART2);

    // 6. Wyczyść i włącz przerwanie IDLE Line w USART
    LL_USART_ClearFlag_IDLE(USART2);
    LL_USART_EnableIT_IDLE(USART2);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  FPU_enable();


  LOG_Init(Send_USART_DMA_LL, 10);
  LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_0);
  ADC_Init();

  LL_TIM_EnableAllOutputs(TIM1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3N);
  LL_TIM_EnableCounter(TIM1);

  LL_USART_EnableDirectionTx(USART2);
  LL_USART_EnableDirectionRx(USART2);
  Start_USART_RX_DMA();
  LL_USART_Enable(USART2);

  CommandExecute_Init();
  CommandExecute_Register(10, ADC_Measure);
  CommandExecute_Register(11, NTC_Measure);
  

  
  Relay_Init();
  Relay_SetThreshold(8000);
  TIM1->CCR1 = UINT16_MAX/2;//- 2000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {    
    if(data_ready_flag)
    {
      CommandExecute(rx_buffer);
      memset(rx_buffer,0,sizeof(rx_buffer));
      data_ready_flag = 0;
      Start_USART_RX_DMA();
    }

//	while(!Relay_IsOn())
//		  Relay_SM();
//
//	  for(uint32_t delay = 0; delay < 100000; delay++)
//	    	;
	  //LL_GPIO_SetOutputPin(WCET_GPIO_Port, WCET_Pin);
	  //OpenLoop_AlfaBeta(&alpha,&beta);
	  //OpenLoop_LUT(&alpha,&beta);
	  //LL_GPIO_ResetOutputPin(WCET_GPIO_Port, WCET_Pin);
	  //SVPWM(alpha, beta);

	  // for(uint32_t delay =0 ;delay < 600000; delay++)
		//   ;

	  // LOG("Miau");
	  // test++;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_HSI_SetDivider(LL_RCC_HSI_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);

  LL_Init1msTick(32000000);

  LL_SetSystemCoreClock(32000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{

  /* Disables the MPU */
  LL_MPU_Disable();

  /** Initializes and configures the Region 0 and the memory to be protected
  */
  LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER0, LL_MPU_REGION_ALL_RO|LL_MPU_INSTRUCTION_ACCESS_DISABLE|LL_MPU_ACCESS_NOT_SHAREABLE, LL_MPU_ATTRIBUTES_NUMBER0, 0x08FFF000, 0x08FFFFFF);
  LL_MPU_EnableRegion(LL_MPU_REGION_NUMBER0);

  /** Initializes and configures the Attribute 0 and the memory to be protected
  */
  LL_MPU_ConfigAttributes(LL_MPU_ATTRIBUTES_NUMBER0, LL_MPU_NOT_CACHEABLE);
  /* Enables the MPU */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
