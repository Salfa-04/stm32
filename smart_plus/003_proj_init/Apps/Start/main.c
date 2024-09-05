#include "main.h"

#include "freertos.h"
#include "gpio.h"
#include "mctrl.h"
#include "usart.h"

int main(void) {
  hal_clock_init();
  led_gpio_init();
  uart1_init();

  uprintf("Hello, world!\n");

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

  if (htim->Instance == TIM6) {
    mctrl_distance_callback();  // 位置环pid
    mctrl_velocity_callback();  // 速度环pid
  } /** FREQ: 100Hz */
}
