#include "WaveGen.h"
#include "SIN_LUT.h"
#include "stdlib.h"

typedef struct WaveGen_struct{
	uint32_t phaseAccumulator;
	uint32_t phaseIncrement;
	uint8_t   phaseAccumulatorBitsUsed;
	float desiredFrequency;
	float samplingFrequency;
}WaveGen_struct;

WaveGen WaveGen_Create()
{
	WaveGen result = malloc(sizeof(WaveGen_struct));
	result->phaseIncrement = (uint32_t)((50.0f / 20000.0f) * 4294967296.0f); //frequency / samplingFrequency
	result->phaseAccumulator = 0;
	return result;
}

void WaveGen_SetFrequency(WaveGen w,float frequency,float samplingFrequency)
{
	w -> desiredFrequency = frequency;
	w -> samplingFrequency = samplingFrequency;
	w -> phaseIncrement = (uint32_t)((frequency/samplingFrequency) * 4294967296.0f);
}

void WaveGen_Reset(WaveGen w)
{
	w -> phaseAccumulator = 0;
}

int32_t WaveGen_Get(WaveGen me)
{
	uint32_t index = (me->phaseAccumulator) >>(32-10);
	int32_t result = lut[index];
	me->phaseAccumulator +=  me->phaseIncrement;
	return result;
}

void WaveGen_Destroy(WaveGen me)
{
	free(me);
}




