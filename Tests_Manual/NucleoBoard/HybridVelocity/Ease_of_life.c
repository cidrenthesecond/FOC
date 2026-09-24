/*
 * Ease_of_life.c
 *
 *  Created on: Mar 11, 2026
 *      Author: 1
 */
#include "main.h"
#include "Ease_of_life.h"

void LL_TIM_EnableIT_CC(TIM_TypeDef *TIMX,uint32_t TIMX_Channelx)
{
	if(TIMX_Channelx == LL_TIM_CHANNEL_CH1) LL_TIM_EnableIT_CC1(TIMX);
	if(TIMX_Channelx == LL_TIM_CHANNEL_CH2) LL_TIM_EnableIT_CC2(TIMX);
	if(TIMX_Channelx == LL_TIM_CHANNEL_CH3) LL_TIM_EnableIT_CC3(TIMX);
	if(TIMX_Channelx == LL_TIM_CHANNEL_CH4) LL_TIM_EnableIT_CC4(TIMX);
}
