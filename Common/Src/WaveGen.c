#include "WaveGen.h"
#include "SIN_LUT.h"
#include "stdlib.h"

typedef struct WaveGen_struct{
	const WaveGen_LUT * LUT;
	uint32_t phaseAccumulator;
	uint32_t phaseIncrement;
	uint8_t   phaseAccumulatorBitsToShift;
	float desiredFrequency;
	float samplingFrequency;
}WaveGen_struct;

WaveGen WaveGen_Create(const WaveGen_LUT *desiredWave)
{
	WaveGen result = malloc(sizeof(WaveGen_struct));
	result -> desiredFrequency = 0.0f;
	result -> samplingFrequency = 0.0f;
	result->phaseAccumulator = 0;
	result->LUT = desiredWave;

	uint32_t lutSize = desiredWave->size;
	uint8_t bitsUsed = 0;

	for(uint8_t timeout = 32; timeout > 0; timeout--)
	{
		if(lutSize == 1)
		{
			result -> phaseAccumulatorBitsToShift = 32 - bitsUsed;
			return result;
		}

		if(lutSize % 2 != 0)
			return NULL;
	
		lutSize = lutSize/2;
		bitsUsed++;
	}
	return NULL;
}

void WaveGen_SetFrequency(WaveGen w,float frequency,float samplingFrequency)
{
	w -> desiredFrequency = frequency;
	w -> samplingFrequency = samplingFrequency;
	w -> phaseIncrement = (uint32_t)((frequency/samplingFrequency) * (float)UINT32_MAX);
}

int32_t WaveGen_SetDesiredWave(WaveGen w,WaveGen_LUT *desiredWave)
{
	w -> LUT = desiredWave;
	return 1;
}

void WaveGen_Reset(WaveGen w)
{
	w -> phaseAccumulator = 0;
}

int32_t WaveGen_Get(WaveGen me)
{
	if(me->desiredFrequency == 0 || me ->samplingFrequency == 0)
		return 0;

	uint32_t index = (me->phaseAccumulator) >>(me -> phaseAccumulatorBitsToShift);
	int32_t result = me -> LUT -> data[index];
	me->phaseAccumulator +=  me->phaseIncrement;
	return result;
}

void WaveGen_Destroy(WaveGen me)
{
	free(me);
}




