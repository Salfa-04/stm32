#include "main.h"

#include "freertos.h"
#include "gpio.h"
#include "usart.h"

void hal_clock_init(void);

int main(void) {
  hal_clock_init();
  led_gpio_init();
  uart1_init();

  uprintf("Hello, world!\n");

  // 初始化系统
  freertos_init();
  osKernelStart();

  for (;;) {
    /* We should never get here */
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  } /* @type tick = ms */
}
