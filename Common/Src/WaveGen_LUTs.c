#include "WaveGen_LUTs.h"
#include "SIN_LUT.h"
#include "SIGMOID_LUT.h"

const WaveGen_LUT SINE = {
    .data = sin_lut,
    .size  = 1024
};

const WaveGen_LUT SIGMOID = {
    .data = sigmoid_lut,
    .size = 1024
};