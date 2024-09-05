#ifndef __TB6612_DEF_H
#define __TB6612_DEF_H

void tb6612_gpio_init(void);
void tb6612_tim9_init(void);
void tb6612_tim12_init(void);

void MotorA_stop(void);       /// 电机A滑行
void MotorB_stop(void);       /// 电机B滑行
void MotorC_stop(void);       /// 电机C滑行
void MotorD_stop(void);       /// 电机D滑行
void MotorA_brack(void);      /// 电机A刹车
void MotorB_brack(void);      /// 电机B刹车
void MotorC_brack(void);      /// 电机C刹车
void MotorD_brack(void);      /// 电机D刹车
void MotorA_forwards(void);   /// 电机A前进
void MotorB_forwards(void);   /// 电机B前进
void MotorC_forwards(void);   /// 电机C前进
void MotorD_forwards(void);   /// 电机D前进
void MotorA_backwards(void);  /// 电机A后退
void MotorB_backwards(void);  /// 电机B后退
void MotorC_backwards(void);  /// 电机C后退
void MotorD_backwards(void);  /// 电机D后退

#define MOTOR_STATEX_BUILD(n, m, set, reset)                \
  void Motor##n##_##m##wards(void) {                        \
    HAL_GPIO_WritePin(GPIOE, n##IN##reset, GPIO_PIN_RESET); \
    HAL_GPIO_WritePin(GPIOE, n##IN##set, GPIO_PIN_SET);     \
  }

#define MOTOR_STATEY_BUILD(n, m, state)                          \
  void Motor##n##_##m(void) {                                    \
    HAL_GPIO_WritePin(GPIOE, n##IN1 | n##IN2, GPIO_PIN_##state); \
  }

#define MOTOR_DIRECT_BUILD(x, n) \
  if (*x > 0)                    \
    Motor##n##_forwards();       \
  else if (*x == 0)              \
    Motor##n##_stop();           \
  else {                         \
    Motor##n##_backwards();      \
    *x = -*x;                    \
  }

/// n 为定时器编号; x 为预分频系数
#define TIM_SETUP_BUILD(n, x)                                        \
  void tb6612_tim##n##_init(void) {                                  \
    __HAL_RCC_TIM##n##_CLK_ENABLE();                                 \
    TIM_OC_InitTypeDef sConfigOC = {0};                              \
                                                                     \
    htim##n.Instance = TIM##n;                                       \
    htim##n.Init.Prescaler = x;                                      \
    htim##n.Init.CounterMode = TIM_COUNTERMODE_UP;                   \
    htim##n.Init.Period = 5599;                                      \
    htim##n.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;             \
    htim##n.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; \
    HAL_TIM_PWM_Init(&htim##n);                                      \
                                                                     \
    sConfigOC.OCMode = TIM_OCMODE_PWM1;                              \
    sConfigOC.Pulse = 0;                                             \
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;                      \
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;                       \
    HAL_TIM_PWM_ConfigChannel(&htim##n, &sConfigOC, TIM_CHANNEL_1);  \
    HAL_TIM_PWM_ConfigChannel(&htim##n, &sConfigOC, TIM_CHANNEL_2);  \
                                                                     \
    HAL_TIM_PWM_Start(&htim##n, TIM_CHANNEL_1);                      \
    HAL_TIM_PWM_Start(&htim##n, TIM_CHANNEL_2);                      \
  }

#endif /* __TB6612_DEF_H */
