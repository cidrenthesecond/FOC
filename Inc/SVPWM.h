#ifndef SVPWM_H
#define SVPWM_H

#include <stdint.h>

typedef struct{
    uint32_t timerCountingPeriod;
    float DC_LinkVoltage;
} SVPWM_InitStruct;

void SVPWM_Create(SVPWM_InitStruct set);
void SVPWM_Proccess(float alpha, float beta);
void SVPWM_Destroy();


#endif