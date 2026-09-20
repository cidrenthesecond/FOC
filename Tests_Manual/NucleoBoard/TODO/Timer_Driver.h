#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include "stdint.h"

void PWM_Init();
void PWM_SetCaptureCompare(uint16_t phaseA, uint16_t phaseB, uint16_t phaseC);
void PWM_SetDuty(float dutyA, float dutyB, float dutyC);
void PWM_ESTOP();
void PWM_ReArm();
uint32_t PWM_GetSwitchingFrequency();
uint32_t PWM_GetAutoReload();

#endif