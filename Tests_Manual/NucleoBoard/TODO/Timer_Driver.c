#include "Timer_Driver.h"
#include "stm32h5xx_ll_tim.h"
#include "stm32h5xx_ll_rcc.h"
#include "Logger.h"

#define PWM_TIMER TIM1
  
void PWM_Init()
{
  LL_TIM_EnableAllOutputs(PWM_TIMER);
  LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH2);
  LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH1N);
  LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH2N);
  LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH3N);
  //LL_TIM_EnableCounter(PWM_TIMER);

}

void PWM_SetCaptureCompare(uint16_t phaseA, uint16_t phaseB, uint16_t phaseC)
{
    LL_TIM_OC_SetCompareCH1(PWM_TIMER, phaseA);
    LL_TIM_OC_SetCompareCH2(PWM_TIMER, phaseB);
    LL_TIM_OC_SetCompareCH3(PWM_TIMER, phaseC);
}

void PWM_SetDuty(float dutyA, float dutyB, float dutyC)
{
    uint32_t arr  = PWM_TIMER->ARR;   /* real timer period, never a magic number */
    uint32_t ccrA = (uint32_t)(dutyA * (float)arr + 0.5f);
    uint32_t ccrB = (uint32_t)(dutyB * (float)arr + 0.5f);
    uint32_t ccrC = (uint32_t)(dutyC * (float)arr + 0.5f);

    PWM_TIMER->CCR1 = ccrA;
    PWM_TIMER->CCR2 = ccrB;
    PWM_TIMER->CCR3 = ccrC;
}

uint32_t PWM_GetSwitchingFrequency()
{
    LL_RCC_ClocksTypeDef clocks;
    LL_RCC_GetSystemClocksFreq(&clocks);

    uint32_t ClockFrequency = clocks.PCLK2_Frequency;
    //Prescaler is *2 beacause center aligned PWM counts up and down
    uint32_t Prescaler = 2 * (1 + LL_TIM_GetAutoReload(PWM_TIMER)) * (1 + LL_TIM_GetPrescaler(PWM_TIMER));
    return ClockFrequency / Prescaler;
}

uint32_t PWM_GetAutoReload()
{
    return LL_TIM_GetAutoReload(PWM_TIMER);
}

void PWM_ESTOP()
{
    LL_TIM_GenerateEvent_BRK(TIM1);
    LOG("Software error : ESTOP -> Engaged");
}

void PWM_ReArm()
{
    LL_TIM_ClearFlag_BRK(TIM1);
    LL_TIM_EnableAllOutputs(PWM_TIMER);
    LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH2);
    LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH3);
    LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH1N);
    LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH2N);
    LL_TIM_CC_EnableChannel(PWM_TIMER, LL_TIM_CHANNEL_CH3N);
    LOG("[PWM] : ARMED");
}




