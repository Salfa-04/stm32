#include "mctrl_distance.h"

#include "mctrl.h"
#include "mctrl_velocity.h"
#include "mecdr.h"
#include "pid.h"

#define DISTANCE_KP 0
#define DISTANCE_KI 0
#define DISTANCE_KD 0
#define DISTANCE_MAXI 700
#define DISTANCE_MAXO 980

#define DISTANCE_ACCU 1.f  // mm/10 -> ANGLE_ACCU/10

static pid_t DistanceX = {0};
static pid_t DistanceY = {0};

static float DISTANCE_A = 0;
static float DISTANCE_B = 0;
static float DISTANCE_C = 0;
static float DISTANCE_D = 0;

void mctrl_distance_init(void) {
  pid_init(&DistanceX, DISTANCE_KP, DISTANCE_KI, DISTANCE_KD, DISTANCE_MAXI,
           DISTANCE_MAXO);
  pid_init(&DistanceY, DISTANCE_KP, DISTANCE_KI, DISTANCE_KD, DISTANCE_MAXI,
           DISTANCE_MAXO);
}

/// 右手坐标系, x指向前, y指向左
/// 控制底盘行走距离; x, y: mm
void mctrl_chassis_control_distance(float x, float y) {
  if (mctrl_mutex != 1 && mctrl_mutex != 3) return;
  mecdr_addup_clear();  // 清除电机计数值

  // 设置目标值
  DistanceX.target = x;
  DistanceY.target = y;

  mctrl_chassis_control_velocity(0, 0);
  mctrl_mutex = 3;  // 锁定

  // 等待电机完成运动
  while (DistanceX.output * 10 > DISTANCE_ACCU ||
         DistanceX.output * 10 < -DISTANCE_ACCU ||
         DistanceY.output * 10 > DISTANCE_ACCU ||
         DistanceY.output * 10 < -DISTANCE_ACCU);

  mctrl_chassis_control_velocity(0, 0);
  mctrl_mutex = 1;  // 解锁
}

void mctrl_distance_callback(void) {
  if (mctrl_mutex != 3) return;  // 未锁定

  // 获取电机累计计数值, 单位: rad
  mecdr_addup_get(&DISTANCE_A, &DISTANCE_B, &DISTANCE_C, &DISTANCE_D);

  /// 运动解算: rad -> mm
  /// X = (+ A + B + C + D) / 4;                    rad
  /// Y = (- A + B - C + D) / 4;                    rad
  /// W = (- A + B + C - D) / 4;                    rad
  ///
  /// Line Velocity (线速度): rad -> mm
  /// N = N * WHEEL_RADIUS;
  float distance_x =
      (DISTANCE_A + DISTANCE_B + DISTANCE_C + DISTANCE_D) * WHEEL_RADIUS / 4.f;
  float distance_y =
      (-DISTANCE_A + DISTANCE_B - DISTANCE_C + DISTANCE_D) * WHEEL_RADIUS / 4.f;

  // 更新位置环pid
  pid_update(&DistanceX, distance_x);
  pid_update(&DistanceY, distance_y);

  // 更新速度环pid
  mctrl_chassis_control_velocity(DistanceX.output, DistanceY.output);
}
