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
#include "stm32h5xx_ll_cortex.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <arm_math.h>
#include "Logger.h"
#include "Relay.h"
#include <stdint.h>
#include "FPU.h"
#include "ADC_Service.h"
#include "CMD_Task.h"
#include "System_Config.h"
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


  System_Init();
  LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_0);

  ADC_Init();
  LL_ADC_EnableIT_AWD2(ADC1);

  LL_TIM_EnableAllOutputs(TIM1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3N);
  LL_TIM_EnableCounter(TIM1);

  CMD_Init();

  LL_USART_EnableDirectionTx(USART2);
  LL_USART_EnableDirectionRx(USART2);
  LL_USART_Enable(USART2);

  Relay_Init();
  Relay_SetThreshold(8000);

  while(!Relay_IsOn())
    Relay_SM();

  TIM1->CCR1 = UINT16_MAX/2;//- 2000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {    
    if(CMD_IsTaskReady())
      CMD_Task();

    if(LL_SYSTICK_IsEnabledIT())
      LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin);

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
