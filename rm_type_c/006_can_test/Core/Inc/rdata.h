#include "stdint.h"

extern int sprintf(char *, const char *, ...);

typedef struct motor_msg {
  uint16_t angle;
  int16_t speed;
  uint16_t torque;
} message;

void can_filter_init();

message motor_receive();
void display(message);
void can_send_motor(uint16_t);
