#include <FOC/foc_Config.h>


void start_general_timer(FOC_HandleTypeDef *foc_control){
	HAL_TIM_Base_Start_IT(foc_control->timer1);
}

void stop_general_timer(FOC_HandleTypeDef *foc_control){
	HAL_TIM_Base_Stop_IT(foc_control->timer1);
}


void start_injection_channel(FOC_HandleTypeDef *foc_control){
	HAL_TIM_OC_Start(foc_control->timer1, TIM_CHANNEL_4);
}

void stop_injection_channel(FOC_HandleTypeDef *foc_control){
	HAL_TIM_OC_Stop(foc_control->timer1, TIM_CHANNEL_4);
}


void start_encoder_timer(FOC_HandleTypeDef *foc_control){
	HAL_TIM_Encoder_Start_IT(foc_control->timer2, TIM_CHANNEL_ALL);
}

void stop_encoder_timer(FOC_HandleTypeDef *foc_control){
	HAL_TIM_Encoder_Stop_IT(foc_control->timer2, TIM_CHANNEL_ALL);
}
