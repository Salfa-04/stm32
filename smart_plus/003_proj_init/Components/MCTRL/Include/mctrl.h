#ifndef __MCTRL_H
#define __MCTRL_H

#include "stm32f4xx_hal.h"

extern uint8_t mctrl_mutex;

#define CHASSIS_RA 0.f     // 单位 mm
#define CHASSIS_RB 0.f     // 单位 mm
#define WHEEL_RADIUS 38.f  // 单位 mm

#define __PI 3.14159265358979323846f

void mctrl_init(void);

void mctrl_chassis_control_angle(float angle);
void mctrl_chassis_control_distance(float x, float y);

void mctrl_distance_callback(void);
void mctrl_velocity_callback(void);

#endif /* __MCTRL_H */
