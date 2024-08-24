#include "blinky.h"

#include "cmsis_os.h"
#include "imu901.h"
#include "usart.h"

void uprintf(const char *fmt, ...);
__UINT8_TYPE__ buffer[64];

void blinky(void const *args) {
  (void)args;

  imu901_init();

  /* Infinite loop */
  for (;;) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (10 + i));
      osDelay(100);
    }
  }
}

void imu901_buffer_parse(uint8_t id, uint8_t *data, uint8_t len) {
  (void)len;
  angle_t angle;
  accel_t accel;
  gyro_t gyro;

  switch (id) {
    case 0x01: {  // 姿态角 (°)
      angle.roll = (float)((int16_t)(data[1] << 8) | data[0]) / 32768 * 180;
      angle.pitch = (float)((int16_t)(data[3] << 8) | data[2]) / 32768 * 180;
      angle.yaw = (float)((int16_t)(data[5] << 8) | data[4]) / 32768 * 180;
      break;
    }

    case 0x03: {  // 陀螺仪 (°/s)、加速度计 (g)
      accel.x = (float)((int16_t)(data[1] << 8) | data[0]) / 32768 * ACCEL_FSR;
      accel.y = (float)((int16_t)(data[3] << 8) | data[2]) / 32768 * ACCEL_FSR;
      accel.z = (float)((int16_t)(data[5] << 8) | data[4]) / 32768 * ACCEL_FSR;
      gyro.x = (float)((int16_t)(data[7] << 8) | data[6]) / 32768 * GYRO_FSR;
      gyro.y = (float)((int16_t)(data[9] << 8) | data[8]) / 32768 * GYRO_FSR;
      gyro.z = (float)((int16_t)(data[11] << 8) | data[10]) / 32768 * GYRO_FSR;
      break;
    }
  }
}
