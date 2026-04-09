#ifndef ENCODER_CONFIG_H
#define ENCODER_CONFIG_H

#include "arm_math.h"
#include <stdbool.h>

#define ENCODER_PPEDGES 4095U

typedef struct {
	int32_t       absolute_position;
	int32_t       speed_accumulator;
	uint32_t      current_raw_position_measured;
	uint32_t      previous_raw_position_measured;

	float32_t     rpm_measured;
	float32_t     rpm_filtered;
	float32_t     alpha_coef_filter;

	__IO bool     encoder_Z_pulse;
	__IO uint32_t raw_position_offset;

	float32_t     s_elec_angle;
	float32_t     s_meca_angle;

	volatile uint32_t encoder_in_zero_setting;
	float32_t magnetic_offset;
	uint32_t  encoder_magnetic_offset;
}ENC_HandleTypeDef;

#endif
