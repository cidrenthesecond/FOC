#include "PID.h"
#include <stdlib.h>

enum{
    SATURATION_NOT_USED = 0,
    SATURATION_USED = 1
};

static inline float LimitOutput(PID *this,float PID_output);
static inline void CalculateWindupCorrection(PID *this,float PID_output,float saturationOutput);

struct PID
{   
    float Kp;
    float Ki;
    float Kd;
    float Ts;

    float integral;
    float previousError;
    float saturation;
    float windupGain;
    float windupCorrection;

    uint8_t isSaturationUsed;
    int8_t saturationSign;
};

PID* PID_Create()
{
    PID *this = malloc(sizeof(PID));
    
    return this;
}

void PID_Destroy(PID *this)
{
    free(this);
}

void PID_Init(PID *this)
{
    this -> Kp = 1;
    this -> Ki = 0;
    this -> Kd = 0;
    this -> Ts = 0.1f;
    this -> integral = 0;
    this -> previousError = 0.0f;
    this -> isSaturationUsed = SATURATION_NOT_USED;
    this -> saturation = -1;
    this -> saturationSign = 0;
    this -> windupGain = 1.0;
    this -> windupCorrection = 0.0;

}

void PID_Reset(PID *this)
{
    this -> integral = 0;
    this -> previousError = 0;
    this -> windupCorrection = 0.0;
}

//TO DO: Readability
float PID_Compute(PID *this,float error)
{   
    float proportionalPart = error*(this->Kp);
    float derivativePart = (this -> Kd) * (error - (this->previousError)) / (this -> Ts);

    if(this -> isSaturationUsed)
        this -> integral = (error - (this->windupCorrection))*(this -> Ki)*(this -> Ts);
    else
        (this -> integral) += error*(this -> Ki)*(this -> Ts);


    float sum = proportionalPart + (this ->integral) + derivativePart;

    float result = LimitOutput(this,sum);

    CalculateWindupCorrection(this,sum,result);
    this -> previousError = error;

    return result;
}

static inline float LimitOutput(PID *this,float PID_output)
{
    if(!(this->isSaturationUsed))
        return PID_output;

    if(PID_output > (this->saturation))
        return (this->saturation);

    if(PID_output < - (this->saturation))
        return - (this->saturation);

    return PID_output;
}

static inline void CalculateWindupCorrection(PID *this,float PID_output,float saturationOutput)
{
    this -> windupCorrection = (PID_output - saturationOutput)*(this -> windupGain);
}

//----------------------------
//SETERS and GETERS
//----------------------------
void PID_SetKp(PID *this,float _Kp)
{   
    if(_Kp < 0)
    {
        //LOG(PID KP CANT BE NEGATIVE)
        return;
    }
    this -> Kp = _Kp;
}

void PID_SetKd(PID *this,float _Kd)
{   
    if(_Kd < 0)
    {
        //LOG(PID Kd CANT BE NEGATIVE)
        return;
    }
    this -> Kd = _Kd;
}

void PID_SetKi(PID *this,float _Ki)
{   
    if(_Ki < 0)
    {
        //LOG(PID KP CANT BE NEGATIVE)
        return;
    }
    this -> Ki = _Ki;
}

void PID_SetSaturation(PID *this,float _saturation)
{   
    if(_saturation < 0)
        return;

    this -> saturation = _saturation;
    this -> isSaturationUsed = SATURATION_USED;
}

void PID_DeactivateSaturation(PID *this)
{
    this -> saturation = -1.0f;
    this -> isSaturationUsed = SATURATION_NOT_USED;
}

float PID_GetKp(PID *this)
{
    return this -> Kp;
}

float PID_GetKd(PID *this)
{
    return this -> Kd;
}

float PID_GetKi(PID *this)
{
    return this -> Ki;
}

float PID_GetSaturation(PID *this)
{   
    return this -> saturation;
}