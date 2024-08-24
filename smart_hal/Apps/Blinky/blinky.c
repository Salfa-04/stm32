#include "blinky.h"

#include "cmsis_os.h"

void blinky(void const *args) {
  (void)args;

  /* Infinite loop */
  for (;;) {
    for (uint8_t i = 0; i < 2; i++) {
      HAL_GPIO_TogglePin(GPIOA, 1 << (6 + i));
      osDelay(100);
    }
  }
}
