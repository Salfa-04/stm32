#ifndef __IST8310_H
#define __IST8310_H

#include "main.h"

extern I2C_HandleTypeDef hi2c3;

typedef struct ist8310_mag {
  float x;
  float y;
  float z;
} ist8310_t;

uint8_t ist8310_init(void);
void ist8310_read_mag(ist8310_t *mag);
void ist8310_read_from_raw(ist8310_t *mag, uint8_t *raw);

#define IST8310_RST_Pin GPIO_PIN_6
#define IST8310_DRDY_Pin GPIO_PIN_3

#endif /* __IST8310_H */
