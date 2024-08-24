#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"

extern I2C_HandleTypeDef hi2c3;
extern DMA_HandleTypeDef hdma_i2c3_rx;

void MX_I2C3_Init(void);

#endif /* __I2C_H__ */
