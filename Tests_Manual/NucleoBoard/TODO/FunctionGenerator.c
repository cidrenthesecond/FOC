#include "FunctionGenerator.h"

typedef struct Waveform_struct{
	uint32_t phaseAccumulator;
	uint32_t frequency;
	LUT_t    LUT_Used;
}Waveform_struct;

Waveform Waveform_Create();




