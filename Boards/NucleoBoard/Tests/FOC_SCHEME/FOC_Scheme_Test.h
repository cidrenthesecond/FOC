#ifndef FOC_SCHEME_TEST_H
#define FOC_SCHEME_TEST_H

#include "stdint.h"

uint8_t GenerateVoltage_IsTaskReady(void);
void    GenerateVoltage_TaskEnable(void);
void    GenerateVoltage_Task(void);

void OpenLoop_AlfaBeta(float * alpha, float *beta);



#endif