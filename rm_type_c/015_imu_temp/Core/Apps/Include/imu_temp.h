#ifndef __IMU_TEMP_H
#define __IMU_TEMP_H

#define IMU_PWM_SET(pwm) \
  __HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, pwm)  // 5000max

#endif /* __IMU_TEMP_H */
