#ifndef WAVEGEN_LUTS_H
#define WAVEGEN_LUTS_H

#include "SIN_LUT.h"

typedef struct WaveGen_LUT{
    const int32_t *data;
    const uint32_t size;
} WaveGen_LUT;

WaveGen_LUT SINE = {
    .data = lut,
    .size = sizeof(lut)/sizeof(lut[0])
};

#endif