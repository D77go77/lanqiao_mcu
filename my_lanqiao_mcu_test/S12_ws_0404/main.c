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
sbit L4 = led_dat ^3;
sbit L5 = led_dat ^4;
sbit L6 = led_dat ^5;
sbit L7 = led_dat ^6;
sbit L8 = led_dat ^7;
//
xdata struct _cj cj;
//
u8 run_state = 0;
//
void main_init(void);
void state_mainloop(void);
void key_mainloop(void);
void temper_mainloop(void);
void led_mainloop(void);
//
void Delay5ms(void);
void dac_proc(void);
//*******************************
void main(void)
{
	main_init();
	EA = 1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		temper_mainloop();
		led_mainloop();
	}
}
//****************************
void main_init(void)
{
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	

	
	run_state = 10;
	cj. t=0;//温度
	cj. t_set=25;//温度参数
	cj.t_tmp = cj.t_set;
	cj. v=0;//DAC输出
	cj. mode=0;//模式//0为1 ff为2
	
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
		case 10://温度界面
			sprintf(dis_str,"C   %5.2f",cj.t);
		break;
		case 20://参数界面
			sprintf(dis_str,"P     %2bu",cj.t_tmp);
		break;
		case 30://dac输出界面
			sprintf(dis_str,"A   %5.2f",cj.v);
		break;
	}
	disp_tran();
	dac_proc();//dac输出控制
}
//************************
void key_mainloop(void)
{
	u8 _i = 0;
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(!key_code)return;
	
	switch(key_code)
	{
		case S4: //切换界面
			switch(run_state)
			{
				case 10:run_state=20;cj.t_tmp = cj.t_set;break;
				case 20:run_state=30;cj.t_set = cj.t_tmp;break;
				case 30:run_state=10;break;
			}
		break;
		case S8:
			switch(run_state)
			{
				case 20:if(++cj.t_tmp>=99)cj.t_tmp=99;break;//累加
			}
		break;
		case S9: 
			switch(run_state)
			{
				case 20:if(--cj.t_tmp>=99)cj.t_tmp=0;break;//累减
			}
		break;
		case S5:
			cj.mode =~cj.mode;
		break;
	}
	key_code = 0;
}
//*************************
void dac_proc(void)//dac输出模式控制 
{
	switch(cj.mode)//dac输出模式
	{
		case 0:
			if(cj.t<cj.t_set)//模式1;实时温度小于参数，输出0v
			{write_dac(0);cj.v = 0;}
			else
			{write_dac(255);cj.v = 5.0;}
		break;
		case 0xff:
			if(cj.t<=20)
			{write_dac(255/5);cj.v = 1.0;}//输出1v
			else if(cj.t>=40)
			{write_dac(4*255/5);cj.v = 4.0;}//输出4v
			else
			{cj.v = ((3*(float)cj.t-40)/20.0);write_dac(cj.v);}
		break;
	}
}
//*************************
void led_mainloop(void)
{
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	led_dat = 0;
	switch(run_state)
	{
		case 10:L2 = 1;break;
		case 20:L3 = 1;break;
		case 30:L4 = 1;break;
	}
	if(!cj.mode)//模式1亮、既mode=0;
		L1 = 1;
	
	P0 = ~led_dat;
	gate(LED);	
}
//*************************
void temper_mainloop(void)
{
	u16 tmp;
	if(!f_800ms_temp)return;
	f_800ms_temp = 0;

	tmp=read_tem16();
	cj.t = (float)tmp*0.0625;
	
	start_ds18b20();
}
//*****************************
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

