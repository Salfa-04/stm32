#include "stdint.h"

void uprintf(const char *, ...);

typedef struct m2006 {
  uint16_t angle;
  int16_t speed;
  int16_t torque;
  uint16_t id;
} motor_t;

typedef struct PID {
  float kp, ki, kd;
  float error, lastError;
  float integral, maxIntegral;
  float output, maxOutput;
} pid_t;

void display(motor_t *);
void get_msg(motor_t *);
void sent_msg(uint16_t);

void PID_Init(pid_t *, float p, float i, float d, float maxI, float maxOut);
void update_pid(pid_t *, float, float);
