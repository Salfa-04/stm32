#include "tb6612_def.h"

#include "tb6612.h"

TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim12;

#define M1IN1 GPIO_PIN_15  /// GPIOD 正 C
#define M1IN2 GPIO_PIN_11  /// GPIOD 正 C
#define M2IN1 GPIO_PIN_1   /// GPIOC 正 B
#define M2IN2 GPIO_PIN_3   /// GPIOC 正 B
#define M3IN1 GPIO_PIN_0   /// GPIOC 反 A
#define M3IN2 GPIO_PIN_2   /// GPIOC 反 A
#define M4IN1 GPIO_PIN_14  /// GPIOD 反 D
#define M4IN2 GPIO_PIN_10  /// GPIOD 反 D

void tb6612_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
                    GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD,
                    GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_14 | GPIO_PIN_15,
                    GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD11 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
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

MOTOR_STATES_BUILD(A, for, C, M3IN1, M3IN2)
MOTOR_STATES_BUILD(A, back, C, M3IN2, M3IN1)
MOTOR_STATE_BUILD(A, brack, C, M3IN1 | M3IN2, SET)
MOTOR_STATE_BUILD(A, stop, C, M3IN1 | M3IN2, RESET)

MOTOR_STATES_BUILD(B, for, C, M2IN2, M2IN1)
MOTOR_STATES_BUILD(B, back, C, M2IN1, M2IN2)
MOTOR_STATE_BUILD(B, brack, C, M2IN1 | M2IN2, SET)
MOTOR_STATE_BUILD(B, stop, C, M2IN1 | M2IN2, RESET)

MOTOR_STATES_BUILD(C, for, D, M1IN2, M1IN1)
MOTOR_STATES_BUILD(C, back, D, M1IN1, M1IN2)
MOTOR_STATE_BUILD(C, brack, C, M1IN1 | M1IN2, SET)
MOTOR_STATE_BUILD(C, stop, C, M1IN1 | M1IN2, RESET)

MOTOR_STATES_BUILD(D, for, D, M4IN1, M4IN2)
MOTOR_STATES_BUILD(D, back, D, M4IN2, M4IN1)
MOTOR_STATE_BUILD(D, brack, C, M4IN1 | M4IN2, SET)
MOTOR_STATE_BUILD(D, stop, C, M4IN1 | M4IN2, RESET)

void tb6612_motor_direct(int16_t *a, int16_t *b, int16_t *c, int16_t *d) {
  MOTOR_DIRECT_BUILD(a, A)
  MOTOR_DIRECT_BUILD(b, B)
  MOTOR_DIRECT_BUILD(c, C)
  MOTOR_DIRECT_BUILD(d, D)
}
