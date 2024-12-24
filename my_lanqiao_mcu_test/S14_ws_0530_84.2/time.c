#include "time.h"
u8 th=0;
u8 tl=0;
bit f_1s=0;
bit f_20ms_key = 0;
bit f_100ms_adc = 0;
bit f_100ms_state = 0;
bit f_100ms_led=0;
bit f_100ms_rtc = 0;
bit f_200ms_uart = 0;
bit f_800ms_t = 0;
bit f_3s=0;
bit f_led=0;

void tim_proc(void);
//******************************************
void pit1(void) interrupt 3
{
	static u16 cnt1s = 0; 
	if(++cnt1s >= 500)
	{
		TR0 = 0;
		th=TH0;tl=TL0;
		TH0 = 0;TL0 = 0;	
		TR0 = 1;
		cnt1s = 0;f_1s=1;		
	}		
	disp_scan();	
	tim_proc();	
}

void Timer1_Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x40;				//设置定时初始值
	TH1 = 0xA2;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
}

void Timer0_Init(void)		//计数器
{
	TMOD &=0xF0;			//清除低位
	TMOD |= 0x04;			//设置计数器模式
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TR0 = 1;				//定时器0开始计时
}


void tim_proc(void)	
{
	static u8 cnt_20ms = 0;
	static u8 cnt_100ms = 0;
	static u8 cnt_200ms = 0;	
	static u16 cnt_800ms = 0;
	static u16 cnt_3s=0;
	static u16 cnt_50=0;
	
	if (++cnt_20ms == 10)
	{
		cnt_20ms = 0;
		f_20ms_key = 1;  f_100ms_rtc=1;
	}
	if (++cnt_100ms == 50)
	{
		cnt_100ms = 0;
		f_100ms_adc = 1;		f_100ms_state = 1;		f_100ms_led=1;  f_led=~f_led;
	}
	if (++cnt_200ms == 100)
	{
		cnt_200ms = 0;
		f_200ms_uart = 1;
	}
	
	if (++cnt_800ms == 400)
	{
		cnt_800ms = 0;
		f_800ms_t = 1;
	}
	
	if(++cnt_3s>=1500)
	{
		cnt_3s=0;
		f_3s=1;
	}
	
	if(++cnt_50>=25)
	{
		cnt_50=0;
		led_mainloop();
	}
}