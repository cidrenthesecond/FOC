#include "FPU.h"
#include "stm32h533xx.h"

void FPU_enable()
{
	SCB -> CPACR |= (0xF<<20);
}