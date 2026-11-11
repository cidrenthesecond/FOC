#ifndef SCALAR_PROFILE_H
#define SCALAR_PROFILE_H

void ScalarProfile_Init(float set_ratedVoltage, float set_boostVoltage,float set_ratedFrequency, float set_boostFrequency);
float ScalarProfile_GetVoltage(float frequency);
void ScalarProfile_Destroy();

#endif