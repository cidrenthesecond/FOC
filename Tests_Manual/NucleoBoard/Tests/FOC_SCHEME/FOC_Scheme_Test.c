#include "FOC_Scheme_Test.h"

float d_setpoint = 1.0f;//1.0f;
float q_setpoint = 1.0f;


//Watch Variables
float alpha = 0.0f;
float beta = 0.0f;
int8_t loop = 0;

//Mocking rotor position
const float_t angle[36] = {
 0.0, 10.0, 20.0, 30.0,
 40.0, 50.0, 60.0, 70.0,
 80.0, 90.0, 100.0, 110.0,
 120.0, 130.0, 140.0, 150.0,
 160.0, 170.0, 180.0, 190.0,
 200.0, 210.0, 220.0, 230.0,
 240.0, 250.0, 260.0, 270.0,
 280.0, 290.0, 300.0, 310.0,
 320.0, 330.0, 340.0, 350.0 };

//After calling in equal intervals of time function returns
//Alfa and beta which are sines 90 deg of of phase
__attribute__((optimize("O3")))
void OpenLoop_AlfaBeta(float * alpha, float *beta)
{
	float sinus;
	float cosinus;
	arm_sin_cos_f32(angle[loop], &sinus, &cosinus);
	arm_inv_park_f32(d_setpoint, q_setpoint, alpha, beta, sinus, cosinus);
	loop = (loop + 1) % 36;
}