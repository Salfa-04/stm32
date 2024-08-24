#ifndef __IST8310_REG_H
#define __IST8310_REG_H

// 0x10 for default (ro)
#define IST8310_REG_WHOAMI 0x00U
// bit0: new data ready; bit1: data out of range (ro)
#define IST8310_REG_STAT1 0x02U
// bit3: interrupt occurred (ro)
#define IST8310_REG_STAT2 0x09U
// messure mode: bit1:0
#define IST8310_REG_CTRL1 0x0AU
#define IST8310_REG_CTRL2 0x0BU
// self-test: bit6; 1: enter; 0: exit
#define IST8310_REG_CNTL2 0x0CU
// Sampling times; x and z: bit2:0; y: bit5:3
#define IST8310_REG_AVGCNTL 0x41U
#define IST8310_REG_DATA 0x03U  // ro: x, y, z; low, high: len6
#define IST8310_REG_TEMP 0x1CU  // ro: low, high: len2

#endif /* __IST8310_REG_H */
