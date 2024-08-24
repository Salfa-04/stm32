#include "blinky.h"

#include <stdint.h>

#include "cmsis_os.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"

void blinky(void const *args) {
  (void)args;

  uint8_t data[10];

  HAL_UARTEx_ReceiveToIdle_DMA();
  HAL_UART_Receive_DMA(&huart2, data, sizeof(data));

  /* Infinite loop */
  for (;;) {
    for (uint8_t i = 0; i < 2; i++) {
      HAL_GPIO_TogglePin(GPIOA, 1 << (6 + i));
      osDelay(100);
    }
  }
}
