#ifndef __CHASSIS_H
#define __CHASSIS_H

#define CHASSIS_RA 0.f    // 单位 mm
#define CHASSIS_RB 0.f    // 单位 mm
#define WHEEL_RADIUS 0.f  // 单位 mm
#define __PI 3.14159265358979323846f

void chassis_init(void);

/// 电机事件回调函数, 控制速度更新频率
void motor_event_callback(void);

#endif /* __CHASSIS_H */
