#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

extern void SystemClock_Config(void);

extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);

#define CS_Accel_Pin GPIO_PIN_4
#define CS_Accel_GPIO_Port GPIOA
#define CS_Gyro_Pin GPIO_PIN_0
#define CS_Gyro_GPIO_Port GPIOB

#define INT_Accel_Pin GPIO_PIN_4
#define INT_Accel_GPIO_Port GPIOC
#define INT_Accel_EXTI_IRQn EXTI4_IRQn
#define INT_Gyro_Pin GPIO_PIN_5
#define INT_Gyro_GPIO_Port GPIOC
#define INT_Gyro_EXTI_IRQn EXTI9_5_IRQn

#endif /* __MAIN_H */
