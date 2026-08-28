#ifndef WAVEGEN_H
#define WAVEGEN_H

#include "stdint.h"

typedef struct WaveGen_struct * WaveGen;

WaveGen WaveGen_Create();
int32_t      WaveGen_Get(WaveGen me);
void          WaveGen_Reset(WaveGen w);
void          WaveGen_SetFrequency(WaveGen w,float frequency,float samplingFrequency);
void          WaveGen_Destroy(WaveGen me);

#endif




