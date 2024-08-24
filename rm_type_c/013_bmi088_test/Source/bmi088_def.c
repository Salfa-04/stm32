#include "bmi088_def.h"

#include "spi.h"

inline void bmi088_read(uint8_t tx_data, uint8_t* rx_data) {
  HAL_SPI_TransmitReceive(&hspi1, &tx_data, rx_data, 1, 1000);
}

inline void bmi088_write(uint8_t reg, uint8_t data) {
  uint8_t temp[] = {0};
  bmi088_read(reg, temp);
  bmi088_read(data, temp);
}

inline void bmi088_read_multi(uint8_t reg, uint8_t* data, uint8_t len) {
  bmi088_read(reg | 0x80, data);
  for (; len > 0; len--) bmi088_read(0x55, data++);
}

inline void bmi088_accel_write(uint8_t reg, uint8_t data) {
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_RESET);
  bmi088_write(reg, data);
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
}

inline void bmi088_gyro_write(uint8_t reg, uint8_t data) {
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  bmi088_write(reg, data);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
}

inline void bmi088_accel_read(uint8_t reg, uint8_t* data) {
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_RESET);
  bmi088_read(reg | 0x80, data);
  bmi088_read(0x55, data);
  bmi088_read(0x55, data);
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
}

inline void bmi088_gyro_read(uint8_t reg, uint8_t* data) {
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  bmi088_read(reg | 0x80, data);
  bmi088_read(0x55, data);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
}

inline void bmi088_accel_read_multi(uint8_t reg, uint8_t* data, uint8_t len) {
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_RESET);
  bmi088_read(reg | 0x80, data);
  bmi088_read_multi(reg, data, len);
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
}

inline void bmi088_gyro_read_multi(uint8_t reg, uint8_t* data, uint8_t len) {
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_RESET);
  bmi088_read_multi(reg, data, len);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);
}

void bmi088_delay_long(void) { HAL_Delay(80); }

void bmi088_delay_short(void) {
  uint32_t ticks = 150 * 168;
  uint32_t reload = SysTick->LOAD;
  uint32_t told = 0, tnow = 0, tcnt = 0;
  told = SysTick->VAL;
  while (1) {
    tnow = SysTick->VAL;
    if (tnow != told) {
      if (tnow < told)
        tcnt += told - tnow;
      else
        tcnt += reload - tnow + told;
      told = tnow;
      if (tcnt >= ticks) break;
    }
  }
}
