#ifndef __PID_H
#define __PID_H

typedef struct PID {
  float kp, ki, kd;
  float error, lastError;
  float integral, maxIntegral;
  float output, maxOutput;
} pid_t;

void PID_Init(pid_t *, float p, float i, float d, float maxI, float maxOut);
float update_pid(pid_t *, float ref, float fed);

#endif /* __PID_H */
