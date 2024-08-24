#ifndef __IST8310_H__
#define __IST8310_H__

#include "main.h"

typedef struct ist8310_mag {
  float x;
  float y;
  float z;
} ist8310_t;

void ist8310_init(void);
void ist8310_read_mag(ist8310_t *mag);

#define IST8310_RST_Pin GPIO_PIN_6
#define IST8310_DRDY_Pin GPIO_PIN_3

#endif /* __IST8310_H__ */
