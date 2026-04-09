#include <application/app_utils.h>
#include "main.h"

void application_init(FSM_HandleTypeDef *application, FOC_HandleTypeDef *foc_control_system,
		COM_HandleTypeDef *communication_system){
	if(application == NULL)return;

	application->foc_control   = foc_control_system;
	application->communication = communication_system;
}

void application_setup(FSM_HandleTypeDef *application){
	FOC_HandleTypeDef *foc_control = application->foc_control;

	setup_test_foc_open_loop(foc_control);
	foc_control->inner_signal->rpm_setpoint      = 60.0f;
	foc_control->inner_signal->position_setpoint = 0.0f;

#if FOC_TEST_SVPWM
	foc_control->inner_signal->theta_electrique  = 0.0f;
	foc_control->inner_signal->Ud                = 0.0f;
	foc_control->inner_signal->Uq                = 2.0f;
#endif
#if FOC_TEST_SPWM
	foc_control->inner_signal->V_open_loop = 0.2f;
#endif

}


typedef enum {
	MAG_MISALIGNEMNT,
	Z_SETTINGS,
	GO_TO_270_DEG,
	FORCE_ZERO
}launch_seq_t;


#define Kp 0.01f
#define Kpi 0.005f

#define Kd 0.1f
#define Kq 0.1f

static inline void asserv_current(FOC_HandleTypeDef  *foc, float32_t I0){
	ANG_HandleTypeDef *analog = foc->analog;
	Diagram_signal_t  *sig   = foc->inner_signal;

	float32_t error_q = ((float32_t)0.0f - sig->Iq_feedback);
	float32_t error_d = ((float32_t)I0 - sig->Id_feedback);


	float32_t tmp_D = Kd * error_d > 20.0f ? 20.0f : Kd * error_d;
	float32_t tmp_Q = Kq * error_q > 20.0f ? 20.0f : Kq * error_q;

	sig->Ud = tmp_D;
	sig->Uq = tmp_Q;
}

static inline void asserv_pos(FOC_HandleTypeDef  *foc, float32_t theta, float32_t pos_setpoint, float32_t *I0){
	ANG_HandleTypeDef *analog = foc->analog;
	Diagram_signal_t  *sig   = foc->inner_signal;

	float32_t to_rad = pos_setpoint * PI / 180.0f;

	float32_t error_pos = to_rad - theta;

	float32_t tmp_var = Kp * error_pos; // + Kpi * error_pos / foc->inner_signal->Ts_foc_current;
	*I0 = (tmp_var >= 10.0f) ? 10.0f : tmp_var;
}



static inline float32_t wrap_two_pi(float32_t a)
{
    while (a >= TWO_PI) a -= TWO_PI;
    while (a <  0.0f)   a += TWO_PI;
    return a;
}


void launch_sequence(FSM_HandleTypeDef *application){
	FOC_HandleTypeDef  *foc   = application->foc_control;
	UART_HandleTypeDef *com   = application->communication->usart;
	MOT_HandleTypeDef  *motor = foc->motor;
	ENC_HandleTypeDef  *enc   = foc->encoder;
	Diagram_signal_t   *sig   = foc->inner_signal;
	ANG_HandleTypeDef  *analog = foc->analog;

	static launch_seq_t fsm_var = MAG_MISALIGNEMNT;

	switch(fsm_var){
	case MAG_MISALIGNEMNT:
		mag_misalignement_encoder(foc);
		if(motor->motor_flags & FLAG_ZABLE){
			fsm_var = GO_TO_270_DEG;
		}
		break;

//	case GO_TO_270_DEG:
//		update_raw_position(enc, foc->timer2);
//
////		static int tick = 0;
//		static float32_t theta = 0.0f;
//		enc->s_elec_angle = theta;
//
//		sig->Ud = 0.0f;
//		sig->Uq = 2.0f;
//
//		lock_rotor(foc, theta);
//
////		++tick;
//
////		if(tick > 100){
////		theta = (theta + PI / 4.0f);
//		// (float32_t)motor->pole_pairs
//		theta -= 10.0f * RPM_TO_RAD_S * sig->Ts_foc_open_loop;
//		theta = wrap_two_pi(theta);
////		tick = 0;
////		}
//
//		clark_transforn(analog->Ia, analog->Ib, &sig->Ialpha, &sig->Ibeta);
//		park_transforn(sig->Ialpha, sig->Ibeta, theta, &sig->Id_feedback, &sig->Iq_feedback);
//
////		send_to_Python_over_UART_amp(foc, com);
//
//		asserv_current(foc, theta, 3.0f);
//
//		send_to_Python_over_UART_amp_special(foc, com);
//
//		break;
//
//	}

	case GO_TO_270_DEG:
		static __IO float32_t summ_d = 0.0f;
		static __IO float32_t summ_q = 0.0f;


		static int tick = 0;
		static float32_t current_setpoint = 10.0f;


		update_raw_position(enc, foc->timer2);

		float32_t meca = enc->current_raw_position_measured / 4095.0f * TWO_PI;
		float32_t theta = meca * motor->pole_pairs;

		clark_transforn(analog->Ia, analog->Ib, &sig->Ialpha, &sig->Ibeta);

		float32_t theta_park = wrap_two_pi(theta);
		park_transforn(sig->Ialpha, sig->Ibeta, theta_park, &sig->Iq_feedback, &sig->Id_feedback);

		lock_rotor(foc, theta);

		summ_d += sig->Iq_feedback;
		summ_q += sig->Id_feedback;

		++tick;

		if(tick > 3){
			foc->Iq_averaged = summ_q / (float32_t)4.0f;
			foc->Id_averaged = summ_d / (float32_t)4.0f;

			summ_q = 0.0f;
			summ_d = 0.0f;

			sig->Iq_feedback = foc->Iq_averaged;
			sig->Id_feedback = foc->Id_averaged;

			asserv_current(foc, current_setpoint);

//			HAL_GPIO_TogglePin(TEST_ADC_GPIO_Port, TEST_ADC_Pin);

			tick = 0;
		}


//		if(tick > 2000){
//			current_setpoint += 1 * sig->Ts_foc_current;
//			state = (state + 1)%10;
//			tick = 0;
//		}

//		sig->Id_input = 0.0f;
//		sig->Iq_input = current_setpoint;

//		send_to_Python_over_UART_amp_special(foc, com);

		break;

	}


}

void exec_open_loop_500Hz(FSM_HandleTypeDef *application)
{
//    launch_sequence(application)

//	update_raw_position(application->foc_control, application->foc_control->timer2);
//	calculate_rpm(application->foc_control->encoder, application->foc_control->inner_signal->Ts_foc_speed);
	application->foc_control->inner_signal->Ualpha = application->foc_control->Id_averaged;
	application->foc_control->inner_signal->Ubeta  = application->foc_control->Iq_averaged;
}


void exec_loop_10kHz(FSM_HandleTypeDef *application){
//	launch_sequence(application);

	application->foc_control->Id_averaged += 1.0f;
	application->foc_control->Iq_averaged += 2.0f;

	if(application->foc_control->Id_averaged > 10000.0f){
		application->foc_control->Id_averaged = 0.0f;
	}

	if(application->foc_control->Iq_averaged > 20000.0f){
		application->foc_control->Iq_averaged = 0.0f;
	}
}
