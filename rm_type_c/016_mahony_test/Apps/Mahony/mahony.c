#include "mahony.h"

#include "MahonyHook.h"
#include "bmi088.h"
#include "cmsis_os.h"
#include "ist8310.h"

uint8_t gyro_raw[7] = {0};   // offsize = 1
uint8_t accel_raw[8] = {0};  // offsize = 2

bmi088_gyro_t gyro = {0};
bmi088_accel_t accel = {0};
ist8310_t ist8310 = {0};
angle_t angle = {0};

void uprintf(const char* fmt, ...);
_Bool updated = 0;

void mahony(void const* args) {
  (void)args;

  // 初始化外设
  bmi088_dma_init();

  while (bmi088_init()) HAL_Delay(30);
  while (ist8310_init()) HAL_Delay(30);

  /* Infinite loop */
  for (;;) {
    while (!updated);
    updated = 0;

    bmi088_read_gyro_from_raw(&gyro, gyro_raw + 1);
    bmi088_read_accel_from_raw(&accel, accel_raw + 2);

    ahrs_update(&gyro, &accel, &ist8310);
    get_angle(&angle);

    uprintf("yaw=%f,pitch=%f,roll=%f\n", angle.yaw, angle.pitch, angle.roll);
  }
}

static unsigned char mask = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  uint32_t task = taskENTER_CRITICAL_FROM_ISR();

  if (GPIO_Pin == IST8310_DRDY_Pin) {  // 200Hz
    ist8310_read_mag(&ist8310);
    TaskHandle_t xTaskToNotify;
    BaseType_t* pxHigherPriorityTaskWoken;

    vTaskNotifyGiveFromISR(xTaskToNotify, pxHigherPriorityTaskWoken);
  }

  if (GPIO_Pin == INT_Accel_Pin && !(mask & 1)) {
    bmi088_accel_read_dma(accel_raw);
    mask |= 1;
    updated = 1;
  }

  if (GPIO_Pin == INT_Gyro_Pin && !(mask & 2)) {
    bmi088_gyro_read_dma(gyro_raw);
    mask |= 2;
    updated = 1;
  }

  taskEXIT_CRITICAL_FROM_ISR(task);
  if (mask >= 3)  // 800Hz
    mask = 0;
}
