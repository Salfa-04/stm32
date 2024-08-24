#include "imu_temp.h"

#include "bmi088.h"
#include "cmsis_os.h"
#include "main.h"
#include "pid.h"
#include "tim.h"
#include "usart.h"

#define TEMPERATURE_PID_KP 1600.0f  // kp of temperature control PID
#define TEMPERATURE_PID_KI 0.2f     // ki of temperature control PID
#define TEMPERATURE_PID_KD 0.0f     // kd of temperature control PID

#define TEMPERATURE_PID_MAX_OUT 4500.0f   // max out of temperature control PID
#define TEMPERATURE_PID_MAX_IOUT 4400.0f  // max iout of temperature control PID

extern SPI_HandleTypeDef hspi1;

// task handler 任务句柄
TaskHandle_t INS_task_local_handler;

volatile uint8_t imu_start_flag = 0;

bmi088_t bmi088 = {0};
pid_t imu_temp_pid;

/**
 * @brief          bmi088温度控制
 * @param[in]      argument: NULL
 * @retval         none
 */
void imu_temp_control_task(void const* args) {
  (void)args;
  osDelay(500);

  // pid init  PID初始化
  PID_Init(&imu_temp_pid, TEMPERATURE_PID_KP, TEMPERATURE_PID_KI,
           TEMPERATURE_PID_KD, TEMPERATURE_PID_MAX_IOUT,
           TEMPERATURE_PID_MAX_OUT);

  // bmi088 init. bmi088初始化
  bmi088_init();

  // set spi frequency
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  HAL_SPI_Init(&hspi1);

  // 获取任务句柄，必须在cubeMX使能'xTaskGetHandle'
  INS_task_local_handler = xTaskGetHandle(pcTaskGetName(NULL));
  imu_start_flag = 1;
  while (1) {
    // wait for task waked up
    // 等待任务被唤醒
    while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS) {
    }
    // read data.读取数据
    // BMI088_read(gyro, accel, &temp);
    bmi088_read_temp(&bmi088);

    uint16_t tempPWM;
    // pid calculate. PID计算
    update_pid(&imu_temp_pid, bmi088.temp, 40.0f);
    uprintf("temp=%f\n", bmi088.temp);

    if (imu_temp_pid.output < 0.0f) {
      imu_temp_pid.output = 0.0f;
    }

    tempPWM = (uint16_t)imu_temp_pid.output;
    IMU_PWM_SET(tempPWM);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == INT_Accel_Pin) {
    if (imu_start_flag) {
      // 唤醒任务
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        static BaseType_t xHigherPriorityTaskWoken;
        vTaskNotifyGiveFromISR(INS_task_local_handler,
                               &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
      }
    }
  }

  if (GPIO_Pin == INT_Gyro_Pin) {
  }
}

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//   if (GPIO_Pin == INT_Accel_Pin) {
//     bmi088_read_accel(&test);
//     uprintf("accelx=%f,accely=%f,accelz=%f\n", test.accel.x, test.accel.y,
//             test.accel.z);
//   }

//   if (GPIO_Pin == INT_Gyro_Pin) {
//     bmi088_read_gyro(&test);
//     uprintf("gyrox=%f,gyroy=%f,gyroz=%f\n", test.gyro.x, test.gyro.y,
//             test.gyro.z);
//   }
// }
