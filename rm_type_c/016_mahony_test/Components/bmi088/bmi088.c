#include "bmi088.h"

#include "bmi088_reg.h"
#include "main.h"
#include "usart.h"

void bmi088_spi_init(void);
void bmi088_gpio_init(void);
void bmi088_delay_long(void);
void bmi088_delay_short(void);

void bmi088_accel_write(uint8_t reg, uint8_t data);
void bmi088_gyro_write(uint8_t reg, uint8_t data);
void bmi088_accel_read(uint8_t reg, uint8_t* data);
void bmi088_gyro_read(uint8_t reg, uint8_t* data);
void bmi088_accel_read_multi(uint8_t reg, uint8_t* data, uint8_t len);
void bmi088_gyro_read_multi(uint8_t reg, uint8_t* data, uint8_t len);

// #define PANIC()                                        \
//   while (1) {                                          \
//     uprintf("Paniced at %s:%d\n", __FILE__, __LINE__); \
//     HAL_Delay(300);                                    \
//   }

#define PANIC() return 1

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

uint8_t bmi088_accel_init(void) {
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

  return 0;
}

uint8_t bmi088_gyro_init(void) {
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

  return 0;
}

uint8_t bmi088_init(void) {
  bmi088_spi_init();
  bmi088_gpio_init();

  if (bmi088_accel_init()) PANIC();
  if (bmi088_gyro_init()) PANIC();

  return 0;
}

void bmi088_read_accel(bmi088_accel_t* accel) {
  int16_t temp_raw;
  uint8_t buf[6] = {0};

  bmi088_accel_read_multi(BMI088_ACCEL_XOUT_L, buf, 6);
  temp_raw = (int16_t)((buf[1]) << 8) | buf[0];
  accel->x = temp_raw * BMI088_ACCEL_SEN;
  temp_raw = (int16_t)((buf[3]) << 8) | buf[2];
  accel->y = temp_raw * BMI088_ACCEL_SEN;
  temp_raw = (int16_t)((buf[5]) << 8) | buf[4];
  accel->z = temp_raw * BMI088_ACCEL_SEN;
}

void bmi088_read_accel_from_raw(bmi088_accel_t* accel, uint8_t* raw) {
  int16_t temp_raw;

  temp_raw = (int16_t)((raw[1]) << 8) | raw[0];
  accel->x = temp_raw * BMI088_ACCEL_SEN;
  temp_raw = (int16_t)((raw[3]) << 8) | raw[2];
  accel->y = temp_raw * BMI088_ACCEL_SEN;
  temp_raw = (int16_t)((raw[5]) << 8) | raw[4];
  accel->z = temp_raw * BMI088_ACCEL_SEN;
}

void bmi088_read_gyro(bmi088_gyro_t* gyro) {
  int16_t temp_raw;
  uint8_t buf[6] = {0};

  bmi088_gyro_read_multi(BMI088_GYRO_X_L, buf, 6);
  temp_raw = (int16_t)((buf[1]) << 8) | buf[0];
  gyro->x = temp_raw * BMI088_GYRO_SEN;
  temp_raw = (int16_t)((buf[3]) << 8) | buf[2];
  gyro->y = temp_raw * BMI088_GYRO_SEN;
  temp_raw = (int16_t)((buf[5]) << 8) | buf[4];
  gyro->z = temp_raw * BMI088_GYRO_SEN;
}

void bmi088_read_gyro_from_raw(bmi088_gyro_t* gyro, uint8_t* raw) {
  int16_t temp_raw;

  temp_raw = (int16_t)((raw[1]) << 8) | raw[0];
  gyro->x = temp_raw * BMI088_GYRO_SEN;
  temp_raw = (int16_t)((raw[3]) << 8) | raw[2];
  gyro->y = temp_raw * BMI088_GYRO_SEN;
  temp_raw = (int16_t)((raw[5]) << 8) | raw[4];
  gyro->z = temp_raw * BMI088_GYRO_SEN;
}

void bmi088_read_temp(float* temp) {
  int16_t temp_raw;
  uint8_t buf[2] = {0};

  bmi088_accel_read_multi(BMI088_TEMP_M, buf, 2);
  temp_raw = (int16_t)((buf[0] << 3) | (buf[1] >> 5));
  if (temp_raw > 1023) temp_raw -= 2048;
  *temp = temp_raw * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
}

void bmi088_read_temp_from_raw(float* temp, uint8_t* raw) {
  int16_t temp_raw;

  temp_raw = (int16_t)((raw[0] << 3) | (raw[1] >> 5));
  if (temp_raw > 1023) temp_raw -= 2048;
  *temp = temp_raw * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
}

void bmi088_read_all(bmi088_t* bmi088) {
  bmi088_read_accel(&bmi088->accel);
  bmi088_read_gyro(&bmi088->gyro);
  bmi088_read_temp(&bmi088->temp);
}
