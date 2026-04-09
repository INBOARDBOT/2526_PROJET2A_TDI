#include <FOC/foc_math.h>


void clark_transforn(float32_t Ia, float32_t Ic, float32_t *Ialpha, float32_t *Ibeta) {
    arm_clarke_f32(Ia, Ic, Ialpha, Ibeta);
}

void park_transforn(float32_t Ialpha, float32_t Ibeta, float32_t theta, float32_t *Id, float32_t *Iq) {
    float32_t sinVal, cosVal;

    sinVal = arm_sin_f32(theta);
    cosVal = arm_cos_f32(theta);

    arm_park_f32(Ialpha, Ibeta, Id, Iq, sinVal, cosVal);
}

void park_invtransforn(float32_t Ud, float32_t Uq, float32_t theta, float32_t *Ualpha, float32_t *Ubeta) {
    float32_t sinVal, cosVal;

    sinVal = arm_sin_f32(theta);
    cosVal = arm_cos_f32(theta);

    arm_inv_park_f32(Ud, Uq, Ualpha, Ubeta, sinVal, cosVal);
}


void svpwm(float valpha, float vbeta, float vbus, uint32_t pwm_period,
          uint32_t *pwm_u, uint32_t *pwm_v, uint32_t *pwm_w)
{
    // 1. Normalize voltages by vbus
    float alpha = valpha / vbus;
    float beta = vbeta / vbus;

    // 2. Sector determination
    uint8_t sector;
    if (beta >= 0.0f) {
        if (alpha >= 0.0f) {
            sector = (ONE_BY_SQRT3 * beta > alpha) ? 2 : 1;  // 1/sqrt(3) ≈ 0.577
        } else {
            sector = (-ONE_BY_SQRT3 * beta > alpha) ? 3 : 2;
        }
    } else {
        if (alpha >= 0.0f) {
            sector = (-ONE_BY_SQRT3 * beta > alpha) ? 5 : 6;
        } else {
            sector = (ONE_BY_SQRT3 * beta > alpha) ? 4 : 5;
        }
    }

    // 3. Calculate active vector times
    int32_t t1, t2;
    switch(sector) {
        case 1:
            t1 = (int32_t)((alpha - ONE_BY_SQRT3 * beta) * pwm_period);
            t2 = (int32_t)(TWO_BY_SQRT3 * beta * pwm_period);
            *pwm_u = (pwm_period + t1 + t2) / 2;
            *pwm_v = *pwm_u - t1;
            *pwm_w = *pwm_v - t2;
            break;

        case 2:
            t1 = (int32_t)((alpha + ONE_BY_SQRT3 * beta) * pwm_period);
            t2 = (int32_t)((-alpha + ONE_BY_SQRT3 * beta) * pwm_period);
            *pwm_v = (pwm_period + t1 + t2) / 2;
            *pwm_u = *pwm_v - t2;
            *pwm_w = *pwm_u - t1;
            break;

        case 3:
            t1 = (int32_t)(TWO_BY_SQRT3 * beta * pwm_period);
            t2 = (int32_t)((-alpha - ONE_BY_SQRT3 * beta) * pwm_period);
            *pwm_v = (pwm_period + t1 + t2) / 2;
            *pwm_w = *pwm_v - t1;
            *pwm_u = *pwm_w - t2;
            break;

        case 4:
            t1 = (int32_t)((-alpha + ONE_BY_SQRT3 * beta) * pwm_period);
            t2 = (int32_t)(-TWO_BY_SQRT3 * beta * pwm_period);
            *pwm_w = (pwm_period + t1 + t2) / 2;
            *pwm_v = *pwm_w - t2;
            *pwm_u = *pwm_v - t1;
            break;

        case 5:
            t1 = (int32_t)((-alpha - ONE_BY_SQRT3 * beta) * pwm_period);
            t2 = (int32_t)((alpha - ONE_BY_SQRT3 * beta) * pwm_period);
            *pwm_w = (pwm_period + t1 + t2) / 2;
            *pwm_u = *pwm_w - t1;
            *pwm_v = *pwm_u - t2;
            break;

        case 6:
            t1 = (int32_t)(-TWO_BY_SQRT3 * beta * pwm_period);
            t2 = (int32_t)((alpha + ONE_BY_SQRT3 * beta) * pwm_period);
            *pwm_u = (pwm_period + t1 + t2) / 2;
            *pwm_w = *pwm_u - t2;
            *pwm_v = *pwm_w - t1;
            break;
    }

    // 4. Clamp outputs to valid range
    *pwm_u = (*pwm_u > pwm_period) ? pwm_period : *pwm_u;
    *pwm_v = (*pwm_v > pwm_period) ? pwm_period : *pwm_v;
    *pwm_w = (*pwm_w > pwm_period) ? pwm_period : *pwm_w;
}

