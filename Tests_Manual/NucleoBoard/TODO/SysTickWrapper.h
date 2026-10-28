#ifndef SYS_TICK_WRAPPER_H
#define SYS_TICK_WRAPPER_H

#include "stdint.h"

#define TIMEOUT_AMOUNT 10u

typedef struct Timeout_struct_t * Timeout_t;

void SysTick_Init();
uint32_t getSysTick();
Timeout_t Timeout_Create(uint32_t timeout);
void Timeout_Start(Timeout_t timeout);
uint8_t Timeout_IsTriggered(Timeout_t timeout);

#endif