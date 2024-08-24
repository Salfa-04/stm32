#ifndef __IMU901_H
#define __IMU901_H

#include "stm32f4xx_hal.h"

typedef struct {
  float roll, pitch, yaw;
} angle_t;

/// 初始化IMU901
void imu901_init(void);

/// 向IMU901发送数据
void imu901_reg_write(uint8_t reg, uint8_t data);
/// 从IMU901读取数据
void imu901_reg_read(uint8_t reg, uint8_t* data, unsigned timeout);
/// 获取IMU901主动上传的数据
void imu901_data_callback(uint8_t id, uint8_t* data, uint8_t len);

#endif /* __IMU901_H */
