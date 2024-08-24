#ifndef __IMU901_H
#define __IMU901_H

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart4;

typedef struct {
  float roll, pitch, yaw;
} angle_t;

typedef struct {
  float x, y, z;
} accel_t, gyro_t;

void imu901_init(void);

void imu901_reg_write(uint8_t reg, uint8_t data);
void imu901_reg_read(uint8_t reg, uint8_t* data, unsigned timeout);
void imu901_buffer_parse(uint8_t id, uint8_t* data, uint8_t len);

#define ACCEL_FSR 2    // 2G 4G 8G 16G
#define GYRO_FSR 2000  // 250DPS 500DPS 1000DPS 2000DPS

#endif /* __IMU901_H */
