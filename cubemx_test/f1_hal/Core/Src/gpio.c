#include "gpio.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_gpio.h"

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */

void MX_GPIO_Init(void) {

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
}

void GPIO_Init() {
  GPIO_InitTypeDef gpio_init = {0};

  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pin = GPIO_PIN_13;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOC, &gpio_init);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}
