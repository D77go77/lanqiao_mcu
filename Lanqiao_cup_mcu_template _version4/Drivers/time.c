#include "time.h"
idata unsigned long int uwTick = 0;   //系统计时变量
void ne555_proc();
//**************************
void Timer0_Init(void)
{
  AUXR &= 0x7F; // 定时器时钟1ms 12T模式
  TMOD &= 0xF0; // 设置定时器模式
  TMOD |= 0x05; // 设置为模式5（定时器模式）//计数器模式
  TL0 = 0;      // 设置初始值
  TH0 = 0;      // 设置初始值
  TF0 = 0;      // 清除TF0标志
  TR0 = 1;      // 启动定时器0
}
//**************************
void Timer1_Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x40;				//设置定时初始值
	TH1 = 0xA2;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;
}

// 定时器1中断服务程序，每进入中断一次，uwTick自增1
void pit1() interrupt 3
{
    uwTick++;//系统总运行时长
    disp_scan(); // 调用数码管扫描函数
    ne555_proc();  // 调用时间处理函数
}

// ne555处理函数
void ne555_proc()
{
    static u32 cnt_1s = 0;
    // 每500次中断（即每1s）更新频率计数并清空定时器0
    if (++cnt_1s >= 500) {
        cnt_1s = 0;
        cj.freq = (TH0 << 8) | TL0; // 计算频率
        TH0 = TL0 = 0;           // 清空定时器0
    }
}