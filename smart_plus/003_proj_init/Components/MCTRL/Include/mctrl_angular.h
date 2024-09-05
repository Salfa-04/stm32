#ifndef __MCTRL_ANGULAR_H
#define __MCTRL_ANGULAR_H

#include "stm32f4xx_hal.h"

void mctrl_angular_init(void);
void mctrl_chassis_control_angular(float angle);
void hwt101_angular_callback(short angle);

#endif /* __MCTRL_ANGLE_ULAR*/
