#include "ist8310.h"

#include "ist8310_def.h"
#include "ist8310_reg.h"
#include "usart.h"

#define PANIC()                                        \
  while (1) {                                          \
    HAL_GPIO_TogglePin(GPIOH, 1 << 11);                \
    ist8310_delay_long();                              \
    uprintf("Paniced at %s:%d\n", __FILE__, __LINE__); \
  }

#define IST8310_INIT_CMD_NUM 4
#define MAG_SEN 0.3f

const uint8_t ist8310_init_cmd[IST8310_INIT_CMD_NUM][2] = {
    {IST8310_REG_CTRL2, 0x08},    // enalbe interrupt and low pin polarity
    {IST8310_REG_AVGCNTL, 0x09},  // average 2 times
    {0x42, 0xC0},                 // must be 0xC0
    {IST8310_REG_CTRL1, 0x0B},    // 200Hz output rate
};

void ist8310_init(void) {
  ist8310_gpio_init();
  ist8310_reset();

  uint8_t data_buff = 0;
  ist8310_read(IST8310_REG_WHOAMI, &data_buff);
  for (uint8_t writeNum = 0; writeNum < IST8310_INIT_CMD_NUM; writeNum++) {
    ist8310_write(ist8310_init_cmd[writeNum][0], ist8310_init_cmd[writeNum][1]);
    ist8310_delay_short();
  }

  if (data_buff != 0x10) PANIC();
}

void ist8310_read_mag(ist8310_t *mag) {
  uint8_t buf[6];
  int16_t temp = 0;
  ist8310_read_multi(IST8310_REG_DATA, buf, 6);

  temp = (int16_t)((buf[1] << 8) | buf[0]);
  mag->x = MAG_SEN * temp;
  temp = (int16_t)((buf[3] << 8) | buf[2]);
  mag->y = MAG_SEN * temp;
  temp = (int16_t)((buf[5] << 8) | buf[4]);
  mag->z = MAG_SEN * temp;
}
