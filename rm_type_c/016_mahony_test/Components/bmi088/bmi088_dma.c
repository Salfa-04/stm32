#include "bmi088.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal_spi.h"

DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

void bmi088_dma_init(void) {
  /* SPI1_RX Init */
  hdma_spi1_rx.Instance = DMA2_Stream2;
  hdma_spi1_rx.Init.Channel = DMA_CHANNEL_3;
  hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi1_rx.Init.Mode = DMA_NORMAL;
  hdma_spi1_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdma_spi1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_spi1_rx);
  __HAL_LINKDMA(&hspi1, hdmarx, hdma_spi1_rx);

  /* SPI1_TX Init */
  hdma_spi1_tx.Instance = DMA2_Stream3;
  hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;
  hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi1_tx.Init.Mode = DMA_NORMAL;
  hdma_spi1_tx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_spi1_tx);
  __HAL_LINKDMA(&hspi1, hdmatx, hdma_spi1_tx);

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

void DMA2_Stream2_IRQHandler(void) {
  /* DMA2 stream2 global interrupt Handler */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
}

void DMA2_Stream3_IRQHandler(void) {
  /* DMA2 stream3 global interrupt Handler */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

static _Bool gyro_flag = 0;
static _Bool accel_flag = 0;
static _Bool temp_flag = 0;
static uint8_t locked = 0;

static void unlock(void) {
  HAL_SPI_DMAStop(&hspi1);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
  locked = gyro_flag = accel_flag = temp_flag = 0;
}

/// BUFFER OFFSIZE = 1, BUFFER LEN = 7
void bmi088_gyro_read_dma(uint8_t* data) {
  if (accel_flag || gyro_flag || temp_flag) {
    if (locked++ > 233) unlock();
    return;
  }

  gyro_flag = 1;
  uint8_t buffer[7] = {0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive_DMA(&hspi1, buffer, data, 7);
}

/// BUFFER OFFSIZE = 2, BUFFER LEN = 8
void bmi088_accel_read_dma(uint8_t* data) {
  if (accel_flag || gyro_flag || temp_flag) {
    if (locked++ > 233) unlock();
    return;
  }

  accel_flag = 1;
  uint8_t buffer[8] = {0x92, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive_DMA(&hspi1, buffer, data, 8);
}

/// BUFFER OFFSIZE = 2, BUFFER LEN = 4
void bmi088_temp_read_dma(uint8_t* data) {
  if (accel_flag || gyro_flag || temp_flag) {
    if (locked++ > 233) unlock();
    return;
  }

  temp_flag = 1;
  uint8_t buffer[4] = {0xA2, 0xFF, 0xFF, 0xFF};
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive_DMA(&hspi1, buffer, data, 4);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi) {
  if (hspi->Instance == SPI1) {
    if (gyro_flag) {
      HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
      gyro_flag = 0;
    }

    if (accel_flag) {
      HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
      accel_flag = 0;
    }

    if (temp_flag) {
      HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
      temp_flag = 0;
    }
  }
}
