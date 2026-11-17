#ifndef CONTROL_UTILITY_H
#define CONTROL_UTILITY_H

#include "stdint.h"

typedef struct {
    float alpha;
    float beta;
} VoltageVector_t;

VoltageVector_t GetVoltageVector(int32_t sinValue,int32_t cosValue,float magnitude);

#endif