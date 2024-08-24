#include "main.h"
#include "can.h"
#include "dma.h"
#include "gpio.h"
#include "source.h"
#include "usart.h"
#include <stdint.h>

/// [0] << 4 = TargetSpeed,
/// [1,2,3] = PID_Kp,
/// [4,5,6] = PID_Ki,
/// [7,8,9] = PID_Kd,
uint8_t pid_target[1] = {0};

motor_t m2006 = {0};
pid_t pid = {0};

const float H_PID[3] = {
    // 00 20 00 | 00 01 ff | 00 5f 00
    // 00 19 08 | 00 00 b1 | 00 2b 03
    // 00 18 fe | 00 03 fe | 00 3e 00
    (float)0x0018fe / 1000, // P
    (float)0x0003fe / 1000, // I
    (float)0x003e00 / 1000, // D
};

int main(void) {

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_USART1_UART_Init();

  uprintf("HELLO WORLD\r\n");
  HAL_Delay(800);

  HAL_CAN_Start(&hcan1);
  PID_Init(&pid, H_PID[0], H_PID[1], H_PID[2], 0x1388, 0x2710);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

  HAL_UART_Receive_IT(&huart1, pid_target, sizeof(pid_target));

  while (1) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (10 + i));
      HAL_Delay(100);
    }
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  if (hcan->Instance == CAN1) {
    get_msg(&m2006);
    update_pid(&pid, pid_target[0] << 4, m2006.speed);
    sent_msg((int16_t)(pid.output));

    // printf("target=%d, speed=%d, current=%d\n\n", pid_target[0] << 4,
    //        m2006.speed, (int16_t)(pid.output));

    uprintf("target=%d, speed=%d, current=%d\r\nkp=%f, ki=%f, kd=%f\n\n",
            pid_target[0] << 4, m2006.speed, (int16_t)(pid.output), pid.kp,
            pid.ki, pid.kd);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  // if (huart->Instance == USART1) {
  //   pid.kp = (float)((pid_target[1] << 16) | (pid_target[2] << 8) |
  //                    (pid_target[3])) /
  //            1000U;
  //   pid.ki = (float)((pid_target[4] << 16) | (pid_target[5] << 8) |
  //                    (pid_target[6])) /
  //            1000U;
  //   pid.kd = (float)((pid_target[7] << 16) | (pid_target[8] << 8) |
  //                    (pid_target[9])) /
  //            1000U;

  //   pid.integral = 0;
  //   pid.lastError = 0;
  //   pid.error = 0;
  //   pid.output = 0;
  // }

  HAL_UART_Receive_IT(huart, pid_target, sizeof(pid_target));
}
