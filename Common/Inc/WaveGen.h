#ifndef WAVEGEN_H
#define WAVEGEN_H

#include "stdint.h"
#include "WaveGen_LUTs.h"

typedef struct WaveGen_struct * WaveGen;

WaveGen WaveGen_Create(const WaveGen_LUT *desiredWave);
int32_t      WaveGen_Get(WaveGen me);
void          WaveGen_Reset(WaveGen w);
void          WaveGen_SetFrequency(WaveGen w,float frequency,float samplingFrequency);
uint8_t      WaveGen_SetDesiredWave(WaveGen w,const WaveGen_LUT *desiredWave);
void          WaveGen_Destroy(WaveGen me);

#endif




