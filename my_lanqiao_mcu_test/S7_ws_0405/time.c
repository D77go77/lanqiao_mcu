#include "time.h"
bit f_1s=0;
bit f_20ms_key = 0;
bit f_100ms_state = 0;
bit f_100ms_adc = 0;
bit f_100ms_led=0;
bit f_100ms_rtc = 0;
bit f_800ms_temp=0;
bit f_1s_blink8= 0;
void timflag_proc(void);
void fan_proc(void);
void Timer2Init(void);
//******************************************
void PIT1(void) interrupt 3
{
	disp_scan();	
	timflag_proc();	
}
//
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
//
void timflag_proc(void)	
{
	static u8 cnt_20ms = 0;
	static u8 cnt_100ms = 0;
	static u16 cnt_800ms = 0;
	static u16 cnt_1s = 0;
	
	if (++cnt_20ms >= 10)
	{
		cnt_20ms = 0;
		f_20ms_key = 1;
	}
	if (++cnt_100ms >= 50)
	{
		cnt_100ms = 0;
		f_100ms_state = 1;f_100ms_led=1;f_100ms_adc = 1;f_100ms_rtc =1;
	}
	if(++cnt_800ms>=400)
	{
		cnt_800ms = 0;f_800ms_temp =1;
	}
	if(++cnt_1s >= 500)
	{
		cnt_1s = 0;
		f_1s = 1;
		if(cj.tim) cj.tim--;
	}
}
//*************************
void fan_proc(void)//输出
{
	static u8 t=0;
	if(!cj.tim)return;//倒计时结束不输出
	if(++t==10) t=0;
	switch(cj.mode)
	{
		case 1:
			if(t>=2) P34=0;
			else P34=0xff;
		break;
		
		case 2:
			if(t>=3) P34=0;
			else P34=0xff;
		break;
		
		case 3:
			if(t>=7) P34=0;
			else P34=0xff;
		break;
	}
}

void Timer2Init(void)		//100微秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x50;		//设置定时初值
	T2H = 0xFB;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
}


void Timer2_Isr(void) interrupt 12
{
	fan_proc();//1khz
}