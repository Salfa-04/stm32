#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

#include "stm32f4xx_hal.h"

extern uint8_t MotAupMutex;

void motor_init(void);
void motor_speed_ctrl(int16_t a, int16_t b, int16_t c, int16_t d);
void motor_addup_get(int64_t *a, int64_t *b, int64_t *c, int64_t *d);
void motor_addup_clear(void);

/// 中断数据接收回调函数, 请把该函数放到中断服务函数中, 如:
///
/// ```
/// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
///     if (MotAupMutex) motor_addup_rx_callback(huart);
/// }
/// ```
void motor_addup_rx_callback(UART_HandleTypeDef *mot);

/// 中断数据接收回调函数, 请把该函数放到中断服务函数中, 如:
///
/// ```
/// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
///     if (MotAupMutex) motor_addup_tx_callback(huart);
/// }
/// ```
void motor_addup_tx_callback(UART_HandleTypeDef *mot);

#endif /* __MOTOR_H */
