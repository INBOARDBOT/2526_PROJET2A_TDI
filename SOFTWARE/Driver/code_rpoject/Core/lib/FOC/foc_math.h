#ifndef FOC_MATH_H
#define FOC_MATH_H

#include "arm_math.h"


#define ONE_BY_SQRT3  0.577350269f
#define TWO_BY_SQRT3  1.154700538f
#define TWO_PI        6.283185307f
#define SQRT3         1.73205081f
#define RAD_TO_DEG_F  57.2957795f

#define CLAMP(x, min, max) ((x)<(min)?(min):((x)>(max)?(max):(x)))


void clark_transforn(float32_t Ia, float32_t Ic, float32_t *Ialpha, float32_t *Ibeta);
void park_transforn(float32_t Ialpha, float32_t Ibeta, float32_t theta, float32_t *Id, float32_t *Iq);
void park_invtransforn(float32_t Ud, float32_t Uq, float32_t theta, float32_t *Ualpha, float32_t *Ubeta);
void svpwm(float32_t Ualpha, float32_t Ubeta, float32_t vbus, uint32_t pwm_period,
                     uint32_t *pwm_u, uint32_t *pwm_v, uint32_t *pwm_w);
void calculate_inductive_Q(void);
void calculate_inductive_D(void);
void calculate_resistive_D(void);

#endif
