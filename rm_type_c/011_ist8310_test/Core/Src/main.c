#include "main.h"

#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "ist8310.h"
#include "usart.h"

ist8310_t test = {0};

int main(void) {
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C3_Init();
  MX_USART1_UART_Init();

  ist8310_init();

  while (1) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (10 + i));
      HAL_Delay(100);
    }
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == IST8310_DRDY_Pin) {
    ist8310_read_mag(&test);
    uprintf("x=%f,y=%f,z=%f\n", test.x, test.y, test.z);
  }
}
