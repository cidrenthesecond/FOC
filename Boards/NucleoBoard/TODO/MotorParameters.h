#ifndef MOTOR_PARAMETERS_H
#define MOTOR_PARAMETERS_H

#include "stdint.h"

typedef struct {
    uint32_t ratedDeltaVoltage; //mV
    uint16_t ratedDeltaCurrent; //mA
    uint16_t ratedPower;        //Wats
    uint16_t ratedRPM;          //RPM
    uint8_t  ratedFrequency;    //hz
    uint8_t  poles;             //
    uint8_t  cosPhi;            //decimal
} motor_params_t;

motor_params_t GetMotorParameters();

#endif