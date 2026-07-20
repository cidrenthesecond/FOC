#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_SetPhaseA(uint32_t CCR);
void PWM_SetPhaseB(uint32_t CCR);
void PWM_SetPhaseC(uint32_t CCR);
void PWM_SetSwitchingFrequency(uint32_t frequency);
uint32_t PWM_GetARR();

#endif
