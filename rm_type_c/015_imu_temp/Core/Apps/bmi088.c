#include "bmi088.h"

#include "bmi088_def.h"
#include "bmi088_reg.h"
#include "main.h"
#include "usart.h"

#define PANIC()                                        \
  while (1) {                                          \
    uprintf("Paniced at %s:%d\n", __FILE__, __LINE__); \
    HAL_Delay(300);                                    \
  }

#define BMI088_ACCEL_CMD_NUM 6
#define BMI088_GYRO_CMD_NUM 6

float BMI088_ACCEL_SEN = BMI088_ACCEL_3G_SEN;
float BMI088_GYRO_SEN = BMI088_GYRO_2000_SEN;

const uint8_t bmi088_aclel_init_cmd[BMI088_ACCEL_CMD_NUM][2] = {
    {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON},
    {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE},
    {BMI088_ACC_CONF,
     BMI088_ACC_NORMAL | BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set},
    {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G},
    {BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP |
                              BMI088_ACC_INT1_GPIO_LOW},
    {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT},
};

const uint8_t bmi088_gyro_init_cmd[BMI088_GYRO_CMD_NUM][2] = {
    {BMI088_GYRO_RANGE, BMI088_GYRO_2000},
    {BMI088_GYRO_BANDWIDTH,
     BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set},
    {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE},
    {BMI088_GYRO_CTRL, BMI088_DRDY_ON},
    {BMI088_GYRO_INT3_INT4_IO_CONF,
     BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW},
    {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3},
};

void bmi088_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_Accel_GPIO_Port, CS_Accel_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(CS_Gyro_GPIO_Port, CS_Gyro_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PF6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

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
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void bmi088_accel_init(void) {
  uint8_t res = {0};

  // check commiunication
  bmi088_accel_read(BMI088_ACC_CHIP_ID, &res);
  bmi088_delay_short();
  bmi088_accel_read(BMI088_ACC_CHIP_ID, &res);
  bmi088_delay_short();

  // accel software reset
  bmi088_accel_write(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
  bmi088_delay_long();

  // check commiunication is normal after reset
  bmi088_accel_read(BMI088_ACC_CHIP_ID, &res);
  bmi088_delay_short();
  bmi088_accel_read(BMI088_ACC_CHIP_ID, &res);
  bmi088_delay_short();

  // check the "who am I"
  if (res != BMI088_ACC_CHIP_ID_VALUE) PANIC();

  // set accel sonsor config and check
  for (uint8_t writeNum = 0; writeNum < BMI088_ACCEL_CMD_NUM; writeNum++) {
    bmi088_accel_write(bmi088_aclel_init_cmd[writeNum][0],
                       bmi088_aclel_init_cmd[writeNum][1]);
    bmi088_delay_short();

    bmi088_accel_read(bmi088_aclel_init_cmd[writeNum][0], &res);
    bmi088_delay_short();

    if (res != bmi088_aclel_init_cmd[writeNum][1]) PANIC();
  }
}

void bmi088_gyro_init(void) {
  uint8_t res = {0};

  // check commiunication
  bmi088_gyro_read(BMI088_GYRO_CHIP_ID, &res);
  bmi088_delay_short();
  bmi088_gyro_read(BMI088_GYRO_CHIP_ID, &res);
  bmi088_delay_short();

  // reset the gyro sensor
  bmi088_gyro_write(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
  bmi088_delay_long();

  // check commiunication is normal after reset
  bmi088_gyro_read(BMI088_GYRO_CHIP_ID, &res);
  bmi088_delay_short();
  bmi088_gyro_read(BMI088_GYRO_CHIP_ID, &res);
  bmi088_delay_short();

  // check the "who am I"
  if (res != BMI088_GYRO_CHIP_ID_VALUE) PANIC();

  // set gyro sonsor config and check
  for (uint8_t writeNum = 0; writeNum < BMI088_GYRO_CMD_NUM; writeNum++) {
    bmi088_gyro_write(bmi088_gyro_init_cmd[writeNum][0],
                      bmi088_gyro_init_cmd[writeNum][1]);
    bmi088_delay_short();

    bmi088_gyro_read(bmi088_gyro_init_cmd[writeNum][0], &res);
    bmi088_delay_short();

    if (res != bmi088_gyro_init_cmd[writeNum][1]) PANIC();
  }
}

void bmi088_init(void) {
  bmi088_gpio_init();
  bmi088_accel_init();
  bmi088_gyro_init();
}

void bmi088_read_accel(bmi088_t* bmi088) {
  int16_t temp_raw;
  uint8_t buf[8] = {0};

  bmi088_accel_read_multi(BMI088_ACCEL_XOUT_L, buf, 6);
  temp_raw = (int16_t)((buf[1]) << 8) | buf[0];
  bmi088->accel.x = temp_raw * BMI088_ACCEL_SEN;
  temp_raw = (int16_t)((buf[3]) << 8) | buf[2];
  bmi088->accel.y = temp_raw * BMI088_ACCEL_SEN;
  temp_raw = (int16_t)((buf[5]) << 8) | buf[4];
  bmi088->accel.z = temp_raw * BMI088_ACCEL_SEN;
}

void bmi088_read_gyro(bmi088_t* bmi088) {
  int16_t temp_raw;
  uint8_t buf[8] = {0};

  bmi088_gyro_read_multi(BMI088_GYRO_CHIP_ID, buf, 8);
  temp_raw = (int16_t)((buf[3]) << 8) | buf[2];
  bmi088->gyro.x = temp_raw * BMI088_GYRO_SEN;
  temp_raw = (int16_t)((buf[5]) << 8) | buf[4];
  bmi088->gyro.y = temp_raw * BMI088_GYRO_SEN;
  temp_raw = (int16_t)((buf[7]) << 8) | buf[6];
  bmi088->gyro.z = temp_raw * BMI088_GYRO_SEN;
}

void bmi088_read_temp(bmi088_t* bmi088) {
  int16_t temp_raw;
  uint8_t buf[8] = {0};

  bmi088_accel_read_multi(BMI088_TEMP_M, buf, 2);
  temp_raw = (int16_t)((buf[0] << 3) | (buf[1] >> 5));
  if (temp_raw > 1023) temp_raw -= 2048;
  bmi088->temp = temp_raw * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
}

void bmi088_read_all(bmi088_t* bmi088) {
  bmi088_read_accel(bmi088);
  bmi088_read_gyro(bmi088);
  bmi088_read_temp(bmi088);
}
