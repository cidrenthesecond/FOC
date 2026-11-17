#include "DriveParameters.h"

static Motor_params_t          Motor_Parameters;
static Encoder_Parameters_t Encoder_Parameters = ENCODER_DEFAULT_CONFIG;
static Drive_Parameters_t      Drive_Parameters;

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

