#include <application/displayPyGraph.h>

//static char tx_buf[64];
//static volatile uint8_t tx_busy = 0;
//
//void send_to_Python_over_UART_pos(const FOC_HandleTypeDef *foc, UART_HandleTypeDef *huart) {
//    if (tx_busy) return;  // previous transfer still running — drop this frame
//
//    tx_busy = 1;
//    int len = snprintf(tx_buf, sizeof(tx_buf), "a:%.3f | b:%.3f\r\n",
//        foc->inner_signal->position_setpoint,
//        foc->encoder->s_elec_angle * (180.0f / PI));
//
//    HAL_UART_Transmit_DMA(huart, (uint8_t*)tx_buf, len);
//}


void send_to_Python_over_UART_RPM(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1){
	  char buf[512];
	  int len = snprintf(buf, sizeof(buf), "a:%.3f | b:%.3f | c:%.3f\r\n",
			  foc_control->inner_signal->rpm_setpoint, foc_control->encoder->rpm_measured, foc_control->encoder->rpm_filtered);
	  HAL_UART_Transmit(hlpuart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
}

void send_to_Python_over_UART_pos(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1){
	  char buf[512];
	  int len = snprintf(buf, sizeof(buf), "a:%.3f | b:%.3f\r\n",
			  foc_control->inner_signal->position_setpoint, (foc_control->encoder->s_elec_angle - foc_control->encoder->magnetic_offset) * (180.0f / PI));
	  HAL_UART_Transmit(hlpuart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
}


void send_to_Python_over_UART_amp(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1){
	  char buf[512];
	  int len = snprintf(buf, sizeof(buf), "a:%.3f | b:%.3f | c:%.3f\r\n",
			  foc_control->analog->Ia, foc_control->analog->Ib, foc_control->analog->Ic);
	  HAL_UART_Transmit(hlpuart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
}

void send_to_Python_over_UART_amp_special(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1){
	  char buf[512];
	  int len = snprintf(buf, sizeof(buf), "a:%.3f | b:%.3f | c:%.3f | d:%.3f | e:%.3f | f:%.3f | g:%.3f\r\n",
			  foc_control->analog->Ia, foc_control->analog->Ib, foc_control->analog->Ic,
			  foc_control->inner_signal->Id_feedback, foc_control->inner_signal->Iq_feedback,
			  foc_control->inner_signal->Iq_input, foc_control->inner_signal->Id_input);
	  HAL_UART_Transmit(hlpuart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
}

void send_to_Python_over_UART_pos_direct(const FOC_HandleTypeDef *foc,
                                          UART_HandleTypeDef *hlpuart1,
                                          float32_t setpoint_deg,
                                          float32_t measured_deg)
{
    char buf[64];
    float32_t error = setpoint_deg - measured_deg;
    int len = snprintf(buf, sizeof(buf), "a:%.2f|b:%.2f|e:%.2f\n",
                       setpoint_deg, measured_deg, error);
    HAL_UART_Transmit(hlpuart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
}
