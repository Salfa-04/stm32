#include "cmsis_os.h"
#include "hwt101.h"

UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart6_rx;

void hwt101_dma_init(void) {
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* UART4 RX DMA Init */
  hdma_usart6_rx.Instance = DMA2_Stream2;
  hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
  hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart6_rx.Init.Mode = DMA_NORMAL;
  hdma_usart6_rx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  HAL_DMA_Init(&hdma_usart6_rx);
  __HAL_LINKDMA(&huart6, hdmarx, hdma_usart6_rx);

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
}

void hwt101_uart_init(void) {
  /* UART6 handler init */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;

  /* UART4 init, and call UART MSP init */
  HAL_UART_Init(&huart6);
}

void hwt101_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* USART6 clock enable */
  __HAL_RCC_USART6_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();
  /**USART6 GPIO Configuration
  PC6     ------> USART6_TX
  PC7     ------> USART6_RX
  */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void DMA2_Stream2_IRQHandler(void) {
  /* DMA2 stream2 global interrupt handler */
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}

void hwt101_delay_short(void) { osDelay(1); }
void hwt101_delay_long(void) { osDelay(80); }
