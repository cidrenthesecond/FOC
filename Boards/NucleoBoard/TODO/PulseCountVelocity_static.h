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
#include "SpeedFeedback.h"

void             Encoder_Start();
Speed_Feedback_t Encoder_GetFeedback(uint16_t num_pulses);
void             Encoder_Stop();

#ifdef __cplusplus
}
#endif

#endif /* INC_PULSECOUNTVELOCITY_STATIC_H_ */
