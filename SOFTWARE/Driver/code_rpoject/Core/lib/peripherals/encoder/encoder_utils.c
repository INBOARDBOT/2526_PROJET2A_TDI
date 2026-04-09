#include <peripherals/encoder/encoder_utils.h>

__IO uint8_t z_pulse_detected = 0;
__IO uint32_t raw_off_pos = 0;

void encoder_init(ENC_HandleTypeDef *encoder) {
    if (encoder == NULL) return;

    *encoder = (ENC_HandleTypeDef){
        .absolute_position = 0.0f,
        .alpha_coef_filter = 0.1f,
        .current_raw_position_measured = 0,
        .encoder_Z_pulse = false,
        .raw_position_offset = 0,
        .rpm_filtered = 0.0f,
        .rpm_measured = 0.0f,
        .s_elec_angle = 0.0f,
        .s_meca_angle = 0.0f,
		.encoder_in_zero_setting = 0
    };
}


void set_encoder_zero(ENC_HandleTypeDef *encoder, TIM_HandleTypeDef *htim){
	encoder_reset(encoder);
	htim->Instance->CNT = 0;
}

void update_raw_position(ENC_HandleTypeDef *encoder, TIM_HandleTypeDef *htim){
	encoder->previous_raw_position_measured = encoder->current_raw_position_measured;
	encoder->current_raw_position_measured = htim->Instance->CNT;

	/* Accumulate absolute position — handles wrap */
//	int32_t delta = (int32_t)encoder->current_raw_position_measured
//				  - (int32_t)encoder->previous_raw_position_measured;
//
//	int32_t half = (int32_t)(ENCODER_PPEDGES / 2);
//	if      (delta >  half) delta -= (int32_t)ENCODER_PPEDGES;
//	else if (delta < -half) delta += (int32_t)ENCODER_PPEDGES;
//
//	encoder->absolute_position += delta; /* does not reset */
//	encoder->speed_accumulator += delta; /* does reset at 500Hz */

}

void calculate_rpm(ENC_HandleTypeDef *encoder, float32_t Ts) {
	int32_t delta = encoder->speed_accumulator;
	encoder->speed_accumulator = 0;

    encoder->rpm_measured = ((float32_t)delta / (float32_t)ENCODER_PPEDGES) / Ts * 60.0f;
    encoder->rpm_filtered = (encoder->alpha_coef_filter * encoder->rpm_measured) + ((1.0f - encoder->alpha_coef_filter) * encoder->rpm_filtered);
}

void update_encoder_interrupt(ENC_HandleTypeDef *encoder, TIM_HandleTypeDef *htim){
	if(encoder->encoder_in_zero_setting == 1){
		encoder->raw_position_offset = htim->Instance->CNT;
		encoder->encoder_Z_pulse = true;
	}
}


void update_angles(ENC_HandleTypeDef *encoder, uint8_t pole_pairs)
{
    int32_t delta = (int32_t)encoder->current_raw_position_measured
                  - (int32_t)encoder->previous_raw_position_measured;

    int32_t half = (int32_t)(ENCODER_PPEDGES / 2);
    if      (delta >  half) delta -= (int32_t)ENCODER_PPEDGES;
    else if (delta < -half) delta += (int32_t)ENCODER_PPEDGES;

    float32_t delta_norm = (float32_t)delta / (float32_t)ENCODER_PPEDGES;

    /* Mechanical angle [0, 2π] */
    encoder->s_meca_angle += delta_norm * 2.0f * PI;
    if      (encoder->s_meca_angle >  2.0f * PI) encoder->s_meca_angle -= 2.0f * PI;
    else if (encoder->s_meca_angle <  0.0f)       encoder->s_meca_angle += 2.0f * PI;

    /* Electrical angle [0, 2π] */
    encoder->s_elec_angle += delta_norm * 2.0f * PI * pole_pairs;
	if      (encoder->s_elec_angle >  2.0f * PI) encoder->s_elec_angle -= 2.0f * PI;
	else if (encoder->s_elec_angle <  0.0f)       encoder->s_elec_angle += 2.0f * PI;

}



void encoder_reset(ENC_HandleTypeDef *encoder){
	encoder->absolute_position = 0;
	encoder->current_raw_position_measured = 0;
	encoder->previous_raw_position_measured = 0;
	encoder->encoder_Z_pulse = false;
	encoder->raw_position_offset = 0;
	encoder->rpm_filtered = 0.0f;
	encoder->rpm_measured = 0.0f;
	encoder->s_elec_angle = 0.0f;
	encoder->s_meca_angle = 0.0f;
	encoder->speed_accumulator = 0;
	encoder->magnetic_offset   = 0;
}
