#ifndef WAVEGEN_LUTS_H
#define WAVEGEN_LUTS_H

#include "stdint.h"

typedef struct WaveGen_LUT{
    const int32_t *data;
    const uint32_t size;
} WaveGen_LUT;

extern const WaveGen_LUT SINE;
extern const WaveGen_LUT SIGMOID;

#endif