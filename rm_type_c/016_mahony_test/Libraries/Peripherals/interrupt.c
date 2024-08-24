#include "stm32f4xx_hal.h"

/** @ref the startup file (startup_stm32f4xx.s) */

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&huart1); }

/**
 * @brief This function handles DMA2 stream7 global interrupt.
 */
void DMA2_Stream7_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_usart1_tx); }
