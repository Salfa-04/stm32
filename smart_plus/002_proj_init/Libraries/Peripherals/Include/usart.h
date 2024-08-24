#ifndef __USART_H
#define __USART_H

#include "main.h"

extern UART_HandleTypeDef huart2;

void uart2_init(void);
void uprintf(const char *fmt, ...);

void USART2_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);

#endif /* __USART_H */
