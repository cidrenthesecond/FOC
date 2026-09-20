#include "PWM_Schemes.h"
#include <arm_math.h>
#include "stdint.h"

typedef enum {
    MINUS_30_DEGREES,
    MINUS_60_DEGREES,
    PLUS_30_DEGREES,
} Angle_t;

typedef struct {
    float alpha_prim;
    float beta_prim;
} RotatedVector_t;

#define COS_MINUS_30   0.8660254f      /* cos(-30 deg) */
#define SIN_MINUS_30   (-0.5f)         /* sin(-30 deg) */
#define COS_MINUS_60   0.5f            /* cos(-60 deg) */
#define SIN_MINUS_60   (-0.8660254f)   /* sin(-60 deg) */
#define COS_30         0.8660254f      /* cos(30 deg) */
#define SIN_30         0.5f            /* sin(30 deg) */

static inline float findMax(float a, float b)
{
	return (a >= b) ? a : b;
}
static inline float findMin(float a, float b)
{
	return (a <= b) ? a : b;
}

static inline float GetSinValue(Angle_t angle)
{
    float result;

    switch (angle) {

        case MINUS_30_DEGREES:
           result = SIN_MINUS_30;
           break;

        case MINUS_60_DEGREES:
            result = SIN_MINUS_60;
            break;

        case PLUS_30_DEGREES:
            result = SIN_30;
            break;
    }

    return result;
}

static inline float GetCosValue(Angle_t angle)
{
    float result;

    switch (angle) {

        case MINUS_30_DEGREES:
            result = COS_MINUS_30;
            break;
        
        case MINUS_60_DEGREES:
            result = COS_MINUS_60;
            break;

        case PLUS_30_DEGREES:
            result = SIN_30;
            break;
    }
    
    return result;
}

static inline RotatedVector_t RotateVector(float a, float b, Angle_t angle)
{
    float sin = GetSinValue(angle);
    float cos = GetCosValue(angle);

    RotatedVector_t result;
    result.alpha_prim = (cos*a) - (sin * b);
    result.beta_prim  = (sin*a) + (cos * b);
    return result;
}

static inline Duty_t Saturate(Duty_t d)
{
    d.duty_a = (d.duty_a < 0.0f) ? 0.0f : (d.duty_a > 1.0f ? 1.0f : d.duty_a);
    d.duty_b = (d.duty_b < 0.0f) ? 0.0f : (d.duty_b > 1.0f ? 1.0f : d.duty_c);
    d.duty_c = (d.duty_c < 0.0f) ? 0.0f : (d.duty_c > 1.0f ? 1.0f : d.duty_c);
    return d;
}

Duty_t SVPWM(float v_alpha, float v_beta, float v_bus)
{
	float _alpha = v_alpha/ v_bus;
	float _beta = v_beta  / v_bus;

	float a,b,c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	float max = findMax(findMax(a,b),c);
	float min = findMin(findMin(a,b),c);

	float offset = (max + min) * 0.5f;

    Duty_t result;
    result.duty_a = a - offset;
    result.duty_b = b - offset;
    result.duty_c = c - offset;

    return Saturate(result);
}

Duty_t SPWM(float v_alpha, float v_beta, float v_bus)
{
	float _alpha = v_alpha/v_bus;
	float _beta = v_beta/v_bus;

	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

    Duty_t result;
    result.duty_a = a;
    result.duty_b = b;
    result.duty_c = c;

    return Saturate(result);
}

Duty_t DPWM_MAX(float v_alpha, float v_beta, float v_bus)
{
    float _alpha = v_alpha / v_bus;   /* Vdc = actual DC bus voltage, not ADC Vref */
    float _beta  = v_beta  / v_bus;

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

    Duty_t result;
    result.duty_a = a + offset + 0.5f;
    result.duty_b = b + offset + 0.5f;
    result.duty_c = c + offset + 0.5f;

    /* Saturate -- DPWMMAX has less linear-range headroom than DPWM1/2, so
     * this matters more here, not less. */
    return Saturate(result);
}

Duty_t DPWM_MIN(float v_alpha, float v_beta, float v_bus)
{
	float _alpha = v_alpha/v_bus;
	float _beta  = v_beta /v_bus;

	float a;
	float b;
	float c;

	arm_inv_clarke_f32(_alpha, _beta, &a, &b);
	c = -a -b;

	float min = findMin(findMin(a,b),c);

	float offset = -0.5f - min;

    Duty_t result;
    result.duty_a = a + offset + 0.5f;
    result.duty_b = b + offset + 0.5f;
    result.duty_c = c + offset + 0.5f;

    /* Saturate -- DPWMMAX has less linear-range headroom than DPWM1/2, so
     * this matters more here, not less. */
    return Saturate(result);
}

Duty_t DPWM1(float v_alpha, float v_beta, float v_bus)
{
    float _alpha = v_alpha / v_bus;
    float _beta  = v_beta  / v_bus;

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

    Duty_t result;
    result.duty_a = a + offset + 0.5f;
    result.duty_b = b + offset + 0.5f;
    result.duty_c = c + offset + 0.5f;

    /* Saturate -- guards against over-modulation AND against casting a
     * negative float to an unsigned CCR register (undefined behaviour). */
    return Saturate(result);
}

Duty_t DPWM2(float v_alpha, float v_beta, float v_bus)
{
    float _alpha = v_alpha / v_bus;   
    float _beta  = v_beta  / v_bus;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    RotatedVector_t rotatedVector;
    rotatedVector = RotateVector(v_alpha, v_beta, MINUS_30_DEGREES);

    float a_prim, b_prim, c_prim;
    arm_inv_clarke_f32(rotatedVector.alpha_prim, rotatedVector.beta_prim, &a_prim, &b_prim);
    c_prim = -a_prim - b_prim;

    float max_v = findMax(findMax(a_prim, b_prim), c_prim);
    float min_v = findMin(findMin(a_prim, b_prim), c_prim);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    Duty_t result;
    result.duty_a = a + offset + 0.5f;
    result.duty_b = b + offset + 0.5f;
    result.duty_c = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    return Saturate(result);
}

Duty_t DPWM3(float v_alpha, float v_beta, float v_bus)
{
    float _alpha = v_alpha / v_bus;
    float _beta  = v_beta  / v_bus;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    RotatedVector_t rotatedVector;
    rotatedVector = RotateVector(v_alpha, v_beta, MINUS_60_DEGREES);

    float a_prim, b_prim, c_prim;
    arm_inv_clarke_f32(rotatedVector.alpha_prim, rotatedVector.beta_prim, &a_prim, &b_prim);
    c_prim = -a_prim - b_prim;

    float max_v = findMax(findMax(a_prim, b_prim), c_prim);
    float min_v = findMin(findMin(a_prim, b_prim), c_prim);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    Duty_t result;
    result.duty_a = a + offset + 0.5f;
    result.duty_b = b + offset + 0.5f;
    result.duty_c = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    return Saturate(result);
}

Duty_t DPWM0(float v_alpha, float v_beta, float v_bus)
{
    float _alpha = v_alpha / v_bus;   
    float _beta  = v_alpha / v_bus;

    /* --- Real phase voltages: used for the actual PWM output. --- */
    float a, b, c;
    arm_inv_clarke_f32(_alpha, _beta, &a, &b);
    c = -a - b;

    /* --- Rotated (virtual) phase voltages: used ONLY to pick the clamp
     *     direction. This -30 deg shift is what turns DPWM1 into DPWM2. --- */
    RotatedVector_t rotatedVector;
    rotatedVector = RotateVector(v_alpha,v_beta, PLUS_30_DEGREES);

    float a_prim, b_prim, c_prim;
    arm_inv_clarke_f32(rotatedVector.alpha_prim, rotatedVector.beta_prim, &a_prim, &b_prim);
    c_prim = -a_prim - b_prim;
    float max_v = findMax(findMax(a_prim, b_prim), c_prim);
    float min_v = findMin(findMin(a_prim, b_prim), c_prim);

    float max = findMax(findMax(a, b), c);
    float min = findMin(findMin(a, b), c);

    float offset;
    if ((max_v + min_v) >= 0.0f)
        offset = 0.5f - max;     /* clamp the REAL max phase to the top rail */
    else
        offset = -0.5f - min;    /* clamp the REAL min phase to the bottom rail */

    Duty_t result;
    result.duty_a = a + offset + 0.5f;
    result.duty_b = b + offset + 0.5f;
    result.duty_c = c + offset + 0.5f;

    /* Saturate -- same reasoning as the DPWM1 fix: guards against
     * over-modulation and undefined behaviour on the float->uint cast. */
    return Saturate(result);
}