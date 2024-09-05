#ifndef __HWT101_H
#define __HWT101_H

#include "stm32f4xx_hal.h"

/// HWT101 initialization
void hwt101_init(void);

/// 角速度数据接收回调函数: dps
void hwt101_angular_callback(short angle);

/// 偏航角数据接收回调函数: d(°)
void hwt101_angle_callback(short angle);

#endif /* __HWT101_H */
