/*
 * PulseMeas_static.h
 *
 *  Created on: Mar 6, 2026
 *      Author: pawluczenko
 */


#ifndef INC_PULSECOUNTVELOCITY_STATIC_H_
#define INC_PULSECOUNTVELOCITY_STATIC_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t PCVs_CalculateVelocity();
int32_t PCVs_CalculateVelocity1();
void PCVs_Start();

#ifdef __cplusplus
}
#endif

#endif /* INC_PULSECOUNTVELOCITY_STATIC_H_ */
