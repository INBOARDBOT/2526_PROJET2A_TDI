#ifndef FSM_CONTROL_H
#define FSM_CONTROL_H

#include "FOC/foc_config.h"
#include "comm_handler.h"
//#include <FOC/foc_utils.h>

typedef enum{
	TEST,
	TEST1,
	TEST2
}fsm_mode_t;


typedef struct{
	fsm_mode_t fsm_current_state;
	fsm_mode_t fsm_previous_state;

	FOC_HandleTypeDef *foc_control;
	COM_HandleTypeDef *communication;
}FSM_HandleTypeDef;



#endif
