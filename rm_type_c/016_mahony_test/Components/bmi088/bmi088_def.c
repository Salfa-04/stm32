#include "bmi088.h"
#include "freertos.h"
#include "stm32f4xx_hal_spi.h"

SPI_HandleTypeDef hspi1;

void bmi088_read(uint8_t tx_data, uint8_t* rx_data) {
  HAL_SPI_TransmitReceive(&hspi1, &tx_data, rx_data, 1, 1000);
}

void bmi088_write(uint8_t reg, uint8_t data) {
  uint8_t temp[] = {0};
  bmi088_read(reg, temp);
  bmi088_read(data, temp);
}

void bmi088_read_multi(uint8_t reg, uint8_t* data, uint8_t len) {
  bmi088_read(reg | 0x80, data);
  for (; len > 0; len--) bmi088_read(0x55, data++);
}

void bmi088_accel_write(uint8_t reg, uint8_t data) {
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_RESET);
  bmi088_write(reg, data);
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
}

void bmi088_gyro_write(uint8_t reg, uint8_t data) {
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  bmi088_write(reg, data);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
}

void bmi088_accel_read(uint8_t reg, uint8_t* data) {
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_RESET);
  bmi088_read(reg | 0x80, data);
  bmi088_read(0x55, data);
  bmi088_read(0x55, data);
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
}

void bmi088_gyro_read(uint8_t reg, uint8_t* data) {
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  bmi088_read(reg | 0x80, data);
  bmi088_read(0x55, data);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
}

void bmi088_accel_read_multi(uint8_t reg, uint8_t* data, uint8_t len) {
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_RESET);
  bmi088_read(reg | 0x80, data);
  bmi088_read_multi(reg, data, len);
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
}

void bmi088_gyro_read_multi(uint8_t reg, uint8_t* data, uint8_t len) {
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  bmi088_read_multi(reg, data, len);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
}

void bmi088_spi_init(void) {
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;

  HAL_SPI_Init(&hspi1);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (spiHandle->Instance == SPI1) {
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PB4     ------> SPI1_MISO
    PB3     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void bmi088_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CS_Accel_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_Accel_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = INT_Accel_Pin | INT_Gyro_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CS_Gyro_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS_Gyro_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI4_IRQHandler(void) {
  /* EXTI line4 interrupt Handler */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void) {
  /* EXTI line[9:5] interrupt Handler */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void bmi088_delay_long(void) { osDelay(80); }
void bmi088_delay_short(void) { osDelay(1); }
