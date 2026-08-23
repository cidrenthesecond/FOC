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
#include <stdio.h>
#include "SIN_LUT.h"
#include "Thermistor.h"
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
static void EnableFPU()
{
	SCB -> CPACR |= (0xF<<20);
}

float d_setpoint = 1.0f;//1.0f;
float q_setpoint = 1.0f;

float alpha = 0.0f;
float beta = 0.0f;

const float_t angle[36] = {
 0.0, 10.0, 20.0, 30.0,
 40.0, 50.0, 60.0, 70.0,
 80.0, 90.0, 100.0, 110.0,
 120.0, 130.0, 140.0, 150.0,
 160.0, 170.0, 180.0, 190.0,
 200.0, 210.0, 220.0, 230.0,
 240.0, 250.0, 260.0, 270.0,
 280.0, 290.0, 300.0, 310.0,
 320.0, 330.0, 340.0, 350.0 };

int8_t loop = 0;

__attribute__((optimize("O3")))
void OpenLoop_AlfaBeta(float * alpha, float *beta)
{
	float sinus;
	float cosinus;
	arm_sin_cos_f32(angle[loop], &sinus, &cosinus);
	arm_inv_park_f32(d_setpoint, q_setpoint, alpha, beta, sinus, cosinus);
	loop = (loop + 1) % 36;
}

uint32_t phaseAccumulator = 0;
uint32_t  increment = UINT32_MAX/36;
float target = 1.0f;

void OpenLoop_LUT(float * alpha, float *beta)
{
	uint16_t index = phaseAccumulator >> 22;

	float sin = (float)lut[index] / (float)INT32_MAX;//INT32_MAX;

	uint32_t cosIndex = (index + 256) & 1023;
	float cos = (float)lut[cosIndex] / (float)INT32_MAX;

	*alpha = target * cos;
	*beta  = target * sin;

	phaseAccumulator += increment;
}

float findMax(float a, float b)
{
	return (a >= b) ? a : b;
}

float findMin(float a, float b)
{
	return (a <= b) ? a : b;
}

__attribute__((optimize("O3")))
void SVPWM(float alpha, float beta)
{
	float _alpha = alpha/3.3f;
	float _beta = beta/3.3f;


	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	float max = findMax(findMax(a,b),c);
	float min = findMin(findMin(a,b),c);

	float offset = (max + min) * 0.5f;

	a -= offset;
	b -= offset;
	c -= offset;

	TIM1 -> CCR1 = (uint32_t) ((a + 0.5f) * 65535);
	TIM1 -> CCR2 = (uint32_t) ((b + 0.5f) * 65535);
	TIM1 -> CCR3 = (uint32_t) ((c + 0.5f) * 65535);
}

__attribute__((optimize("O3")))
void SPWM(float alpha, float beta)
{
	float _alpha = alpha/3.3f;
	float _beta = beta/3.3f;

	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	TIM1 -> CCR1 = (uint32_t) ((a + 0.5f) * 65535);
	TIM1 -> CCR2 = (uint32_t) ((b + 0.5f) * 65535);
	TIM1 -> CCR3 = (uint32_t) ((c + 0.5f) * 65535);
}

__attribute__((optimize("O3")))
void DPWM_MAX(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    float max = findMax(findMax(a, b), c);

    /* Unconditional top-rail clamp of the max phase. This is what makes it
     * DPWMMAX (contrast with DPWM1/2, which flip between clamping max and
     * min depending on sign(max+min)). With this offset, the max phase's
     * duty comes out to exactly 1.0 automatically -- no manual override
     * needed. */
    float offset = 0.5f - max;

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- DPWMMAX has less linear-range headroom than DPWM1/2, so
     * this matters more here, not less. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

float abs_f(float x)
{
	return (x >= 0) ? x : -x;
}

__attribute__((optimize("O3")))
void DPWM1(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;
    float _beta  = beta  / 3.3f;

    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if (max + min >= 0.0f)
        offset = 0.5f - max;
    else
        offset = -0.5f - min;

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- guards against over-modulation AND against casting a
     * negative float to an unsigned CCR register (undefined behaviour). */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* actual timer period -- not a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);  /* +0.5f = round-to-nearest */
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

#define COSINE_MINUS_30   0.8660254f   /* cos(-30 deg) */
#define SINE_MINUS_30     0.5f         /* -sin(-30 deg) */

/* Rotate (alpha, beta) by -30 degrees. */
static inline void rotate_minus30(float alpha, float beta,
                                   float *alpha_r, float *beta_r)
{
    *alpha_r = COSINE_MINUS_30 * alpha + SINE_MINUS_30 * beta;
    *beta_r  = -SINE_MINUS_30   * alpha + COSINE_MINUS_30 * beta;
}

__attribute__((optimize("O3")))
void DPWM2(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    float alpha_v, beta_v, a_v, b_v, c_v;
    rotate_minus30(_alpha, _beta, &alpha_v, &beta_v);
    arm_inv_clarke_f32(alpha_v, beta_v, &a_v, &b_v);
    c_v = -a_v - b_v;

    float max_v = findMax(findMax(a_v, b_v), c_v);
    float min_v = findMin(findMin(a_v, b_v), c_v);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

#define COS_MINUS_60   0.5f   /* cos(-60 deg) */
#define SIN_MINUS_60   -0.8660254f         /* -sin(-60 deg) */

/* Rotate (alpha, beta) by -60 degrees. */
static inline void rotate_minus60(float alpha, float beta,
                                   float *alpha_r, float *beta_r)
{
    *alpha_r = COS_MINUS_60 * alpha + SIN_MINUS_60 * beta;
    *beta_r  = -SIN_MINUS_60   * alpha + COS_MINUS_60 * beta;
}

__attribute__((optimize("O3")))
void DPWM3(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    float alpha_v, beta_v, a_v, b_v, c_v;
    rotate_minus60(_alpha, _beta, &alpha_v, &beta_v);
    arm_inv_clarke_f32(alpha_v, beta_v, &a_v, &b_v);
    c_v = -a_v - b_v;

    float max_v = findMax(findMax(a_v, b_v), c_v);
    float min_v = findMin(findMin(a_v, b_v), c_v);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

#define COS_30   0.8660254f   /* cos(30 deg) */
#define SIN_30   -0.5f         /* -sin(30 deg) */

/* Rotate (alpha, beta) by -60 degrees. */
static inline void rotate_30(float alpha, float beta,
                                   float *alpha_r, float *beta_r)
{
    *alpha_r = COS_30 * alpha + SIN_30 * beta;
    *beta_r  = -SIN_30   * alpha + COS_30 * beta;
}

__attribute__((optimize("O3")))
void DPWM0(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    float alpha_v, beta_v, a_v, b_v, c_v;
    rotate_30(_alpha, _beta, &alpha_v, &beta_v);
    arm_inv_clarke_f32(alpha_v, beta_v, &a_v, &b_v);
    c_v = -a_v - b_v;

    float max_v = findMax(findMax(a_v, b_v), c_v);
    float min_v = findMin(findMin(a_v, b_v), c_v);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}


__attribute__((optimize("O3")))
void DPWM_MIN(float alpha, float beta)
{
	float _alpha = alpha/3.3f;
	float _beta = beta/3.3f;


	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	float min = findMin(findMin(a,b),c);

	float offset = -0.5f - min;

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- DPWMMAX has less linear-range headroom than DPWM1/2, so
     * this matters more here, not less. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

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

uint32_t GetDcLinkVoltage()
{
	ADC_Start();

	uint32_t result;
	result = DIVIDER_CONSTANT * REFERENCE_VOLTAGE_mV * DcLinkMeasurement / ADC_MAX;
	return result;
}

uint16_t GetNtcVoltage()
{
	ADC_Start();
	return NtcMeasurement*REFERENCE_VOLTAGE_mV / ADC_MAX;

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
  EnableFPU();
  LL_TIM_EnableAllOutputs(TIM1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_EnableCounter(TIM1);

  LL_USART_EnableDirectionTx(USART2);
  LL_USART_Enable(USART2);


  LOG_Init(UART_PrintPolling, 10);

  Relay_Init();
  Relay_SetThreshold(8000);

  LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);

  while(LL_ADC_IsCalibrationOnGoing(ADC1))
	  ;

  LL_ADC_Enable(ADC1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

	  for(uint32_t delay =0 ;delay < 600000; delay++)
		  ;

	  LOG("Miau");
	  test++;
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
