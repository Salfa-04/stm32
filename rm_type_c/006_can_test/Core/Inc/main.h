#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void Error_Handler(void);

#define R_Pin GPIO_PIN_12
#define R_GPIO_Port GPIOH
#define B_Pin GPIO_PIN_11
#define B_GPIO_Port GPIOH
#define G_Pin GPIO_PIN_10
#define G_GPIO_Port GPIOH

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
