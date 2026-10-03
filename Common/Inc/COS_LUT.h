#ifndef SIN_LUT_H
#define SIN_LUT_H

#include "stdint.h"

/** Generated using Dr LUT - Free Lookup Table Generator
  * https://github.com/ppelikan/drlut
  **/
// Formula: cos(2*pi*t/T)
extern const int32_t cos_lut[1024];

#endif
