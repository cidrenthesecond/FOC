/*
 * PulseMeas_static.h
 *
 *  Created on: Mar 6, 2026
 *      Author: pawluczenko
 */


#ifndef INC_PULSECOUNTVELOCITY_STATIC_H_
#define INC_PULSECOUNTVELOCITY_STATIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"


int32_t PCV_CalculateVelocity(uint16_t num_pulses);

#ifdef __cplusplus
}
#endif

#endif /* INC_PULSECOUNTVELOCITY_STATIC_H_ */
