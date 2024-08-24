#ifndef __BMI088_DEF_H
#define __BMI088_DEF_H

#include "main.h"

void bmi088_delay_long(void);
void bmi088_delay_short(void);

void bmi088_read(uint8_t tx_data, uint8_t* rx_data);
void bmi088_write(uint8_t reg, uint8_t data);
void bmi088_read_multi(uint8_t reg, uint8_t* data, uint8_t len);

void bmi088_accel_write(uint8_t reg, uint8_t data);
void bmi088_gyro_write(uint8_t reg, uint8_t data);

void bmi088_accel_read(uint8_t reg, uint8_t* data);
void bmi088_gyro_read(uint8_t reg, uint8_t* data);

void bmi088_accel_read_multi(uint8_t reg, uint8_t* data, uint8_t len);
void bmi088_gyro_read_multi(uint8_t reg, uint8_t* data, uint8_t len);

#endif /* __BMI088_DEF_H */
