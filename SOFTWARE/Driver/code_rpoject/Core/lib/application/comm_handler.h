#ifndef COMM_HANDLER_H
#define COMM_HANDLER_H

#include "stm32g4xx_hal.h"
#include <string.h>


typedef struct{
	UART_HandleTypeDef* usart;
}COM_HandleTypeDef;


void comm_init(COM_HandleTypeDef *comm_system, UART_HandleTypeDef* huart);

#endif
