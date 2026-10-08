#include "FOC_Scheme_Test.h"
#include "arm_math.h"
#include "stdint.h"
#include "PWM_Schemes.h"
#include "Timer_Driver.h"

//Mocking rotor position
const float angle[36] = {
   0.0f,  10.0f,  20.0f,  30.0f,
  40.0f,  50.0f,  60.0f,  70.0f,
  80.0f,  90.0f,   100.0f, 110.0f,
 120.0f, 130.0f, 140.0f, 150.0f,
 160.0f, 170.0f, 180.0f, 190.0f,
 200.0f, 210.0f, 220.0f, 230.0f,
 240.0f, 250.0f, 260.0f, 270.0f,
 280.0f, 290.0f, 300.0f, 310.0f,
 320.0f, 330.0f, 340.0f, 350.0f };


static float powerSupplyVoltage = 48.0f;
static float VoltageSetpoint = 5.0f;

static float d_setpoint = 1.0f;
static float q_setpoint = 1.0f;

static uint8_t loop = 0;
static uint8_t testTaskReady = 0;

void GenerateVoltage_TaskEnable(void)
{
  testTaskReady = 1;
}

uint8_t GenerateVoltage_IsTaskReady(void)
{
	return testTaskReady;
}

void GenerateVoltage_Task(void)
{
  if(testTaskReady == 0)
    return;

	float sinus, cosinus;
	arm_sin_cos_f32(angle[loop], &sinus, &cosinus);

	float alpha = VoltageSetpoint * sinus;
	float beta  = VoltageSetpoint * cosinus;

	Duty_t duty = SPWM(alpha, beta, powerSupplyVoltage);
	PWM_SetDuty(duty.duty_a, duty.duty_b, duty.duty_c);

	loop = (loop + 1) % 36;
	testTaskReady = 0;
}

//After calling in equal intervals of time function returns
//Alfa and beta which are sines 90 deg of of phase
void OpenLoop_AlfaBeta(float * alpha, float *beta)
{
	float sinus;
	float cosinus;
	arm_sin_cos_f32(angle[loop], &sinus, &cosinus);
	arm_inv_park_f32(d_setpoint, q_setpoint, alpha, beta, sinus, cosinus);
	loop = (loop + 1) % 36;
}