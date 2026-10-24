#include "SysTickWrapper.h"
#include "stm32h5xx_ll_cortex.h"

static volatile uint32_t sysTick = 0;

void SysTick_Init()
{
    LL_SYSTICK_EnableIT();
    sysTick = 0;
}

uint32_t getSysTick()
{
  return sysTick;
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  sysTick++;
}