#ifndef PWM_SCHEMES_H
#define PWM_SCHEMES_H

typedef struct {
    float duty_a;
    float duty_b;
    float duty_c;
} Duty_t;

Duty_t SVPWM    (float v_alpha, float v_beta, float v_bus);
Duty_t SPWM     (float v_alpha, float v_beta, float v_bus);
Duty_t DPWM_MAX (float v_alpha, float v_beta, float v_bus);
Duty_t DPWM_MIN (float v_alpha, float v_beta, float v_bus);
Duty_t DPWM1    (float v_alpha, float v_beta, float v_bus);
Duty_t DPWM2    (float v_alpha, float v_beta, float v_bus);
Duty_t DPWM3    (float v_alpha, float v_beta, float v_bus);
Duty_t DPWM0    (float v_alpha, float v_beta, float v_bus);

#endif