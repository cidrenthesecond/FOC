#ifndef GPIO_MOCK_H
#define GPIO_MOCK_H

#include "GPIO_Driver.h"

enum {
    GPIO_STATE_OFF       = 0x00,
    GPIO_STATE_ON        = 0x01,
    GPIO_STATE_UNDEFINED = 0xff
};

void GPIO_Mock_init();

#endif