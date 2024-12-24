#include "time.h"
idata unsigned long int uwTick = 0;   //系统计时变量
u8 f_on=0;//led闪烁标志
u8 f_wei=0;//smg闪烁标志

void tim_proc();
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
    tim_proc();  // 调用时间处理函数
}

// 时间处理函数，用于处理不同的时间间隔事件
void tim_proc()
{
    static u8 cnt_20ms = 0;
    static u32 cnt_800ms = 0;
    static u32 cnt_1s = 0;
    
    // 每10次中断（即每20ms）执行一次LED扫描
    if (++cnt_20ms >= 10) {
        cnt_20ms = 0;
        led_scan(); // 调用LED扫描函数
    }

    // 每400次中断（即每800ms）切换f_wei和f_on的状态
    if (++cnt_800ms >= 400) {
        cnt_800ms = 0;
        f_wei = !f_wei; // 切换f_wei状态
        f_on = !f_on;   // 切换f_on状态
    }
    
    // 每500次中断（即每1s）更新频率计数并清空定时器0
    if (++cnt_1s >= 500) {
        cnt_1s = 0;
        cj.freq = (TH0 << 8) | TL0; // 计算频率
        TH0 = TL0 = 0;           // 清空定时器0
    }
    
    // 如果uu.ton标志位为真，则执行定时关断逻辑
    if (uu.ton) {
        if (++uu.t >= 20) {
            uu.cmd = 2; // 设置命令为2
        }
    }
}