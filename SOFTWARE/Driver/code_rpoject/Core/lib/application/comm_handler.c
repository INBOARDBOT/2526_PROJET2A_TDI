#include <application/comm_handler.h>


void comm_init(COM_HandleTypeDef *comm_system, UART_HandleTypeDef* huart){
	if(comm_system == NULL)return;

	memset(comm_system, 0, sizeof(COM_HandleTypeDef));

	*comm_system = (COM_HandleTypeDef){
		.usart = huart
	};
}
