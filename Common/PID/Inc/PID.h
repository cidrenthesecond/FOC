#ifndef PID_B_EULER_H
#define PID_B_EULER_H

#include <stdint.h>

typedef struct PID PID;

PID* PID_Create();
void PID_Destroy(PID *this);
void PID_Init(PID *this);
void PID_Reset(PID *this);
float PID_Compute(PID *this,float error);

void PID_SetKp(PID *this,float _Kp);
void PID_SetKd(PID *this,float _Kd);
void PID_SetKi(PID *this,float _Ki);
void PID_SetSaturation(PID *this,float _saturation);
void PID_DeactivateSaturation(PID *this);
float PID_GetKp(PID *this);
float PID_GetKi(PID *this);
float PID_GetKd(PID *this);
float PID_GetSaturation(PID *this);



#endif