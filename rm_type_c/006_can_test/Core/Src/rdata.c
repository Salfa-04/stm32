#include "rdata.h"
#include "can.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"
#include <stdint.h>

extern uint8_t can_buff[8];
uint8_t display_buffer[64];

typedef struct {
  uint8_t angle_high;
  uint8_t angle_low;
  uint8_t speed_high;
  uint8_t speed_low;
  uint8_t torque_high;
  uint8_t torque_low;

  uint8_t __rev1;
  uint8_t __rev2;
} motor_msg_t;

void display(message msg) {
  uint16_t len = sprintf((char *)display_buffer,
                         "CAN: angle: %d, speed: %d, torque: %d\n\n\n",
                         msg.angle, msg.speed, msg.torque);

  HAL_UART_Transmit(&huart1, display_buffer, len, 1000);
}

message motor_receive() {
  uint8_t raw_data[8];

  CAN_RxHeaderTypeDef RxHeader;
  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, raw_data);

  motor_msg_t *temp = (motor_msg_t *)raw_data;
  message msg = {(temp->angle_high << 8) | temp->angle_low,
                 (temp->speed_high << 8) | temp->speed_low,
                 (temp->torque_high << 8) | temp->torque_low};

  return msg;
}

void can_send_motor(uint16_t current) {
  uint8_t data[8] = {
      [0] = current >> 8,
      [1] = current & 0xFF,
  };

  uint32_t txMail = CAN_TX_MAILBOX0;
  CAN_TxHeaderTypeDef TxHeader;

  TxHeader.DLC = 8;
  TxHeader.StdId = 0x1FF;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.RTR = CAN_RTR_DATA;

  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &txMail);
}

// uint8_t uart_buffer[2];
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//   if (huart->Instance == USART1) {
//     can_send_motor(uart_buffer[0] << 8 | uart_buffer[1]);
//   }
// }

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  //   CAN_RxHeaderTypeDef RxHeader;
  //   HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, can_buff);

  //   display();
}

void can_filter_init() {
  CAN_FilterTypeDef sFilterConfig;

  sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  sFilterConfig.FilterBank = 0;

  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;

  HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  HAL_CAN_Start(&hcan1);
}
