#ifndef __FREERTOS_H
#define __FREERTOS_H

#include "cmsis_os.h"

extern osThreadId defaultTaskHandle;

void MX_FREERTOS_Init(void);

#endif /* __FREERTOS_H */
