#include "freertos.h"

#include "cmsis_os.h"
#include "task.h"

osThreadId blinkyTask;
void blinky(void const *);

void freertos_init(void) {
  /* add mutexes, ... */

  /* add semaphores, ... */

  /* start timers, add new ones, ... */

  /* add queues, ... */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(BlinkyTask, blinky, osPriorityNormal, 0, 256);
  blinkyTask = osThreadCreate(osThread(BlinkyTask), NULL);

  /* add threads, ... */
}
