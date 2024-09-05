#include "mctrl_velocity.h"

#include "mctrl.h"
#include "mecdr.h"
#include "pid.h"
#include "tb6612.h"

#define SPEED_KP 0
#define SPEED_KI 0
#define SPEED_KD 0
#define SPEED_MAXI 4000
#define SPEED_MAXO 5600

static pid_t PID_MotorA = {0};
static pid_t PID_MotorB = {0};
static pid_t PID_MotorC = {0};
static pid_t PID_MotorD = {0};

/// 单位 cpm (rpm * 1320)
static float MotorA_Speed = 0;
static float MotorB_Speed = 0;
static float MotorC_Speed = 0;
static float MotorD_Speed = 0;

// pid速度环 init
void mctrl_velocity_init(void) {
  pid_init(&PID_MotorA, SPEED_KP, SPEED_KI, SPEED_KD, SPEED_MAXI, SPEED_MAXO);
  pid_init(&PID_MotorB, SPEED_KP, SPEED_KI, SPEED_KD, SPEED_MAXI, SPEED_MAXO);
  pid_init(&PID_MotorC, SPEED_KP, SPEED_KI, SPEED_KD, SPEED_MAXI, SPEED_MAXO);
  pid_init(&PID_MotorD, SPEED_KP, SPEED_KI, SPEED_KD, SPEED_MAXI, SPEED_MAXO);
}

/// 右手坐标系, x指向前, y指向左, w为旋转
/// 速度为底盘速度, 单位 x,y: mm/s, w: dps
/// 函数使用之后需要手动锁定电机: `mctrl_mutex = 3`
void mctrl_chassis_control_velocity(float x, float y) {
  /// 运动解算: * -> cpm (rpm * 1320)
  ///
  /// X: V(mm/s); Y: V(mm/s); W: V(dps); T: 60s/min
  ///
  /// Line Velocity (线速度): mm/s -> cpm (rpm * 1320)
  /// V = X - Y                                 mm/s
  /// V = V * T / (2π * WHEEL_RADIUS)           rpm
  /// V = V * (11 * 30 * 4)                     cpm
  /// SO:
  ///   V = (X - Y) * 39600 / (WHEEL_RADIUS * __PI);
  ///
  /// 单位与 `WHEEL_RADIUS` `CHASSIS_RA` `CHASSIS_RB` 的单位相同

  // 速度环控制, 写入目标值 cpm (rpm * 1320)
  PID_MotorA.target = (x - y) * 39600.f / (WHEEL_RADIUS * __PI);
  PID_MotorB.target = (x + y) * 39600.f / (WHEEL_RADIUS * __PI);
  PID_MotorC.target = (x - y) * 39600.f / (WHEEL_RADIUS * __PI);
  PID_MotorD.target = (x + y) * 39600.f / (WHEEL_RADIUS * __PI);
}

/// Called by TIM6 Interrupts; 100Hz
void mctrl_velocity_callback(void) {
  if (mctrl_mutex != 3) return;  // 未锁定

  // 获取速度值, 单位 cpm (rpm * 1320)
  mecdr_speed_calc(&MotorA_Speed, &MotorB_Speed, &MotorC_Speed, &MotorD_Speed);
  // 更新速度环, 目标值由 speed_set 设置
  pid_update(&PID_MotorA, MotorA_Speed);
  pid_update(&PID_MotorB, MotorB_Speed);
  pid_update(&PID_MotorC, MotorC_Speed);
  pid_update(&PID_MotorD, MotorD_Speed);

  // 输出PWM值: 0 ~ 5600
  tb6612_motor_pwm(PID_MotorA.output, PID_MotorB.output, PID_MotorC.output,
                   PID_MotorD.output);
}
