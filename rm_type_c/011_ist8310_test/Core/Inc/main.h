#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

extern void SystemClock_Config(void);

extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);

/// GPIOG
#define IST8310_RST_Pin GPIO_PIN_6
#define IST8310_DRDY_Pin GPIO_PIN_3

#endif /* __MAIN_H */
