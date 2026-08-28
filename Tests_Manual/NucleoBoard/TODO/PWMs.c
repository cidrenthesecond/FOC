#include <arm_math.h>
#include "stdint.h"

int



float findMax(float a, float b)
{
	return (a >= b) ? a : b;
}

float findMin(float a, float b)
{
	return (a <= b) ? a : b;
}

__attribute__((optimize("O3")))
void SVPWM(float alpha, float beta)
{
	float _alpha = alpha/3.3f;
	float _beta = beta/3.3f;


	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	float max = findMax(findMax(a,b),c);
	float min = findMin(findMin(a,b),c);

	float offset = (max + min) * 0.5f;

	a -= offset;
	b -= offset;
	c -= offset;

	TIM1 -> CCR1 = (uint32_t) ((a + 0.5f) * 65535);
	TIM1 -> CCR2 = (uint32_t) ((b + 0.5f) * 65535);
	TIM1 -> CCR3 = (uint32_t) ((c + 0.5f) * 65535);
}

__attribute__((optimize("O3")))
void SPWM(float alpha, float beta)
{
	float _alpha = alpha/3.3f;
	float _beta = beta/3.3f;

	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	TIM1 -> CCR1 = (uint32_t) ((a + 0.5f) * 65535);
	TIM1 -> CCR2 = (uint32_t) ((b + 0.5f) * 65535);
	TIM1 -> CCR3 = (uint32_t) ((c + 0.5f) * 65535);
}

__attribute__((optimize("O3")))
void DPWM_MAX(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    float max = findMax(findMax(a, b), c);

    /* Unconditional top-rail clamp of the max phase. This is what makes it
     * DPWMMAX (contrast with DPWM1/2, which flip between clamping max and
     * min depending on sign(max+min)). With this offset, the max phase's
     * duty comes out to exactly 1.0 automatically -- no manual override
     * needed. */
    float offset = 0.5f - max;

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- DPWMMAX has less linear-range headroom than DPWM1/2, so
     * this matters more here, not less. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

float abs_f(float x)
{
	return (x >= 0) ? x : -x;
}

__attribute__((optimize("O3")))
void DPWM1(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;
    float _beta  = beta  / 3.3f;

    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if (max + min >= 0.0f)
        offset = 0.5f - max;
    else
        offset = -0.5f - min;

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- guards against over-modulation AND against casting a
     * negative float to an unsigned CCR register (undefined behaviour). */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* actual timer period -- not a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);  /* +0.5f = round-to-nearest */
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

#define COSINE_MINUS_30   0.8660254f   /* cos(-30 deg) */
#define SINE_MINUS_30     0.5f         /* -sin(-30 deg) */

/* Rotate (alpha, beta) by -30 degrees. */
static inline void rotate_minus30(float alpha, float beta,
                                   float *alpha_r, float *beta_r)
{
    *alpha_r = COSINE_MINUS_30 * alpha + SINE_MINUS_30 * beta;
    *beta_r  = -SINE_MINUS_30   * alpha + COSINE_MINUS_30 * beta;
}

__attribute__((optimize("O3")))
void DPWM2(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    float alpha_v, beta_v, a_v, b_v, c_v;
    rotate_minus30(_alpha, _beta, &alpha_v, &beta_v);
    arm_inv_clarke_f32(alpha_v, beta_v, &a_v, &b_v);
    c_v = -a_v - b_v;

    float max_v = findMax(findMax(a_v, b_v), c_v);
    float min_v = findMin(findMin(a_v, b_v), c_v);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

#define COS_MINUS_60   0.5f   /* cos(-60 deg) */
#define SIN_MINUS_60   -0.8660254f         /* -sin(-60 deg) */

/* Rotate (alpha, beta) by -60 degrees. */
static inline void rotate_minus60(float alpha, float beta,
                                   float *alpha_r, float *beta_r)
{
    *alpha_r = COS_MINUS_60 * alpha + SIN_MINUS_60 * beta;
    *beta_r  = -SIN_MINUS_60   * alpha + COS_MINUS_60 * beta;
}

__attribute__((optimize("O3")))
void DPWM3(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    float alpha_v, beta_v, a_v, b_v, c_v;
    rotate_minus60(_alpha, _beta, &alpha_v, &beta_v);
    arm_inv_clarke_f32(alpha_v, beta_v, &a_v, &b_v);
    c_v = -a_v - b_v;

    float max_v = findMax(findMax(a_v, b_v), c_v);
    float min_v = findMin(findMin(a_v, b_v), c_v);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}

#define COS_30   0.8660254f   /* cos(30 deg) */
#define SIN_30   -0.5f         /* -sin(30 deg) */

/* Rotate (alpha, beta) by -60 degrees. */
static inline void rotate_30(float alpha, float beta,
                                   float *alpha_r, float *beta_r)
{
    *alpha_r = COS_30 * alpha + SIN_30 * beta;
    *beta_r  = -SIN_30   * alpha + COS_30 * beta;
}

__attribute__((optimize("O3")))
void DPWM0(float alpha, float beta)
{
    float _alpha = alpha / 3.3f;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = beta  / 3.3f;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    float alpha_v, beta_v, a_v, b_v, c_v;
    rotate_30(_alpha, _beta, &alpha_v, &beta_v);
    arm_inv_clarke_f32(alpha_v, beta_v, &a_v, &b_v);
    c_v = -a_v - b_v;

    float max_v = findMax(findMax(a_v, b_v), c_v);
    float min_v = findMin(findMin(a_v, b_v), c_v);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}


__attribute__((optimize("O3")))
void DPWM_MIN(float alpha, float beta)
{
	float _alpha = alpha/3.3f;
	float _beta = beta/3.3f;


	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	float min = findMin(findMin(a,b),c);

	float offset = -0.5f - min;

    float dutyA = a + offset + 0.5f;
    float dutyB = b + offset + 0.5f;
    float dutyC = c + offset + 0.5f;

    /* Saturate -- DPWMMAX has less linear-range headroom than DPWM1/2, so
     * this matters more here, not less. */
    dutyA = (dutyA < 0.0f) ? 0.0f : (dutyA > 1.0f ? 1.0f : dutyA);
    dutyB = (dutyB < 0.0f) ? 0.0f : (dutyB > 1.0f ? 1.0f : dutyB);
    dutyC = (dutyC < 0.0f) ? 0.0f : (dutyC > 1.0f ? 1.0f : dutyC);

    uint32_t arr = TIM1->ARR;   /* real timer period, never a magic number */

    TIM1->CCR1 = (uint32_t)(dutyA * (float)arr + 0.5f);
    TIM1->CCR2 = (uint32_t)(dutyB * (float)arr + 0.5f);
    TIM1->CCR3 = (uint32_t)(dutyC * (float)arr + 0.5f);
}