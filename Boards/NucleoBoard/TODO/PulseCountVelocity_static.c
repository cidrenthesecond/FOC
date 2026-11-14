#include "SpeedFeedback.h"
#include <PulseCountVelocity_static.h>
#include <stdint.h>

#define FULL_ANGLE_DECY_DEGREES 3600

//CONFIG START
static const uint8_t Encoder_Poles           	 = 12;


static const uint8_t Encoder_Edges_Counted   	 = 2;
static const uint8_t Encoder_Channels_Counted	 = 2;
static const uint32_t Measurement_Frequency      = 1000;

//CONFIG END

static uint32_t Encoder_PulsesPerRevolution; 
static uint32_t Measurement_Factor;          
static uint32_t old_num_pulses;

static int32_t CalculateVelocity(uint16_t num_pulses);
static uint32_t CalculateAngle(uint16_t num_pulses);

void Encoder_Start()
{
	Encoder_PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;
	Measurement_Factor          = (60*Measurement_Frequency)/Encoder_PulsesPerRevolution;
	old_num_pulses              = 0;
	//sprzet init
}

Speed_Feedback_t Encoder_GetFeedback(uint16_t num_pulses)
{
	Speed_Feedback_t result;
	result.velocity = CalculateVelocity(num_pulses);
	result.angle    = CalculateAngle(num_pulses);
	return result;
}

static int32_t CalculateVelocity(uint16_t num_pulses)
{
	int32_t delta = (int32_t)num_pulses - (int32_t)old_num_pulses;
	if(delta == 0) return 0;

	old_num_pulses = num_pulses;
	return delta*(int32_t)Measurement_Factor;
}

static uint32_t CalculateAngle(uint16_t num_pulses)
{
	uint32_t counts = (uint32_t)num_pulses % Encoder_PulsesPerRevolution;
	return counts * FULL_ANGLE_DECY_DEGREES / Encoder_PulsesPerRevolution;
}

void Encoder_Stop()
{
	//sprzet deinit
}
