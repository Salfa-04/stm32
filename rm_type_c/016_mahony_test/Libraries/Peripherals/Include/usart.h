#ifndef __USART_H
#define __USART_H

#include "main.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

void uart1_init(void);
void uprintf(const char *fmt, ...);

#endif /* __USART_H */
