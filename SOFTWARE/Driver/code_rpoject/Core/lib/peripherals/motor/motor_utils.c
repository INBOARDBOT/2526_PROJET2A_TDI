#include "motor_utils.h"


void motor_init(MOT_HandleTypeDef *motor, TIM_HandleTypeDef *htim){
	uint8_t mask = FLAG_INIT;
	if ((motor->motor_flags & mask) == mask) return;

	motor->motor_flags = 0;

	motor->pole_pairs = POLE_PAIRS;
	motor->pwm_resolution = htim->Instance->ARR;
	motor->motor_flags |= FLAG_INIT;
}

void motor_reset(MOT_HandleTypeDef *motor){
	uint8_t mask = FLAG_INIT;
	if ((motor->motor_flags & mask) != mask) return;

	motor->motor_flags = 0x00;
}


void motor_start(MOT_HandleTypeDef *motor, TIM_HandleTypeDef* htim){
	uint8_t mask = (FLAG_INIT | FLAG_START);
	if ((motor->motor_flags & mask) != FLAG_INIT || htim->Instance != TIM1)return; // different than INIT 1 and START 0

//	HAL_TIM_Base_Start_IT(htim);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(htim, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(htim, TIM_CHANNEL_2);

	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(htim, TIM_CHANNEL_3);

	motor->motor_flags |= FLAG_START;
}



void motor_stop(MOT_HandleTypeDef *motor, TIM_HandleTypeDef* htim){
	uint8_t mask = (FLAG_INIT | FLAG_START);
	if ((motor->motor_flags & mask) != mask || htim->Instance != TIM1)return; // different than INIT 1 and START 1

//	HAL_TIM_Base_Stop_IT(htim);
	HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(htim, TIM_CHANNEL_1);

	HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(htim, TIM_CHANNEL_2);

	HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Stop(htim, TIM_CHANNEL_3);

	motor->motor_flags &= ~FLAG_START;
	motor->motor_flags &= ~FLAG_MOVING;
}


void motor_set_pwm(MOT_HandleTypeDef *motor, TIM_HandleTypeDef* htim,  uint32_t cha, uint32_t chb, uint32_t chc){
	uint8_t mask = (FLAG_INIT | FLAG_START);
	if ((motor->motor_flags & mask) != mask || htim->Instance != TIM1)return; // different than INIT 1 and START 1

	htim->Instance->CCR1 = cha;
	htim->Instance->CCR2 = chb;
	htim->Instance->CCR3 = chc;

	motor->motor_flags |= FLAG_MOVING;
}


