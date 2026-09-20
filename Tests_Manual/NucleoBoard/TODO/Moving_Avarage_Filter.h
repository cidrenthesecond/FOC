#ifndef MOVING_AVARAGE_FILTER
#define MOVING_AVARAGE_FILTER

#include "stdint.h"

typedef struct MovingAvarage_struct * MovingAvarage;

MovingAvarage MovingAvarage_Init(const uint8_t kernel);
uint16_t      MovingAvarage_Filter(MovingAvarage filter, uint16_t inputSample);
void          MovingAvarage_Reset(MovingAvarage filter);
void          MovingAvarage_Destroy(MovingAvarage filter);

#endif