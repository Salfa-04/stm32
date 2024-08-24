#include "mctrl_angle.h"

#include "mctrl.h"
#include "mctrl_velocity.h"
#include "pid.h"

#define ANGLE_KP 0
#define ANGLE_KI 0
#define ANGLE_KD 0
#define ANGLE_MAXI 700
#define ANGLE_MAXO 980

#define ANGLE_ACCU 1.f  // d(°)/10 -> ANGLE_ACCU/10

static pid_t Angle = {0};
static float yaw = 0;

void mctrl_angle_init(void) {
  pid_init(&Angle, ANGLE_KP, ANGLE_KI, ANGLE_KD, ANGLE_MAXI, ANGLE_MAXO);
}

/// IMU901放置时按照东北天坐标系:
/// 丝印侧向下, x指向正东, y指向正北, z指向天空
/// 角度为绝对角度, 以手机指南针正南为 0°
/// 角度范围为 -180° ~ 180°
/// NOTE: 转动时不能移动!!! 该函数将阻塞运行
void mctrl_chassis_control_angle(float angle) {
  Angle.target = angle * 28;  // 设置角度 d(°) * 28

  if (mctrl_mutex != 1 && mctrl_mutex != 7) return;
  mctrl_mutex = 7;  // 锁定

  // 等待角度环稳定
  while (Angle.output * 10 > ANGLE_ACCU || Angle.output * 10 < -ANGLE_ACCU);

  mctrl_mutex = 1;  // 解锁
}

void imu901_data_callback(uint8_t id, uint8_t *data, uint8_t len) {
  if (id == 0x01 && len == 6 && mctrl_mutex == 7) {  // Yaw 轴姿态角 (°)
    yaw = (float)((int16_t)(data[5] << 8) | data[4]) / 32768.f * 180.f;
    // 更新角度环, 参数为当前角度值 d(°) * 28
    pid_update(&Angle, yaw * 28);
    mctrl_chassis_control_velocity(0, 0, Angle.output);  // 更新速度环
  }
}
