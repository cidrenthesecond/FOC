/*
 * FunctionGenerator.h
 *
 *  Created on: Aug 22, 2026
 *      Author: pawluczenko
 */

#ifndef FUNCTIONGENERATOR_H_
#define FUNCTIONGENERATOR_H_

//#include "SIN_LUT.h"
#include "stdint.h"

typedef enum {
	SINE
} WaveformUsed;

typedef struct LUT_t{
	uint32_t LUT_Size;
	void*    LUT;
}LUT_t;

//waveform
typedef struct Waveform_struct * Waveform;

Waveform Waveform_Create();
void*    Waveform_Get();


#endif /* FUNCTIONGENERATOR_H_ */




