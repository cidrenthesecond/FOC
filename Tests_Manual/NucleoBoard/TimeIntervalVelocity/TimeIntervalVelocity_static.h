/*
 * PeriodMeas.h
 *
 *  Created on: Mar 5, 2026
 *      Author: 1
 */

#ifndef INC_TIMEINTERVALVELOCITY_STATIC_H_
#define INC_TIMEINTERVALVELOCITY_STATIC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

typedef enum {
	TIV_CHANNEL_A,
	TIV_CHANNEL_B
}TIV_Channel_t;

int32_t TIVs_CalculateVelocity(TIV_Channel_t channel);
int32_t TIVs_TimerOverflowISR();
void TIVs_Start();

#ifdef __cplusplus
}
#endif

#endif /* INC_TIMEINTERVALVELOCITY_STATIC_H_ */
