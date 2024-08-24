#include "main.h"
#include "source.h"

void PID_Init(pid_t *pid, float p, float i, float d, float maxI, float maxOut) {
  pid->kp = p;
  pid->ki = i;
  pid->kd = d;
  pid->maxIntegral = maxI;
  pid->maxOutput = maxOut;
}

void update_pid(pid_t *pid, float reference, float feedback) {
  // update error
  pid->lastError = pid->error;
  pid->error = reference - feedback;

  // update p, i, d outputs
  float p_out = pid->error * pid->kp;
  float i_out = pid->error * pid->ki;
  float d_out = (pid->error - pid->lastError) * pid->kd;

  pid->integral += i_out;
  if (pid->integral > pid->maxIntegral)
    pid->integral = pid->maxIntegral;
  else if (pid->integral < -(pid->maxIntegral))
    pid->integral = -(pid->maxIntegral);

  pid->output = p_out + d_out + pid->integral;
  if (pid->output > pid->maxOutput)
    pid->output = pid->maxOutput;
  else if (pid->output < -(pid->maxOutput))
    pid->output = -(pid->maxOutput);
}
