// 防止多次包含的头文件保护宏
#ifndef ALL_H
#define ALL_H

// 包含微控制器STC15F2K60S2的头文件，提供特殊功能寄存器的定义
#include <STC15F2K60S2.H>
// 包含标准输入输出库头文件
#include "stdio.h"
// 包含编译器提供的内建函数头文件
#include "intrins.h"

// 定义无符号字符类型别名u8
typedef unsigned char u8;
// 定义无符号整数类型别名u32
typedef unsigned int u32;

// 包含其他模块或功能的头文件
#include "scheduler.h"
#include "time.h"
#include "ctrl.h"
#include "smg.h"
#include "iic.h"
#include "onewire.h"
#include "rtc.h"
#include "key.h"
#include "wave.h"
#include "uart.h"
#include "led.h"

// 定义控制命令的地址
#define CTRL 0xa0
// 定义LED显示的地址
#define LED 0x80
// 定义位选显示的地址
#define WEI 0xc0
// 定义段显示的地址
#define DUAN 0xe0
// 定义关闭门控的操作
#define CLOSE() P2&=0x1f
// 定义打开指定门控的操作
#define OPEN(a) P2|=a

// 定义继电器控制信号
#define JDQ 1<<4
// 定义电机控制信号
#define MOTO 1<<5
// 定义蜂鸣器控制信号
#define FMQ 1<<6

// 定义一个结构体用于存储测量数据
xdata struct _cj//注意结构体内存对齐问题，大的数据类型放上面
{
	u32 freq; //频率
	float t; // 温度
    u8 state; // 总状态
    u8 h; // 时
    u8 m; // 分
    u8 s; // 秒
    u8 d; // 距离
    u8 ad; // 月份
    u8 key; // 按键值
    u8 adc; // 模拟数字转换值
};
extern xdata struct _cj cj;

#endif 