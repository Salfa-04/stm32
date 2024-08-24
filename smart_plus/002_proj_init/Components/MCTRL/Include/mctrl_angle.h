#ifndef __MCTRL_ANGLE_H
#define __MCTRL_ANGLE_H

#include "stm32f4xx_hal.h"

void mctrl_angle_init(void);
void mctrl_chassis_control_angle(float angle);
void imu901_data_callback(uint8_t id, uint8_t *data, uint8_t len);

#endif /* __MCTRL_ANGLE_H */
