#include "WaveGen.h"
#include "stdlib.h"

enum {
	INVALID_VALUE = 0xFF
};

typedef struct WaveGen_struct{
	const WaveGen_LUT * LUT;
	uint32_t phaseAccumulator;
	uint32_t phaseIncrement;
	uint8_t   phaseAccumulatorBitsToShift;
	float desiredFrequency;
	float samplingFrequency;
}WaveGen_struct;

static uint8_t CalculateBitsToShift(const WaveGen_LUT *Wave);

WaveGen WaveGen_Create(const WaveGen_LUT *desiredWave)
{
	uint8_t bitsToShift = CalculateBitsToShift(desiredWave);

	if(bitsToShift == INVALID_VALUE)
		return NULL;

	WaveGen result = malloc(sizeof(WaveGen_struct));
	result -> LUT = desiredWave;
	result ->phaseAccumulatorBitsToShift = bitsToShift;
	result -> phaseAccumulator = 0;
	result -> phaseIncrement = 0;
	result -> desiredFrequency = 0.0f;
	result -> samplingFrequency = 0.0f;
	return result;
}

static uint8_t CalculateBitsToShift(const WaveGen_LUT *Wave)
{
	uint32_t lutSize = Wave->size;
	uint8_t bitsUsed = 0;

	for(uint8_t timeout = 32; timeout > 0; timeout--)
	{
		if(lutSize == 1)
			return 32 - bitsUsed;

		if(lutSize % 2 != 0)
			return INVALID_VALUE;
	
		lutSize = lutSize/2;
		bitsUsed++;
	}
	
	return INVALID_VALUE;
}

void WaveGen_SetFrequency(WaveGen w,float frequency,float samplingFrequency)
{
	w -> desiredFrequency = frequency;
	w -> samplingFrequency = samplingFrequency;
	w -> phaseIncrement = (uint32_t)((frequency/samplingFrequency) * (float)UINT32_MAX);
}

uint8_t WaveGen_SetDesiredWave(WaveGen w,const WaveGen_LUT *desiredWave)
{
	uint8_t bitsToShift = CalculateBitsToShift(desiredWave);

	if(bitsToShift == INVALID_VALUE)
		return INVALID_VALUE;

	w -> LUT = desiredWave;
	w -> phaseAccumulatorBitsToShift = bitsToShift;

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




