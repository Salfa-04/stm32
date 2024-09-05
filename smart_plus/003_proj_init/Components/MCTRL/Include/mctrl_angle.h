#ifndef __MCTRL_ANGLE_H
#define __MCTRL_ANGLE_H

#include "stm32f4xx_hal.h"

void mctrl_angle_init(void);
void mctrl_chassis_control_angle(float angle);
void hwt101_angle_callback(short angle);

#endif /* __MCTRL_ANGLE_H */
