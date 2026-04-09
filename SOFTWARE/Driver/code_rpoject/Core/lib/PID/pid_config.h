#ifndef PID_CONFIG_H
#define PID_CONFIG_H

#include <FOC/foc_math.h>

typedef struct {
    float32_t Ts;
    float32_t output_limit;        /* shared clamp for all controllers */

    arm_pid_instance_f32 PID_speed;
    arm_pid_instance_f32 PID_position;
    arm_pid_instance_f32 PID_voltage;
    arm_pid_instance_f32 PID_current_d;
    arm_pid_instance_f32 PID_current_q;
} PID_Control_t;

typedef struct {
    float32_t kp;
    float32_t ki;                  /* pass raw Ki — Ts scaling done in init */
    float32_t kd;
    float32_t output_limit;
} PID_Gains_t;

#endif
