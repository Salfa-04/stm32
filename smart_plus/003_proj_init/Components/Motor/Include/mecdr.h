#ifndef __MECDR_H
#define __MECDR_H

#include "stm32f4xx_hal.h"

/// 初始化电机编码器
void mecdr_init(void);

/// Called 100Hz by TIM6 interrupt; return cpm
void mecdr_speed_calc(float *a, float *b, float *c, float *d);

/// 获取累积计数值, 单位: rad
/// S = w * R; 计算可得路程, 单位与R相同
/// 参数: a, b, c, d 对应电机的累计值
void mecdr_addup_get(float *a, float *b, float *c, float *d);

/// 清除电机计数值
void mecdr_addup_clear(void);

#endif /* __MECDR_H */
