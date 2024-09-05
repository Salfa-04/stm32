#include "blinky.h"

#include "cmsis_os.h"
#include "mctrl.h"

void blinky(void const *args) {
  (void)args;

  mctrl_init();

  /* Infinite loop */
  for (;;) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    osDelay(100);
  }
}
