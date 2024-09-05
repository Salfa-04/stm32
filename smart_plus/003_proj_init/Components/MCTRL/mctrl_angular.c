#include "mctrl_angular.h"

#include "hwt101.h"
#include "mctrl.h"
#include "pid.h"
#include "tb6612.h"

#define ANGULAR_KP 0
#define ANGULAR_KI 0
#define ANGULAR_KD 0
#define ANGULAR_MAXI 4000
#define ANGULAR_MAXO 5600

static pid_t PID_AngularA = {0};
static pid_t PID_AngularB = {0};
static pid_t PID_AngularC = {0};
static pid_t PID_AngularD = {0};

void mctrl_angular_init(void) {
  pid_init(&PID_AngularA, ANGULAR_KP, ANGULAR_KI, ANGULAR_KD, ANGULAR_MAXI,
           ANGULAR_MAXO);
  pid_init(&PID_AngularB, ANGULAR_KP, ANGULAR_KI, ANGULAR_KD, ANGULAR_MAXI,
           ANGULAR_MAXO);
  pid_init(&PID_AngularC, ANGULAR_KP, ANGULAR_KI, ANGULAR_KD, ANGULAR_MAXI,
           ANGULAR_MAXO);
  pid_init(&PID_AngularD, ANGULAR_KP, ANGULAR_KI, ANGULAR_KD, ANGULAR_MAXI,
           ANGULAR_MAXO);
}

/// 右手坐标系, x指向前, y指向左, w为旋转
/// 速度为底盘速度, 单位 angle: dps
/// 函数使用之后需要手动锁定电机: `mctrl_mutex = 5`
void mctrl_chassis_control_angular(float angle) {
  /// 运动解算: * -> cpm (rpm * 1320)
  ///
  /// X: V(mm/s); Y: V(mm/s); W: V(dps); T: 60s/min
  ///
  /// Angular Velocity (角速度): dps -> cpm (rpm * 1320)
  /// V = W * 2π * (Ra + Rb) / 360.f            mm/s
  /// V = V * T / (2π * WHEEL_RADIUS)           rpm
  /// V = V * (11 * 30 * 4)                     cpm
  /// SO:
  ///   V = W * (Ra + Rb) * 220 / WHEEL_RADIUS;
  ///
  /// 单位与 `WHEEL_RADIUS` `CHASSIS_RA` `CHASSIS_RB` 的单位相同
  float vm = angle * (CHASSIS_RA + CHASSIS_RB) * 220.f / WHEEL_RADIUS;

  PID_AngularA.target = -vm;
  PID_AngularB.target = -vm;
  PID_AngularC.target = +vm;
  PID_AngularD.target = +vm;
}

void hwt101_angular_callback(short angle) {
  if (mctrl_mutex != 5) return;  // 未锁定

  float vm = angle * (CHASSIS_RA + CHASSIS_RB) * 220.f / WHEEL_RADIUS;
  pid_update(&PID_AngularA, -vm);
  pid_update(&PID_AngularB, -vm);
  pid_update(&PID_AngularC, vm);
  pid_update(&PID_AngularD, vm);

  // 输出PWM值: 0 ~ 5600
  tb6612_motor_pwm(PID_AngularA.output, PID_AngularB.output,
                   PID_AngularC.output, PID_AngularD.output);
}
