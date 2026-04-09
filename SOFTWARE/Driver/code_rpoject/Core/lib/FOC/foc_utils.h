#ifndef FOC_UTILS_H
#define FOC_UTILS_H


#define CAL_SETTLE_TICKS   500u
#define CAL_ITERATIONS     500u
#define CAL_UQ             2.0f
#define CAL_UD             0.0f



#include <peripherals/encoder/encoder_utils.h>
#include <peripherals/motor/motor_utils.h>
#include <peripherals/ADC/analog_utils.h>
#include <PID/pid_utils.h>

/******************************************[ FOC/SINGAL INIT ]******************************************/

void signal_init(Diagram_signal_t *inner_signal);
void foc_init(FOC_HandleTypeDef *foc_control, TIM_HandleTypeDef *htim1,
		TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc1, ADC_HandleTypeDef *hadc2);


/******************************************[ FOC control loop ]******************************************/

void foc_speed_control_update(FOC_HandleTypeDef *fonc_control);
void foc_current_control_update(FOC_HandleTypeDef *fonc_control);

/*******************************************[ FOC additional ]*******************************************/

void foc_position_control_update(FOC_HandleTypeDef *fonc_control);

/******************************************[ OPEN LOOP CONTROL ]*****************************************/

void open_loop_velocity_run(FOC_HandleTypeDef *fonc_control);
void open_loop_velocity_run_SVPWM(FOC_HandleTypeDef *fonc_control);
void setup_test_foc_open_loop(FOC_HandleTypeDef *foc_control);

/******************************************[ CALIBRATION ]*****************************************/

void lock_rotor(FOC_HandleTypeDef *foc, float32_t angle);
void mag_misalignement(FOC_HandleTypeDef *foc_system);
void mag_misalignement_encoder(FOC_HandleTypeDef *foc_system);

void zero_settings_calibration(FOC_HandleTypeDef *foc_system);
void go_to_position(FOC_HandleTypeDef *foc_system, float32_t target_pos_deg, float32_t speed_rpm);

#endif
