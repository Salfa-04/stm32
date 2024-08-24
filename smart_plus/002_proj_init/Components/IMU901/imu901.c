#include "imu901.h"

#include "imu901_reg.h"

void imu901_uart_init(void);
void imu901_gpio_init(void);
void imu901_dma_init(void);
void imu901_delay_long(void);
void imu901_delay_short(void);

extern UART_HandleTypeDef huart4;

static uint8_t buffer[18];
static const uint8_t imu901_init_cmd[][2] = {
    {D0MODE, 0x00U},      // 关闭端口
    {D1MODE, 0x00U},      // 关闭端口
    {D2MODE, 0x00U},      // 关闭端口
    {D3MODE, 0x00U},      // 关闭端口
    {LEDOFF, 0x00U},      // 打开LED
    {BAUCAL, 0x00U},      // 不校准气压计
    {GAUCAL, 0x01U},      // 校准陀螺仪，静止2s
    {ASM, 0x00U},         // 水平安装
    {ALG, 0x01U},         // 九轴融合
    {RETURNRATE, 0x03U},  // 回报100Hz
    {RETURNSET, 0x01U},   // 回传内容
    {BAUD, 0x04U},        // 波特率115200
    {ACCBW, 0x02U},       // 加速度45Hz
    {GYROBW, 0x01U},      // 陀螺仪2000Hz
    {ACCFSR, 0x00U},      // 0:2G; 1:4G
    {GYROFSR, 0x02U},     // 2:1000dps; 3:2000dps

    // {SAVE, 0x00U},        // 保存至Flash
};

void imu901_init(void) {
  imu901_uart_init();
  imu901_gpio_init();
  imu901_dma_init();

  imu901_delay_short();
  for (uint8_t i = 0; i < sizeof(imu901_init_cmd) / 2; i++) {
    imu901_reg_write(imu901_init_cmd[i][0], imu901_init_cmd[i][1]);
    imu901_delay_short();
  }

  uint8_t status = {0};
  imu901_delay_long();  // H: 55 AF 02 01 04 0B
  imu901_reg_read(SENSTA, &status, 1000);
  while (!(status & 4)) {
    imu901_delay_short();
    imu901_reg_read(SENSTA, &status, 100);
  }

  HAL_UARTEx_ReceiveToIdle_DMA(&huart4 /* EDIT */, buffer, sizeof(buffer));
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == UART4 /* EDIT */) {
    uint8_t check_sum = 0xAA + buffer[2] + buffer[3], len = buffer[3];
    if (buffer[0] == 0x55 && buffer[1] == 0x55) {
      while (len--) check_sum += buffer[len + 4];
      if (check_sum == buffer[Size - 1])
        imu901_data_callback(buffer[2], &buffer[4], buffer[3]);
    }

    HAL_UARTEx_ReceiveToIdle_DMA(&huart4 /* EDIT */, buffer, sizeof(buffer));
  }
}

/// 当接收到数据时会自动调用该函数
__weak void imu901_data_callback(uint8_t id, uint8_t *data, uint8_t len) {
  (void)len;
  angle_t *angle = NULL;

  switch (id) {
    case 0x01: {  // 姿态角 (°)
      angle->roll = (float)((int16_t)(data[1] << 8) | data[0]) / 32768 * 180;
      angle->pitch = (float)((int16_t)(data[3] << 8) | data[2]) / 32768 * 180;
      angle->yaw = (float)((int16_t)(data[5] << 8) | data[4]) / 32768 * 180;
      break;
    }
  }
}
