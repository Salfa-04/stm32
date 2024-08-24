#include "main.h"
#include "gpio.h"

void SystemClock_Config(void);

int main(void) {

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  GPIO_Init();

  while (1) {
  }
}
