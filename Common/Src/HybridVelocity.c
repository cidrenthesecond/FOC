#include "main.h"

//TO DO :
//dodanie matematyki staloprzecinkowej dla optymalizacji
//dodac mozliwosc aktualizacji predkosci po wiecej jak jednym przeladniwaniu
//ISR update potrzebny
//HV_Update zmienic na inline


//Config Start
static const uint32_t     Clock_Freq               = 20000000;
static TIM_TypeDef* const capture_compare_timer    = TIM3;
static const uint32_t     capture_compare_channelA = LL_TIM_CHANNEL_CH1;
static const uint32_t     capture_compare_channelB = LL_TIM_CHANNEL_CH2; // | LL_TIM_CHANNEL_CH2

static TIM_TypeDef* Encoder_timer = TIM2;

static const uint8_t  Encoder_Poles            = 12;
static const uint8_t  Encoder_Edges_Counted    = 2;
static const uint8_t  Encoder_Channels_Counted = 2;

static const uint8_t timeout_cycles_goal = 0;
//Config End

//Private constants
static const uint16_t PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;
static const uint32_t measurement_factor  = 25000000;//60 * Clock_Freq / PulsesPerRevolution;

//Private variables
static volatile uint16_t lastcapture = 0;
static volatile uint16_t lastcapture_previous = 0;
static volatile uint8_t  timeout_cycles = 0;
static volatile uint16_t old_num_pulses = 0;
static volatile int32_t  prev_velocity = 0;
static volatile uint8_t  first_run = 1;

void HV_Start()
{
	LL_TIM_SetCounter(Encoder_timer, 0);
	LL_TIM_EnableCounter(Encoder_timer);

	LL_TIM_SetCounter(capture_compare_timer, 0);
	LL_TIM_CC_EnableChannel(capture_compare_timer, capture_compare_channelA);
	LL_TIM_EnableIT_CC1(capture_compare_timer);

	if(Encoder_Channels_Counted == 2)
	{
		LL_TIM_CC_EnableChannel(capture_compare_timer, capture_compare_channelB);
		LL_TIM_EnableIT_CC2(capture_compare_timer);
	}

	LL_TIM_EnableCounter(capture_compare_timer);
	LL_TIM_GenerateEvent_UPDATE(capture_compare_timer);
	LL_TIM_ClearFlag_UPDATE(capture_compare_timer);

	LL_TIM_EnableIT_UPDATE(capture_compare_timer);
}

static inline void HV_CaptureCompare_ISR()
{
	if(LL_TIM_IsActiveFlag_UPDATE(capture_compare_timer) && LL_TIM_IsEnabledIT_UPDATE(capture_compare_timer))
	{
		LL_TIM_ClearFlag_UPDATE(capture_compare_timer);
		//result = HV_CalculateVelocity();
	}
	if(LL_TIM_IsActiveFlag_CC1(capture_compare_timer))
	{
		LL_TIM_ClearFlag_CC1(capture_compare_timer);
		lastcapture = LL_TIM_IC_GetCaptureCH1(capture_compare_timer);
	}
	if(LL_TIM_IsActiveFlag_CC2(capture_compare_timer))
	{
		LL_TIM_ClearFlag_CC2(capture_compare_timer);
		lastcapture = LL_TIM_IC_GetCaptureCH2(capture_compare_timer);
	}
}

void HV_Update_lastcapture(uint32_t cc_channelx)
{
	if(cc_channelx == capture_compare_channelA) lastcapture = LL_TIM_IC_GetCaptureCH1(capture_compare_timer);
	else lastcapture = LL_TIM_IC_GetCaptureCH2(capture_compare_timer);
}



__attribute__((optimize("O3")))
int32_t HV_CalculateVelocity()
{
	//__disable_irq();
//	LL_TIM_DisableCounter(capture_compare_timer);
//	uint16_t ch1 = LL_TIM_IC_GetCaptureCH1(capture_compare_timer);
//	uint16_t ch2 = LL_TIM_IC_GetCaptureCH2(capture_compare_timer);
//	LL_TIM_EnableCounter(capture_compare_timer);

	const uint16_t capture = lastcapture;//(ch1 > ch2) ? ch1 : ch2;
	const uint16_t num_pulses = LL_TIM_GetCounter(Encoder_timer);
	//__enable_irq();

	int16_t delta = (int16_t)(num_pulses - old_num_pulses);

	if(first_run)
	{
		old_num_pulses = num_pulses;
		lastcapture_previous = capture;
		prev_velocity = 0;
		first_run = 0;
		return 0;
	}

	if(delta == 0)
	{
		timeout_cycles++;

		if(timeout_cycles >= timeout_cycles_goal)
		{
			prev_velocity = 0;
			timeout_cycles = timeout_cycles_goal;
			return 0;
		}

		return prev_velocity;
	}

	if(prev_velocity == 0) timeout_cycles = 0;

	uint32_t time_frame = 65536*timeout_cycles + (uint32_t)65536 - (uint32_t)lastcapture_previous + (uint32_t)capture;

	if(time_frame == 0) time_frame = 1;

	int32_t result = (int32_t)(((int64_t)delta * (int64_t)measurement_factor) / (int64_t)time_frame); // to dziala dobrze ale dlugo 4000ns
	//int32_t result = (int32_t)delta*(measurement_factor/time_frame); // to wprowadza blad ale krotko 2000ns

	lastcapture_previous = capture;
	prev_velocity = result;
	old_num_pulses = num_pulses;

	return result;
}
