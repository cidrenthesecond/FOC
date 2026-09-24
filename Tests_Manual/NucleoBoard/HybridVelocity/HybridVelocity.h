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

void HV_Start();
int32_t HV_CalculateVelocity();
float HV_CalculateVelocity_float();
void HV_Update_lastcapture(uint32_t cc_channelx);
int32_t TIVs_Test();

#ifdef __cplusplus
}
#endif

#endif /* INC_HYBRIDVELOCITY_H_ */
