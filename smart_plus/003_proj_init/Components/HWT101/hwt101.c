#include "hwt101.h"

#define UART_USED huart6

void hwt101_uart_init(void);
void hwt101_gpio_init(void);
void hwt101_dma_init(void);
void hwt101_delay_long(void);
void hwt101_delay_short(void);
extern UART_HandleTypeDef UART_USED;

static uint8_t buffer[18];

uint8_t hwt101_read_ok(unsigned timeout) {
  uint8_t rx_buffer[24] = {0}, offsize = 0;

  HAL_UART_Receive(&UART_USED, rx_buffer, sizeof(rx_buffer), timeout);

  for (; offsize < sizeof(rx_buffer) - 10; offsize++)
    if (rx_buffer[offsize] == 0x55 && ((rx_buffer[offsize + 1] & ~1) == 0x52))
      return 1;

  return 0;
}

void hwt101_init(void) {
  hwt101_uart_init();
  hwt101_gpio_init();
  hwt101_dma_init();

  hwt101_delay_long();

  while (!hwt101_read_ok(1000));

  HAL_UARTEx_ReceiveToIdle_DMA(&UART_USED, buffer, sizeof(buffer));
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == USART6 /* EDIT */) {  // !!! EDIT
    uint8_t check_sum = 0x55;
    for (uint8_t i = 1; i < Size - 1; i++) check_sum += buffer[i];
    if (Size == 11 && buffer[0] == 0x55 && check_sum == buffer[10]) {
      short temp = (short)buffer[7] << 8 | buffer[6];
      if (buffer[1] == 0x52)  // 角速度: dps
        hwt101_angular_callback(temp * 2000 / 32768);

      if (buffer[1] == 0x53)  // 偏航角: d(°)
        hwt101_angle_callback(temp * 180 / 32768);
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&UART_USED, buffer, sizeof(buffer));
  }
}

/// 角速度数据接收回调函数: dps
__weak void hwt101_angular_callback(short angle) { (void)angle; }

/// 偏航角数据接收回调函数: d(°)
__weak void hwt101_angle_callback(short angle) { (void)angle; }
