#include "main.h"

#include "bmi088.h"
#include "dma.h"
#include "freertos.h"
#include "gpio.h"
#include "imu_temp.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

bmi088_t test = {0};

int main(void) {
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();

  MX_TIM10_Init();
  MX_SPI1_Init();
  bmi088_init();

  MX_FREERTOS_Init();

  uprintf("Hello, world!\n");

  osKernelStart();
  for (;;) {
    /* We should never get here */
  }
}

void imuCtrl(void const *arg) {
  (void)arg;

  __volatile static uint16_t temp_pwm = 0;

  /* Infinite loop */
  for (;;) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (10 + i));
      osDelay(50);
    }

    IMU_PWM_SET(temp_pwm);

    bmi088_read_temp(&test);
    uprintf("temp=%f\n", test.temp);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  } /* @type tick = ms */
}
