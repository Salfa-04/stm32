#include "main.h"

#include "bmi088.h"
#include "dma.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"

bmi088_t test = {0};

int main(void) {
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();

  bmi088_init();

  uprintf("Hello, world!\n");
  HAL_Delay(300);

  while (1) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (10 + i));
      HAL_Delay(100);
    }

    bmi088_read_temp(&test);
    uprintf("temp=%f\n", test.temp);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == INT_Accel_Pin) {
    bmi088_read_accel(&test);
    uprintf("accelx=%f,accely=%f,accelz=%f\n", test.accel.x, test.accel.y,
            test.accel.z);
  }

  if (GPIO_Pin == INT_Gyro_Pin) {
    bmi088_read_gyro(&test);
    uprintf("gyrox=%f,gyroy=%f,gyroz=%f\n", test.gyro.x, test.gyro.y,
            test.gyro.z);
  }
}
