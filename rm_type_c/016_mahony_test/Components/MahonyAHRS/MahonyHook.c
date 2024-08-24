#include "MahonyHook.h"

#include "MahonyAHRS.h"

extern float atan2f(float, float);
extern float asinf(float);

void ahrs_update(bmi088_gyro_t* gyro, bmi088_accel_t* accel, ist8310_t* mag) {
  MahonyAHRSupdate(gyro->x, gyro->y, gyro->z, accel->x, accel->y, accel->z,
                   mag->x, mag->y, mag->z);
}

void ahrs_update_imu(bmi088_gyro_t* gyro, bmi088_accel_t* accel) {
  MahonyAHRSupdateIMU(gyro->x, gyro->y, gyro->z, accel->x, accel->y, accel->z);
}

void get_angle(angle_t* angle) {
  angle->yaw =
      atan2f(2.0f * (q0 * q3 + q1 * q2), 2.0f * (q0 * q0 + q1 * q1) - 1.0f) *
      100;
  angle->pitch = asinf(-2.0f * (q1 * q3 - q0 * q2)) * 100;
  angle->roll =
      atan2f(2.0f * (q0 * q1 + q2 * q3), 2.0f * (q0 * q0 + q3 * q3) - 1.0f) *
      100;
}

// void get_angle(angle_t* angle) {
//   angle->yaw = atan2f(q0 * q3 - q1 * q2, q0 * q0 + q2 * q2 - 0.5f);
//   angle->pitch = atan2f(q1 * q2 - q1 * q3, q0 * q0 + q3 * q3 - 0.5f);
//   angle->roll = asinf(2.0f * (q0 * q1 + q2 * q3));
// }
