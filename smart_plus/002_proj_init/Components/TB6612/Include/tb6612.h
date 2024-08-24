#ifndef __TB6612_H
#define __TB6612_H

#include "stm32f4xx_hal.h"

/// 电机控制初始化
void tb6612_init(void);

/// 电机滑行 tb6612
void tb6612_motor_stop(void);
/// 电机刹车 tb6612
void tb6612_motor_brack(void);
/// 电机控制 tb6612; 参数为PWM值: 0 ~ 5600, 60% = 3360
void tb6612_motor_pwm(int16_t a, int16_t b, int16_t c, int16_t d);
/// 设置电机转动方向, 将参数修改为绝对值
void tb6612_motor_direct(int16_t *a, int16_t *b, int16_t *c, int16_t *d);

/// pwm; 60% = 3360; 100% = 5600; 0 ~ 5600
#define TB6612_MOTORA_PWM_SET(x) TIM9->CCR2 = (x)
#define TB6612_MOTORB_PWM_SET(x) TIM9->CCR1 = (x)
#define TB6612_MOTORC_PWM_SET(x) TIM12->CCR1 = (x)
#define TB6612_MOTORD_PWM_SET(x) TIM12->CCR2 = (x)

#endif /* __TB6612_H */
