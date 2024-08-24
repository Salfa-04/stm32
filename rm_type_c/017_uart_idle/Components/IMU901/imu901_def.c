#include <stdint.h>

#include "cmsis_os.h"
#include "imu901.h"

UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_uart4_rx;

void imu901_dma_init(void) {
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* UART4 TX DMA Init */
  hdma_uart4_tx.Instance = DMA1_Stream4;
  hdma_uart4_tx.Init.Channel = DMA_CHANNEL_4;
  hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_uart4_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_uart4_tx.Init.Mode = DMA_NORMAL;
  hdma_uart4_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdma_uart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  HAL_DMA_Init(&hdma_uart4_tx);
  __HAL_LINKDMA(&huart4, hdmatx, hdma_uart4_tx);

  /* UART4 RX DMA Init */
  hdma_uart4_rx.Instance = DMA1_Stream2;
  hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
  hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_uart4_rx.Init.Mode = DMA_NORMAL;
  hdma_uart4_rx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  HAL_DMA_Init(&hdma_uart4_rx);
  __HAL_LINKDMA(&huart4, hdmarx, hdma_uart4_rx);

  /* DMA Interrupt Init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
}

void imu901_uart_init(void) {
  /* UART4 handler init */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;

  /* UART4 init, and call UART MSP init */
  HAL_UART_Init(&huart4);

  /* UART4 Interrupt Init */
  HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
}

void imu901_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* UART4 Clock Enable */
  __HAL_RCC_UART4_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();
  /**UART4 GPIO Configuration
  PC10     ------> UART4_TX
  PC11     ------> UART4_RX
  */
  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void imu901_reg_write(uint8_t reg, uint8_t data) {
  uint8_t check_sum = 0x05 + reg + data;
  uint8_t tx_buffer[6] = {0x55, 0xAF, reg, 0x01, data, check_sum};
  HAL_UART_Transmit(&huart4, tx_buffer, sizeof(tx_buffer), 1000U);  // edit
}

void imu901_reg_read(uint8_t reg, uint8_t* data, unsigned timeout) {
  uint8_t rx_buffer[6] = {0};
  imu901_reg_write(reg | 0x80, 0x00);
  HAL_UART_Receive(&huart4, rx_buffer, sizeof(rx_buffer), timeout);  // edit

  if (rx_buffer[0] != 0x55 || rx_buffer[1] != 0xAF || rx_buffer[2] != reg ||
      rx_buffer[3] != 0x01 || rx_buffer[5] != 0x05 + reg + rx_buffer[4]) {
    return;
  }

  *data = rx_buffer[4];
}

void UART4_IRQHandler(void) {
  /* UART4 global interrupt handler */
  HAL_UART_IRQHandler(&huart4);
}

void DMA1_Stream4_IRQHandler(void) {
  /* DMA2 stream7 global interrupt handler */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
}

void DMA1_Stream2_IRQHandler(void) {
  /* DMA2 stream2 global interrupt handler */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
}

void imu901_delay_short(void) { osDelay(1); }
void imu901_delay_long(void) { osDelay(80); }
