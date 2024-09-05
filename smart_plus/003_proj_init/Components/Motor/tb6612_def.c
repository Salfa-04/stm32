#include "tb6612_def.h"

#include "tb6612.h"

TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim12;

#define AIN1 GPIO_PIN_13  /// GPIOE 反
#define AIN2 GPIO_PIN_14  /// GPIOE 反
#define BIN1 GPIO_PIN_11  /// GPIOE 反
#define BIN2 GPIO_PIN_12  /// GPIOE 反
#define CIN1 GPIO_PIN_9   /// GPIOE 正
#define CIN2 GPIO_PIN_10  /// GPIOE 正
#define DIN1 GPIO_PIN_7   /// GPIOE 正
#define DIN2 GPIO_PIN_8   /// GPIOE 正

void tb6612_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /**TIM9 GPIO Configuration
  PE5     ------> TIM9_CH1
  PE6     ------> TIM9_CH2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /**TIM12 GPIO Configuration
  PB14     ------> TIM12_CH1
  PB15     ------> TIM12_CH2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE,
                    AIN1 | AIN2 | BIN1 | BIN2 | CIN1 | CIN2 | DIN1 | DIN2,
                    GPIO_PIN_RESET);

  /*Configure GPIO pins */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  /*Configure GPIO pins : PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 */
  GPIO_InitStruct.Pin = AIN1 | AIN2 | BIN1 | BIN2 | CIN1 | CIN2 | DIN1 | DIN2;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/// TIM9 168MHz
/// 168mHz / 15kHz = 11200
/// 11200 / 5600 = 1+1
/// pwm_max = 5600; 60% = 3360
TIM_SETUP_BUILD(9, 1)

/// TIM12 84MHz
/// 84mHz / 15kHz = 5600
/// 5600 / 5600 = 0+1
/// pwm_max = 5600; 60% = 3360
TIM_SETUP_BUILD(12, 0)

MOTOR_STATEX_BUILD(A, for, 2, 1)
MOTOR_STATEX_BUILD(A, back, 1, 2)
MOTOR_STATEY_BUILD(A, brack, SET)
MOTOR_STATEY_BUILD(A, stop, RESET)

MOTOR_STATEX_BUILD(B, for, 2, 1)
MOTOR_STATEX_BUILD(B, back, 1, 2)
MOTOR_STATEY_BUILD(B, brack, SET)
MOTOR_STATEY_BUILD(B, stop, RESET)

MOTOR_STATEX_BUILD(C, for, 1, 2)
MOTOR_STATEX_BUILD(C, back, 2, 1)
MOTOR_STATEY_BUILD(C, brack, SET)
MOTOR_STATEY_BUILD(C, stop, RESET)

MOTOR_STATEX_BUILD(D, for, 1, 2)
MOTOR_STATEX_BUILD(D, back, 2, 1)
MOTOR_STATEY_BUILD(D, brack, SET)
MOTOR_STATEY_BUILD(D, stop, RESET)

void tb6612_motor_direct(int16_t *a, int16_t *b, int16_t *c, int16_t *d) {
  MOTOR_DIRECT_BUILD(a, A)
  MOTOR_DIRECT_BUILD(b, B)
  MOTOR_DIRECT_BUILD(c, C)
  MOTOR_DIRECT_BUILD(d, D)
}
