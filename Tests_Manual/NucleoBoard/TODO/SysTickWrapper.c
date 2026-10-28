#include "SysTickWrapper.h"
#include "stm32h5xx_ll_cortex.h"

typedef struct Timeout_struct_t{
    uint32_t timeoutValue;
    uint32_t timeoutStart;
    uint8_t  isUsed    : 1;
    uint8_t  running   : 1;
    uint8_t  triggered : 1;
    uint8_t  handled   : 1;
    uint8_t  reserved  : 4;
} Timeout_struct_t;

static Timeout_struct_t timeoutList[TIMEOUT_AMOUNT];
static volatile uint32_t sysTick = 0;

void SysTick_Init()
{
  LL_SYSTICK_EnableIT();
  sysTick = 0;

  for(uint8_t index = 0; index < TIMEOUT_AMOUNT; index++)
  {
    timeoutList[index].timeoutValue = 0;
    timeoutList[index].timeoutStart = 0;
    timeoutList[index].isUsed       = 0;
    timeoutList[index].running      = 0;
    timeoutList[index].triggered    = 0;
    timeoutList[index].handled      = 0;
  }
}

Timeout_t Timeout_Create(uint32_t timeout)
{
  uint8_t firstFreeIndex = 0;
  for(firstFreeIndex = 0; firstFreeIndex < TIMEOUT_AMOUNT; firstFreeIndex++)
  {
    if(timeoutList[firstFreeIndex].isUsed == 0)
    {
      timeoutList[firstFreeIndex].timeoutValue = timeout;
      timeoutList[firstFreeIndex].isUsed       = 1;
      timeoutList[firstFreeIndex].running      = 0;
      timeoutList[firstFreeIndex].triggered    = 0;
      timeoutList[firstFreeIndex].handled      = 0;
      return &timeoutList[firstFreeIndex];
    }
  }

  return NULL;
}

void Timeout_Start(Timeout_t timeout)
{
  timeout -> running      = 1;
  timeout -> timeoutStart = getSysTick();
}

uint8_t Timeout_IsTriggered(Timeout_t timeout)
{
  if(timeout ->running != 1 || timeout -> isUsed != 1 || timeout ->triggered != 0 || timeout ->handled != 0)
    return 0;

  if(getSysTick() - (timeout ->timeoutStart) >= (timeout ->timeoutValue))
  {
    timeout -> triggered = 1;
    timeout -> handled   = 1;
    timeout -> running   = 0;

    return 1;
  }

  return 0;
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