#include "can.h"
#include "main.h"
#include "source.h"
#include "usart.h"

void get_msg(motor_t *msg) {
  if (!HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0)) {
    memset(msg, 0, sizeof(motor_t));
    return;
  }

  uint8_t data[8];
  CAN_RxHeaderTypeDef RxHeader;
  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, data);

  msg->id = RxHeader.StdId;
  msg->angle = (int16_t)(data[0] << 8 | data[1]);
  msg->speed = (int16_t)(data[2] << 8 | data[3]);
  msg->torque = (int16_t)(data[4] << 8 | data[5]);
}

void sent_msg(uint16_t current) {
  uint32_t mail_box = CAN_TX_MAILBOX0;
  CAN_TxHeaderTypeDef TxHeader;

  TxHeader.DLC = 8;
  // TxHeader.StdId = 0x200;
  TxHeader.StdId = 0x1FF;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.RTR = CAN_RTR_DATA;

  uint8_t data[8] = {
      [0] = current >> 8,
      [1] = current & 0xff,
  };

  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &mail_box);
}

void display(motor_t *msg) {
  uprintf("ID=%03x, Angle=%d, Speed=%d, Torque=%d\r\n\r\n", //
          msg->id, msg->angle, msg->speed, msg->torque);
}
