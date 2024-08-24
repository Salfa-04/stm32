#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

extern void SystemClock_Config(void);

extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);

#endif /* __MAIN_H */
