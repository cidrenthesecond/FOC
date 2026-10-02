#include "PulseCountVelocity.h"
#include <stdint.h>
#include "stm32h5xx_ll_tim.h"

#define ENCODER_EDGES_COUNTED    2
#define ENCODER_CHANNELS_COUNTED 2

//CONFIG START
static const uint8_t Encoder_Poles           	 = 12;
static const uint8_t Encoder_Edges_Counted   	 = 2;
static const uint8_t Encoder_Channels_Counted	 = 2;

static const uint32_t Measurement_Frequency      = 305;

static TIM_TypeDef* const Encoder_Timer          = TIM2;
static TIM_TypeDef* const MeasurementFrame_Timer = TIM3;

#define timeout_cycles_goal                        10

//CONFIG END

//Private functions
static inline void PCVs_EncoderTimer_Start();
static inline void PCVs_MeasFrameTimer_Start();

static const uint16_t Encoder_PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;
static uint32_t       Measurement_Factor          = (60*Measurement_Frequency)/Encoder_PulsesPerRevolution;

static volatile uint16_t old_num_pulses	= 0;
static volatile uint8_t timeout_cycles  = 0;
static volatile int32_t prev_velocity   = 0;

void PCV_Init(uint32_t measurementFrequency)
{
	PCV_HardwareInit(measurementFrequency);
}


void PCVs_Start()
{
	PCVs_EncoderTimer_Start();
	PCVs_MeasFrameTimer_Start();
	old_num_pulses = 0;
}

static inline void PCVs_EncoderTimer_Start()
{
	LL_TIM_SetCounter(Encoder_Timer, 0);
	LL_TIM_EnableCounter(Encoder_Timer);
}

static inline void PCVs_MeasFrameTimer_Start()
{
	LL_TIM_EnableCounter(MeasurementFrame_Timer);

	LL_TIM_GenerateEvent_UPDATE(MeasurementFrame_Timer);
	LL_TIM_ClearFlag_UPDATE(MeasurementFrame_Timer);

	LL_TIM_EnableIT_UPDATE(MeasurementFrame_Timer);
}

int32_t PCVs_CalculateVelocity()
{
	uint16_t num_pulses = LL_TIM_GetCounter(Encoder_Timer);

	int16_t delta = (int16_t)(num_pulses - old_num_pulses);

	if(delta == 0) return 0;

	int32_t result = (int32_t)delta*(int32_t)Measurement_Factor;

	prev_velocity = result;
	old_num_pulses = num_pulses;

	return result;
}

int32_t PCVs_CalculateVelocity1()
{
	uint16_t num_pulses = LL_TIM_GetCounter(Encoder_Timer);
	int16_t delta = (int16_t)(num_pulses - old_num_pulses);

	if(delta == 0)
	{
		if(++timeout_cycles >= timeout_cycles_goal)
		{
			prev_velocity = 0;
			timeout_cycles = 0;
			return 0;
		}
		return prev_velocity;
	}

	int32_t result;

	result = (int32_t)delta*UINT16_MAX;

	prev_velocity = result;
	old_num_pulses = num_pulses;
	timeout_cycles = 0;

	return result;
}


