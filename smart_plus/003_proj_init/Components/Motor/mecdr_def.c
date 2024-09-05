#include "mecdr_def.h"

#include "mecdr.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim6;

void mecdr_gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* TIM2 TIM3 TIM4 TIM5 clock enable */
  __HAL_RCC_TIM2_CLK_ENABLE();
  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_TIM4_CLK_ENABLE();
  __HAL_RCC_TIM5_CLK_ENABLE();

  /* GPIOA GPIOB GPIOC GPIOD clock enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*TIM GPIO Configuration*/
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /**TIM2 GPIO Configuration
  PA5     ------> TIM2_CH1
  PB3     ------> TIM2_CH2
  */
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**TIM3 GPIO Configuration
  PA6     ------> TIM3_CH1
  PA7     ------> TIM3_CH2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**TIM4 GPIO Configuration
  PD12     ------> TIM4_CH1
  PD13     ------> TIM4_CH2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /**TIM5 GPIO Configuration
  PA0-WKUP     ------> TIM5_CH1
  PA1          ------> TIM5_CH2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

TIM_SETUP_BUILD(2, RISING, 0)   // 正相捕获
TIM_SETUP_BUILD(3, FALLING, 0)  // 反相捕获
TIM_SETUP_BUILD(4, FALLING, 0)  // 反相捕获
TIM_SETUP_BUILD(5, RISING, 0)   // 正相捕获

TIM_IRQ_BUILDER(TIM2, 2)
TIM_IRQ_BUILDER(TIM3, 3)
TIM_IRQ_BUILDER(TIM4, 4)
TIM_IRQ_BUILDER(TIM5, 5)

void mecdr_tim6_init(void) {
  /* TIM6 clock enable */
  __HAL_RCC_TIM6_CLK_ENABLE();

  htim6.Instance = TIM6;      // 84MHz -> 1MHz -> 100Hz(10ms)
  htim6.Init.Prescaler = 83;  // 84Mhz / 1MHz = 84-1
  htim6.Init.Period = 9999;   // 1MHz / 100Hz = 10k-1
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim6);

  /* TIM6 interrupt Init */
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

  HAL_TIM_Base_Start(&htim6);
}

TIM_IRQ_BUILDER(TIM6_DAC, 6)
