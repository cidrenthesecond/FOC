#ifndef THERMISTOR_LUT_H
#define THERMISTOR_LUT_H

#include "stdint.h"

#define NTC_LUT_ENTRIES          26
#define NTC_LUT_TEMPERATURE_STEP 50

typedef struct NTC_Point
{
    uint16_t NTC_Voltage;
    int16_t temperature;
}NTC_Point;

static const NTC_Point NTC_LUT[NTC_LUT_ENTRIES] ={
{66  ,-300},
{89  ,-250},
{118 ,-200},
{155 ,-150},
{202 ,-100},
{259 ,-50 },
{323 , 0  },
{410 , 50 },
{506 , 100},
{616 , 150},
{738 , 200},
{874 , 250},
{1019, 300},
{1173, 350},
{1331, 400},
{1491, 450},
{1649, 500},
{1804, 550},
{1951, 600},
{2090, 650},
{2220, 700},
{2338, 750},
{2445, 800},
{2543, 850},
{2630, 900},
{2707, 950}
};
#endif