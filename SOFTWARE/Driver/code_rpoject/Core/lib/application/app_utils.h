#ifndef APP_UTILS_H
#define APP_UTILS_H


#define FOC_TEST_SPWM 0
#define FOC_TEST_SVPWM 1

#include <application/fsm_control.h>
#include <application/displayPyGraph.h>
#include <FOC/foc_utils.h>

void application_init(FSM_HandleTypeDef *application, FOC_HandleTypeDef *foc_control_system,
		COM_HandleTypeDef *communication_system);

void application_setup(FSM_HandleTypeDef *application);

void exec_loop_500Hz(FSM_HandleTypeDef *application);
void exec_loop_10kHz(FSM_HandleTypeDef *application);

void exec_open_loop_500Hz(FSM_HandleTypeDef *application);

#endif
