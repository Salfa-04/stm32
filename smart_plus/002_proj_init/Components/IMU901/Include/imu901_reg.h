#ifndef __IMU901_REG_H
#define __IMU901_REG_H

#define SAVE 0x00        // 保存配置
#define SENCAL 0x01      // 设置传感器校准
#define SENSTA 0x02      // 读取传感器校准状态
#define GYROFSR 0x03     // 设置陀螺仪量程
#define ACCFSR 0x04      // 设置加速度计量程
#define GYROBW 0x05      // 设置陀螺仪带宽
#define ACCBW 0x06       // 设置加速度带宽
#define BAUD 0x07        // 设置波特率
#define RETURNSET 0x08   // 设置回传内容
#define RETURNSET2 0x09  // 设置回传内容2（保留）
#define RETURNRATE 0x0A  // 设置回传速率
#define ALG 0x0B         // 设置算法
#define ASM 0x0C         // 设置安装方向
#define GAUCAL 0x0D      // 设置陀螺仪自校准开关
#define BAUCAL 0x0E      // 设置气压计自校准开关
#define LEDOFF 0x0F      // 设置LED开关
#define D0MODE 0x10      // 设置端口D0模式
#define D1MODE 0x11      // 设置端口D1模式
#define D2MODE 0x12      // 设置端口D2模式
#define D3MODE 0x13      // 设置端口D3模式
#define RESET 0x7F       // 恢复默认设置

/*
RETURNRATE: 设置回传速率
0：SRATE_250HZ
1：SRATE_200HZ
2：SRATE_125HZ
3：SRATE_100HZ
4：SRATE_50HZ
*/

/*
RETURNSET: 设置回传内容 (0: 不上传 1: 上传)
Bit 0: 上传姿态角开关
Bit 1: 上传四元数开关
Bit 2: 上传陀螺仪和加速度计数据开关
Bit 3: 上传磁力计数据开关
Bit 4: 上传气压计数据开关
Bit 5: 上传端口状态数据开关
Bit 6: 上传数据到匿名上位机开关
*/

/*
SENSTA: 读取传感器校准状态 (0: 未校准 1: 已经校准)
Bit 0：加速度计校准
Bit 1：磁力计校准
Bit 2：陀螺仪校准
说明：如果选择了陀螺仪自校准，上电后该位为 0，自校准通过后为 1，否则都为 1。陀螺
仪自校准需要上电后保持静止（不要手持）1 秒以上才能校准通过！
*/

#endif /* __IMU901_REG_H */
