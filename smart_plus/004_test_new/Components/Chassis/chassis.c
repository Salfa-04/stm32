#include "chassis.h"

#include <stdint.h>

#include "hwt101.h"
#include "motor.h"
#include "pid.h"

/// 位置环 PID 参数
#define MOT_KP 0U
#define MOT_KI 0U
#define MOT_KD 0U
#define MOT_MAXI 0U
#define MOT_MAXOUT 0U

/// 角度环 PID 参数
#define MA_KP 0U
#define MA_KI 0U
#define MA_KD 0U
#define MA_MAXI 0U
#define MA_MAXOUT 0U

/// 角速度环 PID 参数
#define MW_KP 0U
#define MW_KI 0U
#define MW_KD 0U
#define MW_MAXI 0U
#define MW_MAXOUT 0U

static pid_t PID_MotX = {0};
static pid_t PID_MotY = {0};
static pid_t PID_MotW = {0};
static pid_t PID_MotA = {0};

static int64_t AddupA = 0;
static int64_t AddupB = 0;
static int64_t AddupC = 0;
static int64_t AddupD = 0;

void chassis_init(void) {
  hwt101_init();
  motor_init();

  /// 初始化 位置环 PID
  pid_init(&PID_MotX, MOT_KP, MOT_KI, MOT_KD, MOT_MAXI, MOT_MAXOUT);
  pid_init(&PID_MotY, MOT_KP, MOT_KI, MOT_KD, MOT_MAXI, MOT_MAXOUT);
  /// 初始化 角度环和角速度环 PID
  pid_init(&PID_MotA, MW_KP, MW_KI, MW_KD, MW_MAXI, MW_MAXOUT);
  pid_init(&PID_MotW, MA_KP, MA_KI, MA_KD, MA_MAXI, MA_MAXOUT);
}

/// 控制相对位置, 单位为 mm
void chassis_control_dest(float x, float y) {
  PID_MotX.target = x, PID_MotY.target = y;
  motor_addup_clear();
}

/// 控制相对角度
void chassis_control_angu(float w) {
  float dest = PID_MotA.target + w;

  while (dest > 180) dest -= 360;
  while (dest < -180) dest += 360;

  PID_MotA.target = dest;
}

/// 电机事件回调函数, 控制速度更新频率
/// 原则上应该与陀螺仪更新速度一致
void motor_event_callback(void) {
  float OutputA = 0, OutputB = 0, OutputC = 0, OutputD = 0;
  motor_addup_get(&AddupA, &AddupB, &AddupC, &AddupD);

  /// 运动解算: cnt -> mm
  /// X = (+ A + B + C + D);                    cnt
  /// Y = (- A + B - C + D);                    cnt
  /// W = (- A - B + C + D);                    cnt
  ///
  /// Line Velocity (线速度): cnt -> mm
  /// N = N * 2π / 65536.f                      rad
  /// N = N * WHEEL_RADIUS;                     mm
  /// SO:
  ///   N = N * π * WHEEL_RADIUS / 131072.f;
  ///
  /// 单位与 `WHEEL_RADIUS` `CHASSIS_RA` `CHASSIS_RB` 的单位相同
  float DestX =
      (AddupA + AddupB + AddupC + AddupD) * __PI * WHEEL_RADIUS / 131072.f;
  float DestY =
      (-AddupA + AddupB - AddupC + AddupD) * __PI * WHEEL_RADIUS / 131072.f;

  pid_update(&PID_MotX, DestX);
  pid_update(&PID_MotY, DestY);

  float SpeedX = PID_MotX.output, SpeedY = PID_MotY.output;

  /// 运动解算: dps -> rpm W: V(dps); T: 60s/min
  ///
  /// Angular Velocity (角速度): dps -> rpm
  /// V = W * 2π * (Ra + Rb) / 360.f            mm/s
  /// V = V * T / (2π * WHEEL_RADIUS)           rpm
  /// SO:
  ///   V = W * (Ra + Rb) / (WHEEL_RADIUS * 6.f);
  ///
  /// 单位与 `WHEEL_RADIUS` `CHASSIS_RA` `CHASSIS_RB` 的单位相同
  float vm = PID_MotW.output * (CHASSIS_RA + CHASSIS_RB) / (WHEEL_RADIUS * 6.f);
  OutputA -= vm, OutputB -= vm, OutputC += vm, OutputD += vm;

  /// 运动解算: mm/s -> rpm X: V(mm/s); Y: V(mm/s); T: 60s/min
  ///
  /// Line Velocity (线速度): mm/s -> rpm
  /// V = X - Y                                 mm/s
  /// V = V * T / (2π * WHEEL_RADIUS)           rpm
  /// SO:
  ///   V = (X - Y) * 30.f / (WHEEL_RADIUS * __PI);
  ///
  /// 单位与 `WHEEL_RADIUS` `CHASSIS_RA` `CHASSIS_RB` 的单位相同
  OutputA += (SpeedX - SpeedY) * 30.f / (WHEEL_RADIUS * __PI);
  OutputB += (SpeedX + SpeedY) * 30.f / (WHEEL_RADIUS * __PI);
  OutputC += (SpeedX - SpeedY) * 30.f / (WHEEL_RADIUS * __PI);
  OutputD += (SpeedX + SpeedY) * 30.f / (WHEEL_RADIUS * __PI);

  /// 并集 PID 输出
  motor_speed_ctrl(OutputA, OutputB, OutputC, OutputD);
}

void hwt101_angular_callback(short angle) {
  // 角速度环 PID 更新 dps
  pid_update(&PID_MotW, angle);
}

void hwt101_angle_callback(short angle) {
  // 角度环 PID 更新 d(°)
  pid_update(&PID_MotA, angle);
  PID_MotW.target = PID_MotA.output;
}
