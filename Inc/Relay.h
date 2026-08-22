#ifndef RELAY_H
#define RELAY_H

#include <stdint.h>

enum{
    RELAY_OFF = 0,
    RELAY_ON = 1
};

void Relay_Init();
uint8_t Relay_IsOn();
void Relay_TurnOn();
void Relay_TurnOff();
void Relay_SM();
void Relay_SetThreshold(uint16_t threshold);

#endif