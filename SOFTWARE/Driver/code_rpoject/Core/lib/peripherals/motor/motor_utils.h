#ifndef MOTOR_UTILS_H
#define MOTOR_UTILS_H

#include <FOC/foc_config.h>


void motor_init(MOT_HandleTypeDef *motor, TIM_HandleTypeDef *htim);
void motor_reset(MOT_HandleTypeDef *motor);
void motor_start(MOT_HandleTypeDef *motor, TIM_HandleTypeDef* htim);
void motor_stop(MOT_HandleTypeDef *motor, TIM_HandleTypeDef* htim);
void motor_set_pwm(MOT_HandleTypeDef *motor, TIM_HandleTypeDef* htim,  uint32_t cha, uint32_t chb, uint32_t chc);

#endif
