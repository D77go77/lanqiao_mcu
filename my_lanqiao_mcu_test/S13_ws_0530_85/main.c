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
bit f_jdq = 0;//继电器开启标志
//
void main_init(void);
void state_mainloop(void);
void key_mainloop(void);
void temper_mainloop(void);
void led_mainloop(void);
void rtc_mainloop(void);
//
void Delay5ms(void);
void mode_proc(void);
//*******************************
void main(void)
{
	main_init();
	EA = 1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		rtc_mainloop();
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

	rtc_run(0x15,0x33,0x45);
	
	run_state = 10;
	cj. t=0;//温度
	cj. t_set=23;//温度参数
	cj.mode = 0;//0为温度模式，ff为时间模式
	cj. h=0x15;
	cj. m=0x33;
	cj. s=0x45;
	
	init_temp();
}
//****************************
void state_mainloop(void)
{
	if(!f_100ms_state) return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10://温度界面
			sprintf(dis_str,"U1   %4.1f",cj.t);
		break;
		case 20://时间界面//hm
			sprintf(dis_str,"U2 %02bu-%02bu",cj.h,cj.m);
		break;
		case 21://ms
			sprintf(dis_str,"U2 %02bu-%02bu",cj.m,cj.s);
		break;
		case 30://参数界面
			sprintf(dis_str,"U3    %02bu",cj.t_set);
		break;
	}
	disp_proc();
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
		case S12: //切换界面
			switch(run_state)
			{
				case 10:run_state=20;break;
				case 20:run_state=30;break;
				case 30:run_state=10;
				break;
			}
		break;
		//
		case S13://切换模式//0为温度控制，ff为时间控制
			cj.mode = ~cj.mode;
			off(JDQ);f_jdq = 0;//切换模式并清除jdq打开情况
		break;
		//
		case S16://++
			switch(run_state)
			{
				case 30:if(++cj.t_set > 99)cj.t_set = 10;break;
			}
		break;
		//
		case S17://--
			switch(run_state)
			{
				case 30:if(--cj.t_set < 10)cj.t_set = 99;break;
				case 20:run_state = 21;break;//按下进入ms
			}
		break;
		//
		case S17OFF:
			switch(run_state)//松开返回hm
			{
				case 21:run_state=20;break;
			}
			break;
	}
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	static u8 t = 0;
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	if(cj.m == 0 && cj.s == 0) 
		L1=1;//整点L1亮
	if(L1)
	{	
		if(++t>=50)
		{
			t=0;L1=0;
		}			
	}//L1亮后5秒后灭
	
	mode_proc();//模式以及继电器控制
	
	P0 = ~led_dat;
	gate(LED);	
}
//*************************
void mode_proc(void)//模式以及继电器控制
{
	static u8 t = 0;//计算5s
	switch(cj.mode)
	{
		case 0://温度控制
			L2=1;
			if(cj.t > cj.t_set)//采集值>参数值//报警
			{
				on(JDQ);f_jdq = 1;
			}else 
			{	
				off(JDQ);f_jdq=0;
			}
		break;
		case 0xff://时间控制
			L2=0;
			if(cj.m == 0&&cj.s == 0)//整点报警
			{
				on(JDQ);f_jdq = 1;
			}
			if(f_jdq)
			{
				if(++t >= 50)//5s后关jdq
				{
					t = 0;off(JDQ);f_jdq=0;
				}
			}
		break;
	}
	if(f_jdq)//jdq打开闪烁
		L3 = ~L3;
	else L3=0;
}
//**************************8
void temper_mainloop(void)
{
	u16 tmp;
	if(!f_800ms_temp)return;
	f_800ms_temp = 0;

	tmp=read_tem();
	cj.t = (float)tmp;
	init_temp();
}
//*****************************
void rtc_mainloop(void)
{
	u8 h,m,s;
	
	if(!f_100ms_rtc)return;
	f_100ms_rtc=0;
	h=Read_Ds1302_Byte(0x85);
	m=Read_Ds1302_Byte(0x83);
	s=Read_Ds1302_Byte(0x81);
	
	cj.h = (h>>4)*10 + (h&0x0f);
	cj.m = (m>>4)*10 + (m&0x0f);
	cj.s = (s>>4)*10 + (s&0x0f);
}

