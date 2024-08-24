#include "main.h"

#include <stdint.h>

#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "ist8310.h"
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "usart.h"


int main(void) {
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C3_Init();
  MX_USART1_UART_Init();

  ist8310_init();

  uprintf("Hello, world!\n");

  for (;;) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (i + 10));
      HAL_Delay(100);
    }
  }
}

ist8310_t test = {0};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == IST8310_DRDY_Pin) {
    ist8310_read_mag(&test);
    uprintf("x=%f,y=%f,z=%f\n", test.x, test.y, test.z);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  } /* @type tick = ms */
}
