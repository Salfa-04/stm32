#ifndef __FREERTOS_H
#define __FREERTOS_H

#include "cmsis_os.h"

extern osThreadId blinkyTask;

void freertos_init(void);

#endif /* __FREERTOS_H */
