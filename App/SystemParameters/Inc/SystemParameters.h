#ifndef SYSTEM_PARAMETERS_H
#define SYSTEM_PARAMETERS_H

#include "stdint.h"

typedef struct {
    uint16_t countsPerRevolution;
} Encoder_Parameters_t;

typedef struct {
    uint32_t ratedDeltaVoltage; //mV
    uint16_t ratedDeltaCurrent; //mA
    uint16_t ratedPower;        //Wats
    uint16_t ratedRPM;          //RPM
    uint8_t  ratedFrequency;    //hz
    uint8_t  poles;             //
    uint8_t  cosPhi;            //decimal
} Motor_params_t;

typedef struct {
    uint16_t switchingFrequency;
    uint16_t mainsVoltage;
    uint8_t  mainsFrequency;
} Drive_Parameters_t;

Motor_params_t          GetMotorParameters();
Encoder_Parameters_t GetEncoderParameters();
Drive_Parameters_t      GetDriveParameters();

#define ENCODER_DEFAULT_CONFIG { \
    .countsPerRevolution = 1024  \
}                                \

#define DRIVE_DEFAULT_CONFIG {  \
    .switchingFrequency = 1000, \
    .mainsVoltage       = 230,  \
    .mainsFrequency     = 50    \
}                               \

#define MOTOR_DEFAULT_CONFIG {   \
    .ratedDeltaVoltage = 230000, \
    .ratedDeltaCurrent = 2140    \
}                                \


#endif
