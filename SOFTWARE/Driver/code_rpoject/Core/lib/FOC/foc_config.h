#ifndef FOC_CONFIG_H
#define FOC_CONFIG_H

#define TS_500Hz     0.002f
#define TS_10kHz     0.00005f
/*#define TS_OPEN_LOOP 0.0005f*/
#define TS_OPEN_LOOP 0.002f


#include "stm32g4xx_hal.h"
#include <peripherals/encoder/encoder_config.h>
#include <peripherals/motor/motor_config.h>
#include <peripherals/ADC/analog_config.h>
#include <PID/pid_config.h>


#define MAX_BUFFER 256

typedef struct {
	float32_t Ialpha;
	float32_t Ibeta;

	float32_t Iq_input;
	float32_t Iq_feedback;

	float32_t Id_input; /* NO field weakening*/
	float32_t Id_feedback;

	float32_t V_open_loop;

	float32_t Uq;
	float32_t Ud;

	float32_t Ualpha;
	float32_t Ubeta;

	float32_t theta_electrique;
	float32_t rpm_setpoint;
	float32_t position_setpoint;

	float32_t Ts_foc_open_loop;
	float32_t Ts_foc_current;
	float32_t Ts_foc_speed;
	float32_t Ts_foc_position;

	float32_t Irs[MAX_BUFFER];
	float32_t Ilq[MAX_BUFFER];
	float32_t Ild[MAX_BUFFER];

}Diagram_signal_t;

typedef struct {
	__IO float32_t      Id_averaged;
	__IO float32_t      Iq_averaged;

	TIM_HandleTypeDef   *timer1;
	TIM_HandleTypeDef   *timer2;

	ADC_HandleTypeDef   *adc1;
	ADC_HandleTypeDef   *adc2;

	ENC_HandleTypeDef   *encoder;

	MOT_HandleTypeDef   *motor;

	ANG_HandleTypeDef   *analog;

	PID_Control_t       *pid_control_system;

	Diagram_signal_t    *inner_signal; /* Inner signal correspond to the FOC signal that arent sensor output */
}FOC_HandleTypeDef;


void start_general_timer(FOC_HandleTypeDef *foc_control);
void stop_general_timer(FOC_HandleTypeDef *foc_control);

void start_injection_channel(FOC_HandleTypeDef *foc_control);
void stop_injection_channel(FOC_HandleTypeDef *foc_control);

void start_encoder_timer(FOC_HandleTypeDef *foc_control);
void stop_encoder_timer(FOC_HandleTypeDef *foc_control);

#endif
