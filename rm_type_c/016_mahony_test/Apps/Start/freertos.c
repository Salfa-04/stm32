#include "freertos.h"

#include "cmsis_os.h"
#include "task.h"

/// LED Ctrl Task
osThreadId blinkyTask;
void blinky(void const *);

/// MahonyAHRS Task
osThreadId mahonyTask;
void mahony(void const *);

void freertos_init(void) {
  /* add mutexes, ... */

  /* add semaphores, ... */

  /* start timers, add new ones, ... */

  /* add queues, ... */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(BlinkyTask, blinky, osPriorityNormal, 0, 512);
  blinkyTask = osThreadCreate(osThread(BlinkyTask), NULL);

  osThreadDef(MahonyTask, mahony, osPriorityNormal, 0, 512);
  mahonyTask = osThreadCreate(osThread(MahonyTask), NULL);

  /* add threads, ... */
}
