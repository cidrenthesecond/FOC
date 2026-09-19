#include "SysTickWrapper.h"
#include "stm32h5xx_ll_cortex.h"

void SysTick_Init()
{
    SysTickDispatcher_Init();
    LL_SYSTICK_EnableIT();
}