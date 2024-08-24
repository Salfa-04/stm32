#include "main.h"

void HAL_MspInit(void) {

  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/
  __HAL_AFIO_REMAP_SWJ_DISABLE();
}

void Error_Handler(void) {
  __disable_irq();
  while (1) {
  }
}
