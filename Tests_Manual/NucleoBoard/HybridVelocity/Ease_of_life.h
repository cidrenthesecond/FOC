/*
 * Ease_of_life.h
 *
 *  Created on: Mar 11, 2026
 *      Author: 1
 */

#ifndef INC_EASE_OF_LIFE_H_
#define INC_EASE_OF_LIFE_H_

#ifdef __cplusplus
extern "C" {
#endif

void LL_TIM_EnableIT_CC(TIM_TypeDef *TIMX,uint32_t TIMX_Channelx);

static inline uint8_t LL_GPIO_GetState(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	return (READ_BIT(GPIOx->IDR, PinMask) == PinMask);
}

#ifdef __cplusplus
}
#endif

#endif /* INC_EASE_OF_LIFE_H_ */
