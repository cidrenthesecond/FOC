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
#include "SIN_LUT.h"
#include "Thermistor.h"
#include "FPU.h"
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


void ADC_Start();

static uint16_t DcLinkMeasurement;
static uint16_t NtcMeasurement;

#define ADC_MAX              4095
#define DIVIDER_CONSTANT     126
#define REFERENCE_VOLTAGE_mV 3300


void ADC_Start()
{
	LL_ADC_REG_StartConversion(ADC1);

	while(!LL_ADC_IsActiveFlag_EOC(ADC1))
		  ;

	DcLinkMeasurement = LL_ADC_REG_ReadConversionData12(ADC1); // result A0

	while(!LL_ADC_IsActiveFlag_EOC(ADC1))
		;

	NtcMeasurement = LL_ADC_REG_ReadConversionData12(ADC1); // result A1
	//LL_ADC_ClearFlag_EOC(ADC1);

}

uint32_t GetDcLinkVoltage(uint16_t adcMeasurement)
{
	// ADC_Start();

	uint32_t result;
	result = DIVIDER_CONSTANT * REFERENCE_VOLTAGE_mV * adcMeasurement / ADC_MAX;
	return result;
}

uint16_t GetNtcVoltage()
{
	ADC_Start();
	return NtcMeasurement*REFERENCE_VOLTAGE_mV / ADC_MAX;
}

#define V_BIAS 1700

int32_t GetCurrent(uint16_t adcMeasurement)
{
  int32_t Vmeasured = REFERENCE_VOLTAGE_mV * adcMeasurement /ADC_MAX;
  int32_t Vsign = Vmeasured - V_BIAS;
  return Vsign*3;
  return Vsign * 3910;
  //return (float)Vsign * 3.91f / 1000.0f;
}

void Print_ADC_CurrentValues()
{
  char buffer1[30] = {0};

  sprintf(buffer1, "%u",LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2));
  LOG(buffer1);

  sprintf(buffer1, "%u",LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3));
  LOG(buffer1);

  sprintf(buffer1, "%u",LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_4));
  LOG(buffer1);
}

void Print_Current()
{
  char buffer1[30] = {0};

  sprintf(buffer1, "%ld",GetCurrent(LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2)));
  LOG(buffer1);

  sprintf(buffer1, "%ld",GetCurrent(LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3)));
  LOG(buffer1);

  sprintf(buffer1, "%ld",GetCurrent(LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_4)));
  LOG(buffer1);
}

uint16_t a_mid,b_mid,c_mid;

void Calculate_MIDpoints()
{
  LL_ADC_INJ_SetTriggerSource(ADC1, LL_ADC_INJ_TRIG_SOFTWARE);
  LL_ADC_Enable(ADC1);
  while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
      ; // Czekamy aż przetwornik będzie gotowy

  LL_ADC_ClearFlag_JEOS(ADC1);
  LL_ADC_INJ_StartConversion(ADC1);
  while (!LL_ADC_IsActiveFlag_JEOS(ADC1))
    ;
  LL_ADC_ClearFlag_JEOS(ADC1);

  uint16_t a_data[5];
  uint16_t b_data[5];
  uint16_t c_data[5];

  for(uint8_t i = 0; i < 5; i++)
  {
    LL_ADC_INJ_StartConversion(ADC1);

    while(!LL_ADC_IsActiveFlag_JEOS(ADC1))
      ;

    a_data[i] = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2);
    b_data[i] = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3);
    c_data[i] = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_4);

    LL_ADC_ClearFlag_JEOS(ADC1);
  }

  uint16_t a_sum = 0, b_sum = 0, c_sum = 0;


  for(uint8_t i = 0 ; i < 5; i++)
  {
    a_sum += a_data[i];
    b_sum += b_data[i];
    c_sum += c_data[i];
  }

  a_mid = a_sum /5;
  b_mid = b_sum /5;
  c_mid = c_sum /5;

  char buffer1[30] = {0};

  sprintf(buffer1, "A : %u",a_mid);
  LOG(buffer1);
  sprintf(buffer1, "B : %u",b_mid);
  LOG(buffer1);
  sprintf(buffer1, "C : %u",c_mid);
  LOG(buffer1);

  LL_ADC_Disable(ADC1);
  LL_ADC_INJ_SetTriggerSource(ADC1, LL_ADC_INJ_TRIG_EXT_TIM1_TRGO);
  
}

void PrintCurrent_A()
{
  uint16_t adc_measurement = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2);
  int16_t delta = (int16_t)adc_measurement - (int16_t)a_mid;
  int16_t value = delta * 3;

  char buffer1[30] = {0};
  sprintf(buffer1,"A : %d",value);
  LOG(buffer1);
}

void PrintCurrent_B()
{
  uint16_t adc_measurement = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3);
  int16_t delta = (int16_t)adc_measurement - (int16_t)b_mid;
  int16_t value = delta *3;

  char buffer1[30] = {0};
  sprintf(buffer1,"B : %d",value);
  LOG(buffer1);
}

void PrintCurrent_C()
{
  uint16_t adc_measurement = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2);
  int16_t delta = (int16_t)adc_measurement - (int16_t)c_mid;
  int16_t value = delta *3;

  char buffer1[30] = {0};
  sprintf(buffer1,"C : %d",value);
  LOG(buffer1);
}

int test;
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
  MX_ICACHE_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  FPU_enable();


  LOG_Init(UART_PrintPolling, 10);



  Calculate_MIDpoints();

  LL_ADC_Enable(ADC1);
  LL_ADC_INJ_StartConversion(ADC1);

  LL_TIM_EnableAllOutputs(TIM1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3N);
  LL_TIM_EnableCounter(TIM1);

  LL_USART_EnableDirectionTx(USART2);
  LL_USART_Enable(USART2);
  




  Relay_Init();
  Relay_SetThreshold(8000);
  TIM1->CCR1 = UINT16_MAX/2;//- 2000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;

  char buffer[30] = {0};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    sprintf(buffer, "DC link voltage: %lu V",GetDcLinkVoltage(LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_1)));
    LOG(buffer);

    // float current = GetCurrent(LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2));

    // // Wyciągamy część całkowitą i ułamkową (np. dla 2 miejsc po przecinku)
    // int32_t integral = (int32_t)current;
    // int32_t fractional = (int32_t)((current - integral) * 100);

    // // Obsługa wartości ujemnych dla części ułamkowej
    // if (fractional < 0) {
    //   fractional = -fractional;
    // }

    // sprintf(buffer, "%ld.%02ld A", integral, fractional);
    // LOG(buffer);
    // Print_Current();
    // PrintCurrent_A();
    // PrintCurrent_B();
    // PrintCurrent_C();

    while(!LL_ADC_IsActiveFlag_JEOS(ADC1))
      ;

    //Print_ADC_CurrentValues();
    PrintCurrent_A();
    PrintCurrent_B();
    PrintCurrent_C();
    LL_ADC_ClearFlag_JEOS(ADC1);

    for(uint32_t delay = 0; delay < 9000000; delay++)
	    ;

    
//	  int16_t Temp = Thermistor_GetHeatsinkTemp();
//	  char buff[20];
//	  sprintf(buff, "HT : %d\n",Temp);
//	  LOG(buff);

//	  uint32_t DC = GetDcLinkVoltage();
//	  char buff[30];
//	  sprintf(buff, "DC : %lu\n",DC);
//	  LOG(buff);

//	  ADC_Start();
//
//	  char buff[40];
//	  sprintf(buff, "DC : %u HT : %u\n",DcLinkMeasurement,NtcMeasurement);
//	  LOG(buff);

//	  while(!Relay_IsOn())
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
