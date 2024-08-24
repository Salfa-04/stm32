#include "tb6612.h"

#include "tb6612_def.h"

void tb6612_init(void) {
  tb6612_gpio_init();
  tb6612_tim9_init();
  tb6612_tim12_init();
  tb6612_motor_stop();

  TB6612_MOTORA_PWM_SET(0);
  TB6612_MOTORB_PWM_SET(0);
  TB6612_MOTORC_PWM_SET(0);
  TB6612_MOTORD_PWM_SET(0);
}

/// 全部电机滑行
void tb6612_motor_stop(void) {
  MotorA_stop();
  MotorB_stop();
  MotorC_stop();
  MotorD_stop();
}

/// 全部电机刹车
void tb6612_motor_brack(void) {
  MotorA_brack();
  MotorB_brack();
  MotorC_brack();
  MotorD_brack();
}

/// 设置电机pwm值
///
/// 参数为正, 电机正转; 参数为负, 电机反转; 参数为零, 电机滑行
void tb6612_motor_pwm(int16_t a, int16_t b, int16_t c, int16_t d) {
  tb6612_motor_direct(&a, &b, &c, &d);
  TB6612_MOTORA_PWM_SET(a);
  TB6612_MOTORB_PWM_SET(b);
  TB6612_MOTORC_PWM_SET(c);
  TB6612_MOTORD_PWM_SET(d);
}
