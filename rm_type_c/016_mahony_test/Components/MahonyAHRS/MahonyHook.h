#ifndef __MAHONY_HOOK_H
#define __MAHONY_HOOK_H

#include "bmi088.h"
#include "ist8310.h"

typedef struct {
  float yaw, pitch, roll;
} angle_t;

void get_angle(angle_t* angle);
void ahrs_update_imu(bmi088_gyro_t* gyro, bmi088_accel_t* accel);
void ahrs_update(bmi088_gyro_t* gyro, bmi088_accel_t* accel, ist8310_t* mag);

#endif /* __MAHONY_HOOK_H */
