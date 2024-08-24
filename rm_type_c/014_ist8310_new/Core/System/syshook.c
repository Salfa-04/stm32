#include "usart.h"

extern int vsnprintf(char *, unsigned int, const char *, __builtin_va_list);

void uprintf(const char *format, ...) {
  if (HAL_DMA_GetState(&hdma_usart1_tx) == HAL_DMA_STATE_BUSY) return;

  __builtin_va_list arg;
  __builtin_va_start(arg, format);
  static uint8_t buffer[255] = {0};
  uint8_t len = vsnprintf((char *)buffer, sizeof(buffer), format, arg);
  __builtin_va_end(arg);

  HAL_UART_Transmit_DMA(&huart1, buffer, len);
}
