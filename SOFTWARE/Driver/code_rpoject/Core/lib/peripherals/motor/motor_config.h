#ifndef MOTOR_CONFIG_H
#define MOTOR_CONFIG_H

#include "arm_math.h"

#define FLAG_INIT    (1 << 0)
#define FLAG_START   (1 << 1)
#define FLAG_CALBTN  (1 << 2)
#define FLAG_HOMMG   (1 << 3)
#define FLAG_MOVING  (1 << 4)
#define FLAG_ZABLE   (1 << 5)
#define FLAG7        (1 << 6)
#define FLAG8        (1 << 7)

#define POLE_PAIRS 4U
#define RPM_TO_RAD_S 0.10471975512

typedef struct {
	uint32_t pwm_a;
	uint32_t pwm_b;
	uint32_t pwm_c;

	uint8_t motor_flags;

	uint8_t pole_pairs;
	uint32_t pwm_resolution;
}MOT_HandleTypeDef;


#endif
