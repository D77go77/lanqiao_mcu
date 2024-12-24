#include "time.h"
bit f_20ms_key=0;
bit f_100ms_state=0;
bit f_50ms_led=0;
bit f_100ms_wave=0;
bit f_100ms_adc=0;
bit f_800ms_tem=0;
bit f_freq = 0;
u8 f_on=0;

u16 ping_dat=0;
u8 pwm_set=0;

void tim_proc();
void led_proc();
void moto_proc();
void ne555_proc();
//*****************************
void ne555_init(void)	// 定时器0
{
	TMOD &= 0xF0;		//设置定时器模式
    TMOD |= 0x05;       
	
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}
//
void Timer2_Init(void)		//200微秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0xA0;				//设置定时初始值
	T2H = 0xF6;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
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
//**********************************
void Timer1_Isr(void) interrupt 3//2ms
{
	tim_proc();
	disp_scan();	
	ne555_proc();
}
//
void Timer2_Isr(void) interrupt 12//200us
{
	moto_proc();
}
//*****************************
void tim_proc()
{
	static u8 cnt_20ms=0;
	static u8 cnt_50ms=0;
	static u8 cnt_100ms=0;
	static u16 cnt_800ms=0;

	if(++cnt_20ms>=10)
	{
		cnt_20ms=0;f_20ms_key=1;
	}
	if(++cnt_50ms>=25)
	{
		cnt_50ms=0;f_50ms_led=1;
	}
	if(++cnt_100ms>=50)
	{
		cnt_100ms=0;f_100ms_state=1;f_100ms_wave=1;f_100ms_adc=1;
		f_on=~f_on; 
		led_proc();
		f_freq=1;
	}

	if(++cnt_800ms>=400)
	{
		cnt_800ms=0;f_800ms_tem=1;
	}
}
//*****************************
void led_proc()
{
	P0=~led_dat;
	gate(LED);
}
//******************
void moto_proc()
{
	static u8 _cnt=0;
	
	if(++_cnt==5)_cnt=0;
	
	if(_cnt<pwm_set)
		on(MOTO);
	else
		off(MOTO);
}
//**********************
void ne555_proc()
{
	static u16 cnt_1000ms = 0;

	if(++cnt_1000ms >= 500)
	{
		cnt_1000ms = 0;			
		ping_dat = ((u16)TH0 << 8) |TL0;//读取频率
		TH0 = 0;TL0 = 0;
	}
}








