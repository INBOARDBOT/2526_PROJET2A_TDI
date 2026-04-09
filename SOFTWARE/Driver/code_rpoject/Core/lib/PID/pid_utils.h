#ifndef PID_UTILS_H
#define PID_UTILS_H

#include <PID/pid_config.h>

/* Initialise the whole PID_Control_t block */
void PID_Init(PID_Control_t        *ctrl,
              float32_t             Ts,
              float32_t             output_limit);

/* Initialise a single controller inside the block */
void PID_Config(arm_pid_instance_f32 *pid,
                const PID_Gains_t    *gains,
                float32_t             Ts);

/* Reset integrator state of one controller */
void PID_Reset(arm_pid_instance_f32 *pid);

/* Reset all controllers in the block */
void PID_ResetAll(PID_Control_t *ctrl);

/* Run one control step — returns clamped output */
float32_t PID_Update(arm_pid_instance_f32 *pid,
                     float32_t             error,
                     float32_t             output_limit);

void PID_autoconfig(PID_Control_t *ctrl, float32_t Ts);

#endif
