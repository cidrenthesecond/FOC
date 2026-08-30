#ifndef SIGMOID_LUT_H
#define SIGMOID_LUT_H

#include <stdint.h>

/** Generated using Dr LUT - Free Lookup Table Generator
  * https://github.com/ppelikan/drlut
  **/
// Formula: 1/(1+pow(e,-(t-(T/2))/(T/20))) 
extern const int32_t sigmoid_lut[1024];

#endif