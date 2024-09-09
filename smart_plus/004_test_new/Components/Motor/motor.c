#include "motor.h"

#define ACCEL 0x1A  // 电机加速度
#define MotIdA 1    // Motor A    CW  左前
#define MotIdB 1    // Motor B    CW  左后
#define MotIdC 1    // Motor C   CCW  右后
#define MotIdD 1    // Motor D   CCW  右前

#define MotPort UART_HandleTypeDef
#define MotPortA huart2  // Motor A    CW  左前
#define MotPortB huart5  // Motor B    CW  左后
#define MotPortC huart3  // Motor C   CCW  右后
#define MotPortD huart6  // Motor D   CCW  右前
#define CW 0x00          // 电机正转   CW
#define CCW 0x01         // 电机反转  CCW

extern MotPort MotPortA;
extern MotPort MotPortB;
extern MotPort MotPortC;
extern MotPort MotPortD;

void motor_uart_init(void);
void motor_gpio_init(void);
void motor_dma_init(void);
void motor_delay_short(void);
void motor_delay_long(void);

static uint8_t MotSpeed[4][8] = {
    {[0] = MotIdA, [1] = 0xF6, [5] = ACCEL, [6] = 0x00, [7] = 0x6B},
    {[0] = MotIdB, [1] = 0xF6, [5] = ACCEL, [6] = 0x00, [7] = 0x6B},
    {[0] = MotIdC, [1] = 0xF6, [5] = ACCEL, [6] = 0x00, [7] = 0x6B},
    {[0] = MotIdD, [1] = 0xF6, [5] = ACCEL, [6] = 0x00, [7] = 0x6B},
};

_Bool motor_wait_ok(MotPort *mot, uint8_t id, unsigned timeout) {
  uint8_t rx_buffer[24], offsize = 0;
  uint8_t tx_buffer[6] = {id, 0xF3, 0xAB, 0x01, 0x00, 0x6B};
  HAL_UART_Transmit(mot, tx_buffer, sizeof(tx_buffer), timeout);
  HAL_UART_Receive(mot, rx_buffer, sizeof(rx_buffer), timeout);
  for (; offsize < sizeof(rx_buffer) - 3; offsize++)
    if (rx_buffer[offsize] == id && rx_buffer[offsize + 1] == 0xF3 &&
        rx_buffer[offsize + 2] == 0x02 && rx_buffer[offsize + 3] == 0x6B)
      return 1;

  return 0;
}

/// 电机串口DMA发送
/// uint8_t data[4][len]
inline void motor_uart_dma(uint8_t **data, uint8_t len) {
  /// 等待串口空闲, 防止数据丢失
  while ((&MotPortA)->gState != HAL_UART_STATE_READY);
  while ((&MotPortB)->gState != HAL_UART_STATE_READY);
  while ((&MotPortC)->gState != HAL_UART_STATE_READY);
  while ((&MotPortD)->gState != HAL_UART_STATE_READY);

  HAL_UART_Transmit_DMA(&MotPortA, data[0], len);
  HAL_UART_Transmit_DMA(&MotPortB, data[1], len);
  HAL_UART_Transmit_DMA(&MotPortC, data[2], len);
  HAL_UART_Transmit_DMA(&MotPortD, data[3], len);

  /// 等待发送完成和下位机处理完成
  while ((&MotPortA)->gState != HAL_UART_STATE_READY);
  while ((&MotPortB)->gState != HAL_UART_STATE_READY);
  while ((&MotPortC)->gState != HAL_UART_STATE_READY);
  while ((&MotPortD)->gState != HAL_UART_STATE_READY);
}

void motor_init(void) {
  motor_gpio_init();
  motor_dma_init();
  motor_uart_init();

  motor_delay_long();

  // 等待上线
  while (!motor_wait_ok(&MotPortA, MotIdA, 10)) motor_delay_long();
  while (!motor_wait_ok(&MotPortB, MotIdB, 10)) motor_delay_long();
  while (!motor_wait_ok(&MotPortC, MotIdC, 10)) motor_delay_long();
  while (!motor_wait_ok(&MotPortD, MotIdD, 10)) motor_delay_long();
}

/// 电机速度控制, 单位 rpm, 0 ~ ±5000
void motor_speed_ctrl(int16_t a, int16_t b, int16_t c, int16_t d) {
  MotSpeed[0][2] = CW, MotSpeed[0][3] = a >> 8, MotSpeed[0][4] = a & 0xFF;
  MotSpeed[1][2] = CW, MotSpeed[1][3] = b >> 8, MotSpeed[1][4] = b & 0xFF;
  MotSpeed[2][2] = CCW, MotSpeed[2][3] = c >> 8, MotSpeed[2][4] = c & 0xFF;
  MotSpeed[3][2] = CCW, MotSpeed[3][3] = d >> 8, MotSpeed[3][4] = d & 0xFF;
  motor_uart_dma((uint8_t **)MotSpeed, sizeof(MotSpeed[0]));
}

/// bit :::
/// 0: 0:待用; 1: 进入函数;
/// 4: A 完成; 5: B 完成;
/// 6: C 完成; 7: D 完成
uint8_t MotAupMutex = 0x00;
static uint8_t MotAupA[8], MotAupB[8], MotAupC[8], MotAupD[8];

static const uint8_t MotCnt[4][3] = {
    {MotIdA, 0x36, 0x6B},
    {MotIdB, 0x36, 0x6B},
    {MotIdC, 0x36, 0x6B},
    {MotIdD, 0x36, 0x6B},
};

static const uint8_t MotCntClr[4][4] = {
    {MotIdA, 0x0A, 0x6D, 0x6B},
    {MotIdB, 0x0A, 0x6D, 0x6B},
    {MotIdC, 0x0A, 0x6D, 0x6B},
    {MotIdD, 0x0A, 0x6D, 0x6B},
};

/// 电机脉冲数获取, 一圈 65536 cnt
void motor_addup_get(int64_t *a, int64_t *b, int64_t *c, int64_t *d) {
  MotAupMutex = 0x01;
  motor_uart_dma((uint8_t **)MotCnt, sizeof(MotCnt[0]));

  // 等待数据接收完成
  while (MotAupMutex);

  if (MotAupA[0] == MotIdA && MotAupA[1] == 0x33 && MotAupA[7] == 0x6B)
    *a = MotAupA[3] << 24 | MotAupA[4] << 16 | MotAupA[5] << 8 | MotAupA[6];
  if (MotAupB[0] == MotIdB && MotAupB[1] == 0x33 && MotAupB[7] == 0x6B)
    *b = MotAupB[3] << 24 | MotAupB[4] << 16 | MotAupB[5] << 8 | MotAupB[6];
  if (MotAupC[0] == MotIdC && MotAupC[1] == 0x33 && MotAupC[7] == 0x6B)
    *c = MotAupC[3] << 24 | MotAupC[4] << 16 | MotAupC[5] << 8 | MotAupC[6];
  if (MotAupD[0] == MotIdD && MotAupD[1] == 0x33 && MotAupD[7] == 0x6B)
    *d = MotAupD[3] << 24 | MotAupD[4] << 16 | MotAupD[5] << 8 | MotAupD[6];

  if (MotAupA[2]) *a = -*a;
  if (MotAupB[2]) *b = -*b;
  if (MotAupC[2]) *c = -*c;
  if (MotAupD[2]) *d = -*d;
}

void motor_addup_clear(void) {
  motor_uart_dma((uint8_t **)MotCntClr, sizeof(MotCntClr[0]));
}

/// 中断数据接收回调函数, 请把该函数放到中断服务函数中, 如:
///
/// ```
/// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
///     if (MotAupMutex) motor_addup_tx_callback(huart);
/// }
/// ```
void motor_addup_tx_callback(MotPort *mot) {
  if (MotAupMutex == 0x01) {
    if (mot->Instance == MotPortA.Instance)
      HAL_UART_Receive_DMA(&MotPortA, MotAupA, sizeof(MotAupA));
    if (mot->Instance == MotPortB.Instance)
      HAL_UART_Receive_DMA(&MotPortB, MotAupB, sizeof(MotAupB));
    if (mot->Instance == MotPortC.Instance)
      HAL_UART_Receive_DMA(&MotPortC, MotAupC, sizeof(MotAupC));
    if (mot->Instance == MotPortD.Instance)
      HAL_UART_Receive_DMA(&MotPortD, MotAupD, sizeof(MotAupD));
  }
}

/// 中断数据接收回调函数, 请把该函数放到中断服务函数中, 如:
///
/// ```
/// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
///     if (MotAupMutex) motor_addup_rx_callback(huart);
/// }
/// ```
void motor_addup_rx_callback(MotPort *mot) {
  if (mot->Instance == MotPortA.Instance) MotAupMutex |= 0x10;
  if (mot->Instance == MotPortB.Instance) MotAupMutex |= 0x20;
  if (mot->Instance == MotPortC.Instance) MotAupMutex |= 0x40;
  if (mot->Instance == MotPortD.Instance) MotAupMutex |= 0x80;
  if ((MotAupMutex & 0xF0) == 0xF0) MotAupMutex = 0;
}

__weak void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (MotAupMutex) motor_addup_tx_callback(huart);
}

__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (MotAupMutex) motor_addup_rx_callback(huart);
}
