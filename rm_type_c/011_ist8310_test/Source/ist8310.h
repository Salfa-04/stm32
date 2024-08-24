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

#endif /* __IST8310_H__ */
