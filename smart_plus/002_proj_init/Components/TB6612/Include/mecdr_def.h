#ifndef __MECDR_DEF_H
#define __MECDR_DEF_H

void mecdr_gpio_init(void);
void mecdr_tim2_init(void);
void mecdr_tim3_init(void);
void mecdr_tim4_init(void);
void mecdr_tim5_init(void);
void mecdr_tim6_init(void);

#define MECDR_MOTORA_CNT (TIM5->CNT)
#define MECDR_MOTORB_CNT (TIM3->CNT)
#define MECDR_MOTORC_CNT (TIM2->CNT)
#define MECDR_MOTORD_CNT (TIM4->CNT)

/// FREQ: 1MHz ~ 0.001ms (ro)
/// CNT: 0 ~ 9999; 10ms interrup
#define MECDR_TIMER_CNT (unsigned short)(TIM6->CNT)

/// 生成TIM中断服务函数, x为中断服务函数名, n为TIM的编号
#define TIM_IRQ_BUILDER(x, n) \
  void x##_IRQHandler(void) { HAL_TIM_IRQHandler(&htim##n); }

/// n为要初始化的TIM的编号, x为捕获极性, p为中断优先级
#define TIM_SETUP_BUILD(n, x, p)                                     \
  void mecdr_tim##n##_init(void) {                                   \
    TIM_Encoder_InitTypeDef sConfig = {0};                           \
                                                                     \
    htim##n.Instance = TIM##n;                                       \
    htim##n.Init.Prescaler = 0;                                      \
    htim##n.Init.CounterMode = TIM_COUNTERMODE_UP;                   \
    htim##n.Init.Period = 65535;                                     \
    htim##n.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;             \
    htim##n.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; \
                                                                     \
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;                      \
    sConfig.IC1Polarity = TIM_ICPOLARITY_##x;                        \
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;                 \
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;                           \
    sConfig.IC1Filter = 0;                                           \
    sConfig.IC2Polarity = TIM_ICPOLARITY_##x;                        \
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;                 \
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;                           \
    sConfig.IC2Filter = 0;                                           \
                                                                     \
    HAL_TIM_Encoder_Init(&htim##n, &sConfig);                        \
                                                                     \
    HAL_NVIC_SetPriority(TIM##n##_IRQn, p, 0);                       \
    HAL_NVIC_EnableIRQ(TIM##n##_IRQn);                               \
                                                                     \
    HAL_TIM_Encoder_Start_IT(&htim##n, TIM_CHANNEL_ALL);             \
  }

#define IC_CAPT_BUILDER(n, x)                                      \
  if (htim->Instance == TIM##n) {                                  \
    if (!Motor##x##_InitTick) /* 未计数, 初始化*/            \
      Motor##x##_InitTick = MECDR_TIMER_CNT;                       \
    else { /* 计数完成 1tick */                                \
      Motor##x##_TickAll += MECDR_TIMER_CNT - Motor##x##_InitTick; \
      Motor##x##_InitTick = 0; /* 计数完成, 复位 */          \
    }                                                              \
  }

#endif /* __MECDR_DEF_H */
