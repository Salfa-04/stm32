#include "mctrl_angle.h"

#include "mctrl.h"
#include "mctrl_angular.h"
#include "pid.h"

#define ANGLE_KP 0
#define ANGLE_KI 0
#define ANGLE_KD 0
#define ANGLE_MAXI 700
#define ANGLE_MAXO 980

#define ANGLE_ACCU 1.f  // d(°) * 10 -> ANGLE_ACCU / 10

static pid_t PID_Angle = {0};

void mctrl_angle_init(void) {
  pid_init(&PID_Angle, ANGLE_KP, ANGLE_KI, ANGLE_KD, ANGLE_MAXI, ANGLE_MAXO);
}

/// HWT101放置时水平放置, 芯片侧向上:
/// 角度为相对角度, 以陀螺仪开机上电时为 0°
/// 角度范围为 -180° ~ 180°
/// NOTE: 转动时不能移动!!! 该函数将阻塞运行
void mctrl_chassis_control_angle(float angle) {
  if (mctrl_mutex != 1 && mctrl_mutex != 5) return;

  PID_Angle.target = angle * 28;     // 设置角度 d(°) * 28
  mctrl_chassis_control_angular(0);  // 更新速度环
  mctrl_mutex = 5;                   // 锁定

  // 等待角度环稳定
  while (PID_Angle.output * 10 > ANGLE_ACCU ||
         PID_Angle.output * 10 < -ANGLE_ACCU);

  mctrl_chassis_control_angular(0);  // 更新速度环
  mctrl_mutex = 1;                   // 解锁
}

void hwt101_angle_callback(short angle) {
  if (mctrl_mutex != 5) return;

  // 更新角度环, 参数为当前角度值 d(°) * 28
  pid_update(&PID_Angle, angle * 28);
  mctrl_chassis_control_angular(PID_Angle.output);  // 更新速度环
}
