#include "ist8310_def.h"

#include "i2c.h"
#include "ist8310.h"

#define IST8310_IIC_ADDRESS 0x0E << 1

void ist8310_read(uint16_t reg, uint8_t *data) {
  HAL_I2C_Mem_Read(&hi2c3, 0x0E << 1, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

void ist8310_read_multi(uint16_t reg, uint8_t *data, uint8_t len) {
  HAL_I2C_Mem_Read(&hi2c3, IST8310_IIC_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data,
                   len, 100);
}

void ist8310_write(uint16_t reg, uint8_t data) {
  HAL_I2C_Mem_Write(&hi2c3, IST8310_IIC_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT,
                    &data, 1, 100);
}

void ist8310_write_multi(uint16_t reg, uint8_t *data, uint8_t len) {
  HAL_I2C_Mem_Write(&hi2c3, IST8310_IIC_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT,
                    data, len, 100);
}

void ist8310_reset(void) {
  ist8310_delay_long();
  HAL_GPIO_WritePin(GPIOG, IST8310_RST_Pin, GPIO_PIN_RESET);
  ist8310_delay_long();
  HAL_GPIO_WritePin(GPIOG, IST8310_RST_Pin, GPIO_PIN_SET);
  ist8310_delay_long();
}

void ist8310_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, IST8310_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = IST8310_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = IST8310_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void ist8310_delay_long(void) { HAL_Delay(50); }

void ist8310_delay_short(void) { HAL_Delay(1); }
