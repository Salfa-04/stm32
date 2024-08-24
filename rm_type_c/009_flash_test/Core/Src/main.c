#include "main.h"

#include <stdint.h>

#include "dma.h"
#include "gpio.h"
#include "source.h"
#include "usart.h"

uint8_t uart_buff[4] = {0};

int main(void) {
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();

  HAL_UART_Receive_IT(&huart1, uart_buff, sizeof(uart_buff));

  uprintf("Hello, world!\n");

  HAL_Delay(300);

  while (1) {
    for (uint8_t i = 0; i < 3; i++) {
      HAL_GPIO_TogglePin(GPIOH, 1 << (10 + i));
      HAL_Delay(100);
    }
  }
}

#define FLASH_SECTOR_8_ADDR 0x08080000U

/*
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    if (uart_buff[0] == 0x00) {  // unlock
      uint8_t status = HAL_FLASH_Unlock();
      uprintf("unlock: %d\n", status);
    }

    if (uart_buff[0] == 0x01) {  // erase
      FLASH_EraseInitTypeDef erase_init = {0};
      erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
      erase_init.Sector = FLASH_SECTOR_8;
      erase_init.NbSectors = 1;
      erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3;

      uint32_t error = 0;
      uint8_t status = HAL_FLASHEx_Erase(&erase_init, &error);

      uprintf("erase: %d, error: %d\n", status, error);
    }

    if (uart_buff[0] == 0x02) {
      uint8_t status = HAL_FLASH_Program(
          FLASH_TYPEPROGRAM_HALFWORD, FLASH_SECTOR_8_ADDR + uart_buff[1],
          (uint64_t)(uart_buff[3] << 8 | uart_buff[2]));

      uprintf("flash: %d\n", status);
    }

    if (uart_buff[0] == 0x03) {
      uint8_t status = HAL_FLASH_Lock();
      uprintf("lock: %d\n", status);
    }
  }

  HAL_UART_Receive_IT(huart, uart_buff, sizeof(uart_buff));
}
