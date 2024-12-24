#include "time.h"

bit f_20ms_key = 0;
bit f_100ms_led = 0;
bit f_100ms_state = 0;
bit f_100ms_adc = 0;

void led_proc();
void tim_proc();
//***************************
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
void Timer1_Isr(void) interrupt 3//2ms
{
	tim_proc();
	disp_scan();	
}
//***********************************
void tim_proc()
{
	static u8 cnt_20ms=0;
	static u8 cnt_100ms=0;

	if(++cnt_20ms>=10)
	{
		cnt_20ms=0;f_20ms_key=1;
	}
	if(++cnt_100ms>=50)
	{
		cnt_100ms=0;f_100ms_state=1;f_100ms_led=1;f_100ms_adc=1;
		led_proc();
	}	
}
//*****************************
void led_proc()
{
	P0=~led_dat;
	gate(LED);
}
//******************

