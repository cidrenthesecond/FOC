#ifndef THERMISTOR_H
#define THERMISTOR_H

#include "stdint.h"

enum {
    NTC_TOO_HIGH_VOLTAGE = INT16_MAX,
    NTC_TOO_LOW_VOLTAGE  = INT16_MIN
};

int16_t Thermistor_GetHeatsinkTemp();


#endif