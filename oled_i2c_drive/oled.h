#ifndef __OLED_H_
#define __OLED_H_

#include "i2c.h"

// 向设备写控制命令
void OLED_WR_CMD(uint8_t cmd);

// 向设备写数据
void OLED_WR_DATA(uint8_t data);

// 初始化oled屏幕
void OLED_Init(void);

// 清屏
void OLED_Clear(void);

// 开启OLED显示
void OLED_Display_On(void);

// 关闭OLED显示
void OLED_Display_Off(void);

// 显示ASCII字符chr 字符大小：8x16 / 6x8
//
// x, y: 显示位置坐标 x=>(0..128) y=>(0..64)
// Char_Size: 选择字体大小 16 => 8x16, _ => 6x8
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);

// 显示ASCII字符串chr[] 字符大小：8x16 / 6x8
//
// x, y: 显示位置的初始坐标 x=>(0..128) y=>(0..64)
// Char_Size: 选择字体大小 16 => 8x16, _ => 6x8
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size);

// 显示自定义字符 字符大小：16x16
//
// x, y: 显示位置的初始坐标 x=>(0..=112) y=>(0..=6)
// hzk: 字库字符串取模指针，即用取模软件得出的数组`Hzk[][16]`的指针
// no: 字符序号，即用取模软件得出的数组`Hzk[][16]`的索引
// ```
// 每个自定义字符，高2宽16
// 112 + 16 = 128 (屏幕宽度边界)
// 6 + 2 = 8 (屏幕高度边界)
// ```
void OLED_ShowHzk(uint8_t x, uint8_t y, uint8_t hzk[][16], uint8_t no);

// 显示自定义字符串`*hzk[16]` 字符大小：16x16
//
// x, y: 显示位置的初始坐标 x=>(0..=112) y=>(0..=6)
// hzk: 字库字符串取模指针，即用取模软件得出的数组`Hzk[][16]`的指针
// hzk_len: 自定义字符串的字的个数
// ```
// 每个自定义字符显示时: 高2宽16
// 112 + 16 = 128 (屏幕宽度边界)
// 6 + 2 = 8 (屏幕高度边界)
// ```
void OLED_ShowHzkS(uint8_t x, uint8_t y, uint8_t hzk[][16], uint8_t hzk_len);

#endif
