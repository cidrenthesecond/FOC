/*
 * PulseMeas_static.c
 *
 *  Created on: Mar 6, 2026
 *      Author: pawluczenko
 */


#include <PulseCountVelocity_static.h>

//CONFIG START
static const uint8_t Encoder_Poles           	 = 12;
static const uint8_t Encoder_Edges_Counted   	 = 2;
static const uint8_t Encoder_Channels_Counted	 = 2;

static const uint32_t Measurement_Frequency      = 305;

//CONFIG END

static uint16_t Encoder_PulsesPerRevolution; 
static uint32_t Measurement_Factor;          

static volatile uint16_t old_num_pulses	= 0;
static volatile int32_t  prev_velocity  = 0;


void PCV_Start()
{
	Encoder_PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;
	Measurement_Factor          = (60*Measurement_Frequency)/Encoder_PulsesPerRevolution;
	old_num_pulses              = 0;
	prev_velocity               = 0; 
}

__attribute__((optimize("O3")))
int32_t PCV_CalculateVelocity(uint16_t num_pulses)
{
	int16_t delta = (int16_t)(num_pulses - old_num_pulses);

	if(delta == 0) return 0;

	int32_t result = (int32_t)delta*(int32_t)Measurement_Factor;

	prev_velocity = result;
	old_num_pulses = num_pulses;

	return result;
}


