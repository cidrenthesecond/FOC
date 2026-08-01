#ifndef FAKE_DCLINK_MEASUREMENT_H
#define FAKE_DCLINK_MEASUREMENT_H

#include <stdint.h>
#include "ADC_Service.h"

void Fake_SetDcLinkVoltage(uint16_t voltage);
void Fake_ResetDcLinkVoltage();

//uint16_t Fake_GetDcLinkVoltage();

#endif