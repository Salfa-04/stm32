#include "mctrl.h"

#include "hwt101.h"
#include "mctrl_angle.h"
#include "mctrl_angular.h"
#include "mctrl_distance.h"
#include "mctrl_velocity.h"
#include "mecdr.h"
#include "tb6612.h"

/// 电机控制互斥锁:
///
/// 0: 未初始化
/// 1: 初始化完成
/// 3: 正在控制位置或速度
/// 5: 正在控制姿态或角度
uint8_t mctrl_mutex = 0;

void mctrl_init(void) {
  // 电机编码器初始化
  mecdr_init();
  // 电机驱动初始化
  tb6612_init();
  // 陀螺仪初始化
  hwt101_init();

  // 初始化速度环pid
  mctrl_velocity_init();
  // 初始化角速度环pid
  mctrl_angular_init();
  // 初始化角度环pid
  mctrl_angle_init();
  // 初始化位置环pid
  mctrl_distance_init();

  // 初始化完成
  mctrl_mutex = 1;
}

/// !!!__weak. pls
__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM6) {
    mctrl_distance_callback();  // 位置环pid
    mctrl_velocity_callback();  // 速度环pid
  } /** FREQ: 100Hz (10k-1) */
}
