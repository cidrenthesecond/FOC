/*
 * PulseMeas_static.h
 *
 *  Created on: Mar 6, 2026
 *      Author: pawluczenko
 */


#ifndef INC_PULSECOUNTVELOCITY_STATIC_H_
#define INC_PULSECOUNTVELOCITY_STATIC_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

//Interface for hardware init
void PCV_HardwareInit(uint32_t measurementFrequency);

void PCV_Init(uint32_t measurementFrequency);
int32_t PCVs_CalculateVelocity();
int32_t PCVs_CalculateVelocity1();
void PCVs_Start();
void PCV_Stop();

#ifdef __cplusplus
}
#endif

#endif /* INC_PULSECOUNTVELOCITY_STATIC_H_ */
