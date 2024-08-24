#include "mecdr.h"

#include "mecdr_def.h"

#define __PI 3.14159265358979323846f

/// 单位: cnt
static float ALL_COUNT_A = 0;
static float ALL_COUNT_B = 0;
static float ALL_COUNT_C = 0;
static float ALL_COUNT_D = 0;

static uint16_t MotorA_InitTick = 0;
static uint16_t MotorB_InitTick = 0;
static uint16_t MotorC_InitTick = 0;
static uint16_t MotorD_InitTick = 0;
static uint16_t MotorA_TickAll = 1;
static uint16_t MotorB_TickAll = 1;
static uint16_t MotorC_TickAll = 1;
static uint16_t MotorD_TickAll = 1;

#define MOTOR_CNT_CLEAR() \
  {                       \
    MECDR_MOTORA_CNT = 0; \
    MECDR_MOTORB_CNT = 0; \
    MECDR_MOTORC_CNT = 0; \
    MECDR_MOTORD_CNT = 0; \
    MotorA_InitTick = 0;  \
    MotorB_InitTick = 0;  \
    MotorC_InitTick = 0;  \
    MotorD_InitTick = 0;  \
    MotorA_TickAll = 1;   \
    MotorB_TickAll = 1;   \
    MotorC_TickAll = 1;   \
    MotorD_TickAll = 1;   \
  }

void mecdr_init(void) {
  mecdr_gpio_init();
  mecdr_tim2_init();
  mecdr_tim3_init();
  mecdr_tim4_init();
  mecdr_tim5_init();
  mecdr_tim6_init();

  MOTOR_CNT_CLEAR();
}

/// Called 100Hz by TIM6 interrupt; return cpm
/// !rpm = cpm / (11 * 30 * 4) = cpm / 1320
void mecdr_speed_calc(float *a, float *b, float *c, float *d) {
  float va = (short)MECDR_MOTORA_CNT, ta = MotorA_TickAll, ma;
  float vb = (short)MECDR_MOTORB_CNT, tb = MotorB_TickAll, mb;
  float vc = (short)MECDR_MOTORC_CNT, tc = MotorC_TickAll, mc;
  float vd = (short)MECDR_MOTORD_CNT, td = MotorD_TickAll, md;
  MOTOR_CNT_CLEAR();

  // M0: (short)MECDR_MOTORA_CNT
  // M1: MotorA_TickAll
  // FR: 10000 Hz
  // CNT: FR * M0 / M1
  ALL_COUNT_A += ma = 10000.f * va / ta;
  ALL_COUNT_B += mb = 10000.f * vb / tb;
  ALL_COUNT_C += mc = 10000.f * vc / tc;
  ALL_COUNT_D += md = 10000.f * vd / td;

  // FREQ: 100 Hz
  // C: (60 * FREQ) = 6000/min
  // Speed(V) = CNT * C (cpm)
  *a = 6000.f * ma;
  *b = 6000.f * mb;
  *c = 6000.f * mc;
  *d = 6000.f * md;
}

/// 获取累积计数值, 单位: rad
/// S = w * R; 计算可得路程, 单位与R相同
void mecdr_addup_get(float *a, float *b, float *c, float *d) {
  /// w = 2π * cnt / (11 * 30 * 4);     rad

  *a = __PI * ALL_COUNT_A / 660.f;
  *b = __PI * ALL_COUNT_B / 660.f;
  *c = __PI * ALL_COUNT_C / 660.f;
  *d = __PI * ALL_COUNT_D / 660.f;
}

/// 清除累计计数器
void mecdr_addup_clear(void) {
  ALL_COUNT_A = ALL_COUNT_B = ALL_COUNT_C = ALL_COUNT_D = 0;
}

/// 定时器信号捕获中断, 用于T法测速
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  IC_CAPT_BUILDER(5, A)
  IC_CAPT_BUILDER(3, B)
  IC_CAPT_BUILDER(2, C)
  IC_CAPT_BUILDER(4, D)
}
