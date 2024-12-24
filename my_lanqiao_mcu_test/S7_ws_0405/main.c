/*
1）把每个赛题程序都当成  竞赛时的最终上传程序
2）工程包含的模块只能是竞赛时会写出来的模块
3）题目要逐行仔细分析，注意每一个细节要求。
4）程序编好后，认真修改，严格要求。
5）参考的只能是思路，实现要完全靠自己。
6）重要内容要有注释
*/
#include "all.h"
//
volatile u8 bdata led_dat = 0;
sbit L1 = led_dat ^0;
sbit L2 = led_dat ^1;
sbit L3 = led_dat ^2;
//
u8 bdata blink8_dat=0;
//
xdata struct _cj cj;
//
u8 run_state = 0;
bit f_led = 0;
//
void main_init(void);
void state_mainloop(void);
void key_mainloop(void);
void led_mainloop(void);
void temper_mainloop(void);

void Delay5ms();
//*******************************
void main(void)
{
	main_init();
	EA = 1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		led_mainloop();
		temper_mainloop();
	}
}
//****************************
void main_init(void)
{
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	Timer2Init();

	
	run_state = 10;
	
	cj. t=0;//温度
	cj. mode=1;;//模式
	cj. tim=0;//定时时间
	
	start_ds18b20();
	Delay5ms();
}
//****************************
void state_mainloop(void)
{
	if(!f_100ms_state) return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10://风扇界面
			sprintf(dis_str,"-%1bu- %04bu",cj.mode,cj.tim);
		break;
		case 20://室温界面
			sprintf(dis_str,"-4-  %02buC",cj.t);
		break;
	}
	disp_tran();
}
//************************
void key_mainloop(void)
{
	static u8 _i = 0;//计数定时累计
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(!key_code)return;
	f_led = 0;//按任意键，关闭闪烁
	switch(key_code)
	{
		case S4:
			switch(cj.mode)//切换模式
			{
				case 1:cj.mode=2;break;
				case 2:cj.mode=3;break;
				case 3:cj.mode=1;break;
			}
		break;
		case S5:
			_i++;
			switch(_i)//定时
			{
				case 1:cj.tim=60;break;
				case 2:cj.tim=120;break;
				case 3:cj.tim=0;_i=0;break;
			}
		break;
		case S7: 
			switch(run_state)//切换界面
			{
				case 10:run_state=20;break;
				case 20:run_state=10;break;
			}
		break;
		case S6: //清除时间
			cj.tim = 0; _i=0;
		break;
	}
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	led_dat = 0;
	switch(cj.mode)
	{
		case 1:L1 = 1;break;
		case 2:L2 = 1;break;
		case 3:L3 = 1;break;
	}
	
	if(!cj.tim) led_dat=0;
	
	P0 = ~led_dat;
	gate(LED);	
}
//************************************
void temper_mainloop(void)
{
	u16 tmp;
	if(!f_800ms_temp)return;
	f_800ms_temp = 0;

	tmp=read_tem16();
	cj. t = (float)tmp*0.0625f;
	start_ds18b20();
}
//************************************
void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}
