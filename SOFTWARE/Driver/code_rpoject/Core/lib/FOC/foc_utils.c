#include "foc_utils.h"

typedef enum {
	MAG_LOCK,
//	MAG_SAMPLE,
	MAG_APPLY,
	MAG_DONE
} mag_state;

typedef enum {
    ZERO_IDLE,
	ZERO_LOCK_ZERO,
	ZERO_SEEK_Z_PULSE,
	ZERO_APPLY_OFFSET,
	ZERO_DONE
} zero_state;

/******************************************[ FOC inits ]******************************************/

void signal_init(Diagram_signal_t *inner_signal){
	if(inner_signal == NULL)return;

	*inner_signal = (Diagram_signal_t){0.0f};

	inner_signal->Ts_foc_current   = TS_10kHz;
	inner_signal->Ts_foc_speed     = TS_500Hz;
	inner_signal->Ts_foc_position  = TS_500Hz;
	inner_signal->Ts_foc_open_loop = TS_OPEN_LOOP;

}

void foc_init(FOC_HandleTypeDef *foc_control, TIM_HandleTypeDef *htim1,
		TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc1, ADC_HandleTypeDef *hadc2){

	if(foc_control == NULL)return;

//	memset(foc_control, 0, sizeof(FOC_HandleTypeDef));

	foc_control->adc1   = hadc1;
	foc_control->adc2   = hadc2;
	foc_control->timer1 = htim1;
	foc_control->timer2 = htim3;
}


/******************************************[ FOC control loop ]******************************************/

void foc_speed_control_update(FOC_HandleTypeDef *fonc_control){
	ENC_HandleTypeDef  *encoder    = fonc_control->encoder;
	Diagram_signal_t   *foc_signal = fonc_control->inner_signal;
	PID_Control_t      *pid_speed  = fonc_control->pid_control_system;

	float32_t error = foc_signal->rpm_setpoint - encoder->rpm_measured;

	/*No PID update on Id_input as it remains 0*/
	foc_signal->Id_input = 0.0f;
	foc_signal->Iq_input = PID_Update(&pid_speed->PID_speed, error, pid_speed->output_limit);
}

void foc_current_control_update(FOC_HandleTypeDef *fonc_control){
	Diagram_signal_t   *foc_signal      = fonc_control->inner_signal;
	PID_Control_t      *pid_current     = fonc_control->pid_control_system;

	float32_t Id_ref = foc_signal->Id_input;
	float32_t Id_mes = foc_signal->Id_feedback;

	float32_t Iq_ref = foc_signal->Iq_input;
	float32_t Iq_mes = foc_signal->Iq_feedback;

	float32_t error_d = Id_ref - Id_mes;
	float32_t error_q = Iq_ref - Iq_mes;

	foc_signal->Ud = PID_Update(&pid_current->PID_current_d, error_d, pid_current->output_limit);
	foc_signal->Uq = PID_Update(&pid_current->PID_current_q, error_q, pid_current->output_limit);
}

/********************************************************************************************************/


/*******************************************[ FOC additional ]*******************************************/

/* called every 1/10th and then calls foc_speed_control_update (even though hard reset position to 0 en Z pulses)*/
void foc_position_control_update(FOC_HandleTypeDef *fonc_control){
	ENC_HandleTypeDef  *encoder   = fonc_control->encoder;
	MOT_HandleTypeDef  *motor     = fonc_control->motor;
	Diagram_signal_t   *foc_signal = fonc_control->inner_signal;
	PID_Control_t      *pid_angle = fonc_control->pid_control_system;

	float32_t angle_setpoint = (float32_t)foc_signal->position_setpoint / (float32_t)motor->pwm_resolution * 360.0f;
	float32_t angle_measured = (float32_t)encoder->current_raw_position_measured / (float32_t)motor->pwm_resolution * 360.0f;
	float32_t error = angle_setpoint - angle_measured;

	if(error > 180.0f){
	    error -= 360.0f;
	} else if(error < -180.0f){
	    error += 360.0f;
	}

	foc_signal->position_setpoint = PID_Update(&pid_angle->PID_position, error, pid_angle->output_limit);
}

/********************************************************************************************************/

/******************************************[ OPEN LOOP CONTROL ]*****************************************/

void open_loop_velocity_run(FOC_HandleTypeDef *fonc_control)
{
    MOT_HandleTypeDef  *motor        = fonc_control->motor;
    TIM_HandleTypeDef  *htim_pwm     = fonc_control->timer1;
    Diagram_signal_t   *foc_signal   = fonc_control->inner_signal;
    float32_t           target_rpm   = foc_signal->rpm_setpoint;
    float32_t           Ts           = foc_signal->Ts_foc_open_loop;

    float32_t elec_rpm    = target_rpm * (float32_t)motor->pole_pairs;
    float32_t deg_per_sec = (elec_rpm / 60.0f) * 360.0f;
    float32_t delta_theta = deg_per_sec * Ts;

    foc_signal->position_setpoint += delta_theta;

    if (foc_signal->position_setpoint >= 360.0f) foc_signal->position_setpoint -= 360.0f;
    if (foc_signal->position_setpoint < 0.0f)    foc_signal->position_setpoint += 360.0f;

    float32_t v_norm = foc_signal->V_open_loop;

    float32_t sin_a, sin_b, sin_c, unused;
    arm_sin_cos_f32(foc_signal->position_setpoint,          &sin_a, &unused);
    arm_sin_cos_f32(foc_signal->position_setpoint - 120.0f, &sin_b, &unused);
    arm_sin_cos_f32(foc_signal->position_setpoint + 120.0f, &sin_c, &unused);

    uint32_t pwm_a = (uint32_t)(((sin_a * v_norm) * 0.5f + 0.5f) * motor->pwm_resolution);
    uint32_t pwm_b = (uint32_t)(((sin_b * v_norm) * 0.5f + 0.5f) * motor->pwm_resolution);
    uint32_t pwm_c = (uint32_t)(((sin_c * v_norm) * 0.5f + 0.5f) * motor->pwm_resolution);

    motor_set_pwm(motor, htim_pwm, pwm_a, pwm_b, pwm_c);
}

void open_loop_velocity_run_SVPWM(FOC_HandleTypeDef *fonc_control)
{
    MOT_HandleTypeDef  *motor        = fonc_control->motor;
    TIM_HandleTypeDef  *htim_pwm     = fonc_control->timer1;
    Diagram_signal_t   *foc_signal   = fonc_control->inner_signal;
    float32_t           target_rpm   = foc_signal->rpm_setpoint;
    float32_t           Ts           = foc_signal->Ts_foc_open_loop;
    float32_t           Vbus         = 24.0f;

    float32_t omega = target_rpm * RPM_TO_RAD_S * (float32_t)motor->pole_pairs;

    foc_signal->theta_electrique += omega * Ts; // Change this
    foc_signal->position_setpoint = foc_signal->theta_electrique * 57.2957795131f;

    if (foc_signal->theta_electrique >= TWO_PI)foc_signal->theta_electrique -= TWO_PI;
    else if (foc_signal->theta_electrique < 0.0f)foc_signal->theta_electrique += TWO_PI;

    park_invtransforn(foc_signal->Ud, foc_signal->Uq, foc_signal->theta_electrique,
    		&foc_signal->Ualpha, &foc_signal->Ubeta);

    uint32_t pwm_u, pwm_v, pwm_w;
	svpwm(foc_signal->Ualpha, foc_signal->Ubeta, Vbus,
			motor->pwm_resolution, &pwm_u, &pwm_v, &pwm_w);

	motor_set_pwm(motor, htim_pwm, pwm_u, pwm_v, pwm_w);
}

void setup_test_foc_open_loop(FOC_HandleTypeDef *foc_control){
	/* TIMER */
	start_general_timer(foc_control);
	start_injection_channel(foc_control);

	/* ENCODER */
	encoder_init(foc_control->encoder);
	start_encoder_timer(foc_control);

	/* ANALOG */
	analog_init(foc_control->analog, foc_control->adc1, foc_control->adc2);

	/* MOTOR */
	motor_init(foc_control->motor, foc_control->timer1);
	motor_start(foc_control->motor, foc_control->timer1);

	/* SIGNAL  */
	signal_init(foc_control->inner_signal);
}

/******************************************[ CALIBRATION ]*****************************************/

void lock_rotor(FOC_HandleTypeDef *foc, float32_t angle)
{
    float32_t Ualpha, Ubeta;
    MOT_HandleTypeDef *motor = foc->motor;

//    park_invtransforn(CAL_UD, CAL_UQ, angle, &Ualpha, &Ubeta);
    park_invtransforn(foc->inner_signal->Ud, foc->inner_signal->Uq, angle, &Ualpha, &Ubeta);

    uint32_t pu, pv, pw;
    svpwm(Ualpha, Ubeta, 24.0f, motor->pwm_resolution, &pu, &pv, &pw);
    motor_set_pwm(motor, foc->timer1, pu, pv, pw);
}



//void mag_misalignement(FOC_HandleTypeDef *foc_system)
//{
//    ENC_HandleTypeDef  *enc   = foc_system->encoder;
//    MOT_HandleTypeDef  *motor = foc_system->motor;
//    Diagram_signal_t   *sig   = foc_system->inner_signal;
//
//    static mag_state   cal_state    = MAG_LOCK;
//    static uint32_t    tick         = 0;
//    static float32_t   offset_accum = 0.0f;
//
//	switch (cal_state)
//	{
//		case MAG_LOCK:
//
//			sig->theta_electrique = 0.0f;
//			sig->rpm_setpoint     = 0.0f;
//			lock_rotor(foc_system, 0.0f);
//
//			if (++tick >= CAL_SETTLE_TICKS) {
//
//				enc->previous_raw_position_measured =
//					enc->current_raw_position_measured;
//
//				enc->s_elec_angle = 0.0f;
//				enc->s_meca_angle = 0.0f;
//
//				tick         = 0;
//				offset_accum = 0.0f;
//				cal_state    = MAG_SAMPLE;
//			}
//			break;
//
//		 case MAG_SAMPLE:
//
//			sig->theta_electrique = 0.0f;
//			lock_rotor(foc_system, 0.0f);
//
//			offset_accum += enc->s_elec_angle;
//
//			if (++tick >= CAL_ITERATIONS) {
//				tick      = 0;
//				cal_state = MAG_APPLY;
//			}
//			break;
//		 case MAG_APPLY:
//
//			enc->magnetic_offset = offset_accum / (float32_t)CAL_ITERATIONS;
//
//			sig->theta_electrique = wrap_two_pi(
//				enc->s_elec_angle - enc->magnetic_offset);
//
//			sig->position_setpoint = sig->theta_electrique * RAD_TO_DEG_F;
//			sig->rpm_setpoint      = 0.0f;
//			sig->Uq                = 2.0f;
//
//			motor->motor_flags |= FLAG_ZABLE;
//			cal_state = MAG_DONE;
//			break;
//
//		case MAG_DONE:
//			break;
//	}
//
//}


void mag_misalignement_encoder(FOC_HandleTypeDef *foc_system)
{
    ENC_HandleTypeDef  *enc   = foc_system->encoder;
    MOT_HandleTypeDef  *motor = foc_system->motor;
    Diagram_signal_t   *sig   = foc_system->inner_signal;

    static mag_state   cal_state    = MAG_LOCK;
    static uint32_t    tick         = 0;
    static float32_t   offset_accum = 0.0f;

    sig->Ud = 0.0f;
	sig->Uq = 2.0f;

	switch (cal_state)
	{
		case MAG_LOCK:

			sig->theta_electrique = 0.0f;
			sig->rpm_setpoint     = 0.0f;
			lock_rotor(foc_system, 0.0f);

			if (++tick >= CAL_SETTLE_TICKS) {

				enc->s_elec_angle = 0.0f;
				enc->s_meca_angle = 0.0f;

				tick         = 0;
				offset_accum = 0.0f;
				set_encoder_zero(foc_system->encoder, foc_system->timer2);
				cal_state    = MAG_APPLY;
			}
			break;
		 case MAG_APPLY:

			if (++tick >= CAL_SETTLE_TICKS) {
				tick         = 0;
				motor->motor_flags |= FLAG_ZABLE;
				cal_state = MAG_DONE;
			}
			break;

		case MAG_DONE:
			break;
	}

}


//void zero_settings_calibration(FOC_HandleTypeDef *fonc_control) {
//    MOT_HandleTypeDef  *motor      = fonc_control->motor;
//    ENC_HandleTypeDef  *encoder    = fonc_control->encoder;
//    Diagram_signal_t   *foc_signal = fonc_control->inner_signal;
//    TIM_HandleTypeDef  *timer_encoder = fonc_control->timer2;
//
//    static zero_state   cal_state    = ZERO_IDLE;
//    static uint16_t     settle_ticks = 0;
//
//    switch (cal_state) {
//
//        case ZERO_IDLE:
//            settle_ticks = 0;
////            encoder_reset(encoder);
////            foc_signal->position_setpoint = 0.0f;
////            foc_signal->theta_electrique  = 0.0f;
////            foc_signal->rpm_setpoint      = 0.0f;
//            cal_state = ZERO_LOCK_ZERO;
//            break;
//
//        case ZERO_LOCK_ZERO:
//            settle_ticks++;
//            if (settle_ticks >= 1000) {
//                settle_ticks = 0;
//                foc_signal->Ud = 0.0f;
//                foc_signal->Uq = 2.0f;
//                foc_signal->rpm_setpoint = 10.0f;
//                encoder->encoder_in_zero_setting = 1;
//                cal_state = ZERO_SEEK_Z_PULSE;
//            }
//            break;
//
//        case ZERO_SEEK_Z_PULSE:
////        	update_raw_position(encoder, timer_encoder);
////			update_angles(encoder, fonc_control->motor->pole_pairs);
//            open_loop_velocity_run_SVPWM(fonc_control);
//
//
//            if (encoder->encoder_Z_pulse) {
//                encoder->encoder_in_zero_setting = 0;
//                foc_signal->rpm_setpoint = 0.0f;
//				encoder->s_elec_angle = 0.0f;
//				encoder->s_meca_angle = 0.0f;
//				cal_state = ZERO_APPLY_OFFSET;
//            }
//
//            settle_ticks++;
//            if (settle_ticks >= 7 * 500) {
//                cal_state = ZERO_IDLE;
//            }
//            break;
//
//        case ZERO_APPLY_OFFSET:
//        	timer_encoder->Instance->CNT -= encoder->raw_position_offset;
//        	encoder->current_raw_position_measured  = timer_encoder->Instance->CNT;
//			encoder->previous_raw_position_measured = encoder->current_raw_position_measured;
//            cal_state = ZERO_DONE;
//            break;
//
//        case ZERO_DONE:
////        	update_raw_position(encoder, timer_encoder);
////			update_angles(encoder, fonc_control->motor->pole_pairs);
//
//			foc_signal->rpm_setpoint      = 60.0f;
//			foc_signal->Uq                = 2.0f;
//
//            motor->motor_flags |= FLAG_CALBTN;
//            break;
//    }
//}
