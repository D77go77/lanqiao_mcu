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
void rtc_mainloop(void);
void temper_mainloop(void);
void blink8_mainloop(void);
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
		rtc_mainloop();
		led_mainloop();
		temper_mainloop();
		blink8_mainloop();
	}
}
//****************************
void main_init(void)
{
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	ds1302_init();
	ds1302_run(0x23,0x59,0x50);
	
	run_state = 10;
	
	cj. h_set=0;//闹钟值
	cj. m_set=0;
	cj. s_set=0;
	
	cj. h_tmp=0;//设置值
	cj. m_tmp=0;
	cj. s_tmp=0;
	
	cj. t=0;
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
		case 10://时钟界面
			blink8_dat = 0;
			if(key_code == S4)
				sprintf(dis_str,"     %02buC",cj.t);	
			else
				sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h,cj.m,cj.s);
		break;
		case 20://时钟设置界面h
			blink8_dat = 0x03;
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h_tmp,cj.m_tmp,cj.s_tmp);
		break;
		case 21://时钟设置界面m
			blink8_dat = 0x18;
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h_tmp,cj.m_tmp,cj.s_tmp);
		break;
		case 22://时钟设置界面s
			blink8_dat = 0xc0;
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h_tmp,cj.m_tmp,cj.s_tmp);
		break;
		case 30://闹钟设置界面h
			blink8_dat = 0x03;
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h_tmp,cj.m_tmp,cj.s_tmp);
		break;	
		case 31://闹钟设置界面m
			blink8_dat = 0x18;
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h_tmp,cj.m_tmp,cj.s_tmp);
		break;
		case 32://闹钟设置界面s
			blink8_dat = 0xc0;
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h_tmp,cj.m_tmp,cj.s_tmp);
		break;
	}
	disp_tran();
}
//************************
void key_mainloop(void)
{
	u8 _i = 0;
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(!key_code)return;
	f_led = 0;//按任意键，关闭闪烁
	switch(key_code)
	{
		case S4: //--
			switch(run_state)
			{
				case 20:if(--cj.h_tmp>23)cj.h_tmp=23;break;
				case 21:if(--cj.m_tmp>59)cj.m_tmp=59;break;
				case 22:if(--cj.s_tmp>59)cj.s_tmp=59;break;
				case 30:if(--cj.h_tmp>23)cj.h_tmp=23;break;	
				case 31:if(--cj.m_tmp>59)cj.m_tmp=59;break;
				case 32:if(--cj.s_tmp>59)cj.s_tmp=59;break;
			}
		break;
		case S5://++
			switch(run_state)
			{
				case 20:if(++cj.h_tmp>23)cj.h_tmp=0;break;
				case 21:if(++cj.m_tmp>59)cj.m_tmp=0;break;
				case 22:if(++cj.s_tmp>59)cj.s_tmp=0;break;
				case 30:if(++cj.h_tmp>23)cj.h_tmp=0;break;	
				case 31:if(++cj.m_tmp>59)cj.m_tmp=0;break;
				case 32:if(++cj.s_tmp>59)cj.s_tmp=0;break;
			}
		break;
		case S7: //时钟设置
			switch(run_state)
			{
				case 10:run_state = 30;
					cj.h_tmp = cj.h;
					cj.m_tmp = cj.m;
					cj.s_tmp = cj.s;
				break;
				case 30:run_state = 31;break;	
				case 31:run_state = 32;break;
				case 32:run_state = 10;
					cj.h = cj.h_tmp;
					cj.m = cj.m_tmp;
					cj.s = cj.s_tmp;
					ds1302_run((cj.h/10*16+cj.h%10), \
								(cj.m/10*16+cj.m%10), \
									(cj.s/10*16+cj.s%10));//保存运行时钟
				break;
			}
		break;
		case S6: //闹钟设置
			switch(run_state)
			{
				case 10:run_state = 20;
					cj.h_tmp = cj.h_set;
					cj.m_tmp = cj.m_set;
					cj.s_tmp = cj.s_set;
				break;
				case 20:run_state = 21;break;
				case 21:run_state = 22;break;
				case 22:run_state = 10;
					cj.h_set = (u8)cj.h_tmp;
					cj.m_set = (u8)cj.m_tmp;
					cj.s_set = (u8)cj.s_tmp;
				break;
			}
		break;
	}
	if(key_code == S4 && run_state == 10)return;
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	static _cnt5s = 0;
	static _cnt200ms = 0;
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	if(cj.m_set == cj.m && cj.s_set == cj.s)//整点
	{f_led  = 1;_cnt5s=0;}//开始闪烁
	
	if(f_led)//
	{
		if(++_cnt200ms>=2)
		{
			L1 = ~L1;_cnt200ms = 0;
		}
		if(++_cnt5s >= 50) f_led = 0;
	}else L1 = 0;
	
	P0 = ~led_dat;
	gate(LED);	
}
//*****************************
void rtc_mainloop(void)
{
	u8 h,m,s;
	static u8 s_old=0;
	
	if(!f_100ms_rtc)return;
	f_100ms_rtc=0;
	h=Read_Ds1302_Byte(0x85);
	m=Read_Ds1302_Byte(0x83);
	s=Read_Ds1302_Byte(0x81);
	if(s_old==s)return;//秒没有更新，说明没到一秒，直接退。
	s_old=s;
	
	cj.h = (h>>4)*10 + (h&0x0f);
	cj.m = (m>>4)*10 + (m&0x0f);
	cj.s = (s>>4)*10 + (s&0x0f);
}
//**********************************
void blink8_mainloop(void)
{
	u8 i=0,tmp = 0;
	
	if (!f_1s_blink8) return;//500ms
	f_1s_blink8 = 0;
	
	//if(blink8_dat == 0)return;
	
	for(i = 0;i <8;i++)
	{
		tmp  = blink8_dat & (1 << i);
		if(tmp)
			dis_blink[i] = ~dis_blink[i];
		else
			dis_blink[i] = 0;
	}
}
//************************************
void temper_mainloop(void)
{
	u16 tmp;
	if(!f_800ms_temp)return;
	f_800ms_temp = 0;

	tmp=read_tem16();
	cj.t = (float)tmp*0.0625f;
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
