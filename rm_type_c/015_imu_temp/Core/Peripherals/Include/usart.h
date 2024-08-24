#ifndef __USART_H
#define __USART_H

#include "main.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

void uprintf(const char *fmt, ...);
void MX_USART1_UART_Init(void);

#endif /* __USART_H */
