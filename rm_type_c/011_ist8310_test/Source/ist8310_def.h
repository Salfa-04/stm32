#ifndef __IST8310_DEF_H
#define __IST8310_DEF_H

#include "main.h"

void ist8310_reset(void);
void ist8310_gpio_init(void);

void ist8310_delay_long(void);
void ist8310_delay_short(void);

void ist8310_read(uint16_t reg, uint8_t *data);
void ist8310_read_multi(uint16_t reg, uint8_t *data, uint8_t len);

void ist8310_write(uint16_t reg, uint8_t data);
void ist8310_write_multi(uint16_t reg, uint8_t *data, uint8_t len);

#endif /* __IST8310_DEF_H */
