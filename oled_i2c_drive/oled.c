#include "oled.h"
#include "oledfont.h"

// 初始化命令
uint8_t CMD_Data[] = {
    0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6,
    0xA8, 0x3F, 0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05,
    0xD9, 0xF1, 0xDA, 0x12, 0xD8, 0x30, 0x8D, 0x14, 0xAF};

// 向设备写控制命令
void OLED_WR_CMD(uint8_t cmd)
{
  HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}

// 向设备写数据
void OLED_WR_DATA(uint8_t data)
{
  HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}

// 写初始化命令
void WriteCmd()
{
  uint8_t i = 0;
  for (i = 0; i < 27; i++)
    OLED_WR_CMD(*(CMD_Data + i));
}

// 初始化oled屏幕
void OLED_Init(void)
{
  HAL_Delay(200);
  WriteCmd();
}

// 清屏
void OLED_Clear()
{
  uint8_t i, n;
  for (i = 0; i < 8; i++)
  {
    OLED_WR_CMD(0xb0 + i);
    OLED_WR_CMD(0x00);
    OLED_WR_CMD(0x10);
    for (n = 0; n < 128; n++)
      OLED_WR_DATA(0);
  }
}

// 开启OLED显示
void OLED_Display_On(void)
{
  OLED_WR_CMD(0X8D); // SET DCDC命令
  OLED_WR_CMD(0X14); // DCDC ON
  OLED_WR_CMD(0XAF); // DISPLAY ON
}

// 关闭OLED显示
void OLED_Display_Off(void)
{
  OLED_WR_CMD(0X8D); // SET DCDC命令
  OLED_WR_CMD(0X10); // DCDC OFF
  OLED_WR_CMD(0XAE); // DISPLAY OFF
}

void OLED_Set_Pos(uint8_t x, uint8_t y)
{
  OLED_WR_CMD(0xb0 + y);
  OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10);
  OLED_WR_CMD(x & 0x0f);
}

// 显示ASCII字符chr 字符大小：8x16 / 6x8
// x, y: 显示位置坐标 x=>(0..128) y=>(0..64)
// Char_Size: 选择字体大小 16 => 8x16, _ => 6x8
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
  uint8_t i, c = chr - ' '; // 得到偏移后的值

  if (x > 128 - 1)
  {
    x = 0;
    y += 2;
  }

  if (Char_Size == 16)
  { // 8X16 点阵
    OLED_Set_Pos(x, y);
    for (i = 0; i < 8; i++)
      OLED_WR_DATA(F8x16[c * 16 + i]);
    OLED_Set_Pos(x, y + 1);
    for (i = 0; i < 8; i++)
      OLED_WR_DATA(F8x16[c * 16 + i + 8]);
  }
  else
  { // 6X8 点阵
    OLED_Set_Pos(x, y);
    for (i = 0; i < 6; i++)
      OLED_WR_DATA(F6x8[c][i]);
  }
}

// 显示ASCII字符串chr[] 字符大小：8x16 / 6x8
//
// x, y: 显示位置的初始坐标 x=>(0..128) y=>(0..64)
// Char_Size: 选择字体大小 16 => 8x16, _ => 6x8
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size)
{
  uint8_t j = 0;

  while (chr[j] != '\0')
  {
    OLED_ShowChar(x, y, chr[j++], Char_Size);
    x += 8;

    if (x > 120)
    {
      x = 0;
      y += 2;
    }
  }
}

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
void OLED_ShowHzk(uint8_t x, uint8_t y, uint8_t hzk[][16], uint8_t no)
{
  uint8_t t;

  OLED_Set_Pos(x, y);
  for (t = 0; t < 16; t++)
    OLED_WR_DATA(hzk[2 * no][t]);

  OLED_Set_Pos(x, y + 1);
  for (t = 0; t < 16; t++)
    OLED_WR_DATA(hzk[2 * no + 1][t]);
}

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
void OLED_ShowHzkS(uint8_t x, uint8_t y, uint8_t hzk[][16], uint8_t hzk_len)
{

  uint8_t i, t;
  for (i = 0; i < hzk_len; i++)
  {
    OLED_ShowHzk(x, y, hzk, i);

    x += 16;
    if (x > 120)
    {
      x = 0;
      y += 2;
    }
  }
}
