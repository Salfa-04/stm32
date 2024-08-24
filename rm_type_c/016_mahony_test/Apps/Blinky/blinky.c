#include "blinky.h"

#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

void blinky(void const* args) {
  (void)args;

  // extern int cnt;

  /* Infinite loop */
  for (;;) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (10 + i));
      osDelay(100);
    }

    // uprintf("OKOK: %d\n", cnt);
    // cnt = 0;
    // osDelay(1000);
  }
}
