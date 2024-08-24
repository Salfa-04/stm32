#include "freertos.h"

#include "cmsis_os.h"
#include "task.h"

#define __weak __attribute__((weak))

osThreadId TEMPHandle;
void imuCtrl(void const *);

void MX_FREERTOS_Init(void) {
  /* add mutexes, ... */

  /* add semaphores, ... */

  /* start timers, add new ones, ... */

  /* add queues, ... */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(imuCtrlTask, imuCtrl, osPriorityNormal, 0, 512);
  TEMPHandle = osThreadCreate(osThread(imuCtrlTask), NULL);

  /* add threads, ... */
}

__weak void imuCtrl(void const *arg) {
  (void)arg;

  /* Infinite loop */
  for (;;) {
    osDelay(100);
  }
}
