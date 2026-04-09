#include <PID/pid_utils.h>

/* ------------------------------------------------------------------ */
/*  Initialise the container — zeros everything, stores Ts             */
/* ------------------------------------------------------------------ */
void PID_Init(PID_Control_t *ctrl,
              float32_t      Ts,
              float32_t      output_limit)
{
    ctrl->Ts           = Ts;
    ctrl->output_limit = output_limit;

    /* Zero all three instances so they are in a known state
       before PID_Config is called individually for each one  */
    memset(&ctrl->PID_speed,    0, sizeof(arm_pid_instance_f32));
    memset(&ctrl->PID_position, 0, sizeof(arm_pid_instance_f32));
    memset(&ctrl->PID_voltage,  0, sizeof(arm_pid_instance_f32));
    memset(&ctrl->PID_current_d, 0, sizeof(arm_pid_instance_f32));
    memset(&ctrl->PID_current_q, 0, sizeof(arm_pid_instance_f32));
}

void PID_autoconfig(PID_Control_t *ctrl, float32_t Ts){
	PID_Gains_t speed_gains    = { .kp = 0.005f, .ki = 0.08f, .kd = 0.0f };
	PID_Gains_t position_gains = { .kp = 1.000f, .ki = 0.01f, .kd = 0.0f };
	PID_Gains_t voltage_gains  = { .kp = 0.500f, .ki = 0.05f, .kd = 0.0f };
	PID_Gains_t Id_gains  = { .kp = 0.500f, .ki = 0.05f, .kd = 0.01f };
	PID_Gains_t Iq_gains  = { .kp = 0.500f, .ki = 0.05f, .kd = 0.01f };


	PID_Config(&ctrl->PID_speed,     &speed_gains,    Ts);
	PID_Config(&ctrl->PID_position,  &position_gains, Ts);
	PID_Config(&ctrl->PID_voltage,   &voltage_gains,  Ts);
	PID_Config(&ctrl->PID_current_d, &Id_gains,  	 Ts);
	PID_Config(&ctrl->PID_current_q, &Iq_gains,  	 Ts);

}


/* ------------------------------------------------------------------ */
/*  Configure one controller with gains — scales Ki by Ts             */
/* ------------------------------------------------------------------ */
void PID_Config(arm_pid_instance_f32 *pid,
                const PID_Gains_t    *gains,
                float32_t             Ts)
{
    pid->Kp = gains->kp;
    pid->Ki = gains->ki * Ts;   /* pre-scale: arm_pid_f32 does not do this */
    pid->Kd = gains->kd / Ts;   /* pre-scale: derivative also needs Ts     */

    arm_pid_init_f32(pid, 1);   /* 1 = reset internal state registers      */
}

/* ------------------------------------------------------------------ */
/*  Reset integrator state of one controller                           */
/* ------------------------------------------------------------------ */
void PID_Reset(arm_pid_instance_f32 *pid)
{
    arm_pid_init_f32(pid, 1);
}

/* ------------------------------------------------------------------ */
/*  Reset all three controllers                                         */
/* ------------------------------------------------------------------ */
void PID_ResetAll(PID_Control_t *ctrl)
{
    arm_pid_init_f32(&ctrl->PID_speed,    1);
    arm_pid_init_f32(&ctrl->PID_position, 1);
    arm_pid_init_f32(&ctrl->PID_voltage,  1);
    arm_pid_init_f32(&ctrl->PID_current_d,  1);
    arm_pid_init_f32(&ctrl->PID_current_q,  1);
}

/* ------------------------------------------------------------------ */
/*  Run one step and clamp output                                       */
/* ------------------------------------------------------------------ */
float32_t PID_Update(arm_pid_instance_f32 *pid,
                     float32_t             error,
                     float32_t             output_limit)
{
    float32_t output = arm_pid_f32(pid, error);

    if      (output >  output_limit) output =  output_limit;
    else if (output < -output_limit) output = -output_limit;

    return output;
}
