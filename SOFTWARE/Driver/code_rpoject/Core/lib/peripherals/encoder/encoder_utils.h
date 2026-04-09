#ifndef ENCODER_UTILS_H
#define ENCODER_UTILS_H

#include <FOC/foc_config.h>

void encoder_init(ENC_HandleTypeDef *encoder);
void update_raw_position(ENC_HandleTypeDef *encoder, TIM_HandleTypeDef *htim);
void calculate_rpm(ENC_HandleTypeDef *encoder, float32_t Ts);
void update_encoder_interrupt(ENC_HandleTypeDef *encoder, TIM_HandleTypeDef *htim);
void update_angles(ENC_HandleTypeDef *encoder, uint8_t pole_pairs);
void encoder_reset(ENC_HandleTypeDef *encoder);
void set_encoder_zero(ENC_HandleTypeDef *encoder, TIM_HandleTypeDef *htim);

#endif
