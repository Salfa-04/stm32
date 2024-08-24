#include "tim.h"

TIM_HandleTypeDef htim10;

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

/* TIM10 init function */
void MX_TIM10_Init(void) {
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 0;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 4999;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim10);
  HAL_TIM_PWM_Init(&htim10);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_MspPostInit(&htim10);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* timHandle) {
  if (timHandle->Instance == TIM10) {
    /* TIM10 clock enable */
    __HAL_RCC_TIM10_CLK_ENABLE();
  }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (timHandle->Instance == TIM10) {
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**TIM10 GPIO Configuration
    PF6     ------> TIM10_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    HAL_TIM_Base_Start(&htim10);
    HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
  }
}
