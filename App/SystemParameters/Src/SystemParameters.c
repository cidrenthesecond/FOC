#include "SystemParameters.h"

static Motor_params_t          Motor_Parameters   = MOTOR_DEFAULT_CONFIG;
static Encoder_Parameters_t    Encoder_Parameters = ENCODER_DEFAULT_CONFIG;
static Drive_Parameters_t      Drive_Parameters   = DRIVE_DEFAULT_CONFIG;

Motor_params_t          GetMotorParameters()
{
    return Motor_Parameters;
}

Encoder_Parameters_t GetEncoderParameters()
{
    return Encoder_Parameters;
}
Drive_Parameters_t      GetDriveParameters()
{
    return Drive_Parameters;
}

