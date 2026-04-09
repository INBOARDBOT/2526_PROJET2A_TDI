#ifndef DISPLAY_PY_GRAPH
#define DISPLAY_PY_GRAPH

#include <FOC/foc_utils.h>
#include <string.h>
#include <stdio.h>

void send_to_Python_over_UART(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1);
void send_to_Python_over_UART_RPM(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1);
void send_to_Python_over_UART_pos(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1);
void send_to_Python_over_UART_amp(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1);

void send_to_Python_over_UART_pos_direct(const FOC_HandleTypeDef *foc,
                                          UART_HandleTypeDef *hlpuart1,
                                          float32_t setpoint_deg,
                                          float32_t measured_deg);

void send_to_Python_over_UART_amp_special(const FOC_HandleTypeDef *foc_control, UART_HandleTypeDef *hlpuart1);

#endif
