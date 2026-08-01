/*
 * HybridVelocity.h
 *
 *  Created on: Mar 13, 2026
 *      Author: pawluczenko
 */

#ifndef INC_HYBRIDVELOCITY_H_
#define INC_HYBRIDVELOCITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

void HV_Start();
int32_t HV_CalculateVelocity();
void HV_Update_lastcapture(uint32_t cc_channelx);


#ifdef __cplusplus
}
#endif

#endif /* INC_HYBRIDVELOCITY_H_ */
