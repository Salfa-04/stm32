#ifndef __HWT101_H
#define __HWT101_H

#include "stm32f4xx_hal.h"

/// HWT101 initialization
void hwt101_init(void);

/// 中断数据接收回调函数, 请把该函数放到中断服务函数中, 如:
///
/// ```
/// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
///     if (huart->Instance == UART4 /* EDIT */) hwt101_event_callback(Size);
/// }
/// ```
void hwt101_event_callback(uint16_t size);

/// 角速度数据接收回调函数: dps
void hwt101_angular_callback(short angle);

/// 偏航角数据接收回调函数: d(°)
void hwt101_angle_callback(short angle);

#endif /* __HWT101_H */
