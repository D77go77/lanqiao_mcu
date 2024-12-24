/*
1）把每个赛题程序都当成  竞赛时的最终上传程序
2）工程包含的模块只能是竞赛时会写出来的模块
3）题目要逐行仔细分析，注意每一个细节要求。
4）程序编好后，认真修改，严格要求。
5）参考的只能是思路，实现要完全靠自己。
6）重要内容要有注释
*/
#include "all.h"
//led
volatile u8 bdata led_dat=0;
sbit L1=led_dat^0;
sbit L2=led_dat^1;
sbit L3=led_dat^2;
sbit L4=led_dat^3;
sbit L5=led_dat^4;
sbit L6=led_dat^5;
sbit L7=led_dat^6;
sbit L8=led_dat^7;
//
xdata struct _cj cj;
//var
u8 run_state=10;
//******************************************************
void main_init(void);
//
void key_mainloop(void);
void led_mainloop(void);
void state_mainloop(void);
void rtc_mainloop(void);
void adc_mainloop(void);
void jdq_proc(void);
//******************************************************
void main(void)
{
	main_init();
	EA = 1;
	//
	while (1)
	{		
		key_mainloop();		
		state_mainloop();
		rtc_mainloop();
		adc_mainloop();
		led_mainloop();
	}
}
//******************************************************
void state_mainloop(void)
{
	if(!f_100ms_state)return;
	f_100ms_state=0;

	switch(run_state)
	{
		case 10://时间湿度界面
			sprintf(dis_str,"%02bu-%02bu %02bu",cj.h,cj.m,cj.rh);
		break;
		
		case 20://设定界面
			sprintf(dis_str,"--    %02bu",cj.rh_tmp);
		break;
	}
	disp_tran();	
}

//******************************************************
void jdq_proc(void)
{
	switch(cj.mode)
	{
		case 0://自动模式
			L1=1;
			if(cj.rh < cj.rh_set) 
			{
				on(JDQ);
			}
			else off(JDQ);
		break;
		case 0xff://手动模式
			L2=1;
			if(cj.rh < cj.rh_set && cj.f_fmq)
			{
				on(FMQ);
			}else off(FMQ);
			//
			if(cj.f_jdq)
			{
				on(JDQ);
			}else off(JDQ);
		break;
	}
}
//*******************************
void adc_mainloop(void)
{
	u8 tmp;
	if (!f_100ms_adc)	return;
	f_100ms_adc = 0;
	//
	tmp=read_adc();//电位器读取
	cj.rh=(u16)tmp*99/255;
}
//******************************************************
void rtc_mainloop(void)//获取时间
{
	u8 h,m,s;
	static u8 s_old=0;
	
	if(!f_100ms_rtc)return;
	f_100ms_rtc=0;
	//P12=0;	
	h=Read_Ds1302_Byte(0x85);
	m=Read_Ds1302_Byte(0x83);
	s=Read_Ds1302_Byte(0x81);
	//P12=1;
	if(s_old==s)return;//秒没有更新，说明没到一秒，直接退。
	s_old=s;
	
	cj.h = (h>>4)*10 + (h&0x0f);
	cj.m = (m>>4)*10 + (m&0x0f);
	cj.s = (s>>4)*10 + (s&0x0f);
}
//******************************************************
void main_init(void)
{
	off(JDQ);off(FMQ);
	led_dat = 0;	
	ds1302_init();//hms
	ds1302_run(0x08,0x30,0x00);//hms
	init_adc(0x03); //Rb2
	Timer1_Init();
	
	run_state=10;
	cj. h=8;
	cj. m=30;
	cj. s=0;
	
	cj. rh=0;//湿度
	cj. rh_set = read_eeprom(0);//读取设定值0~99
	if(cj. rh_set > 99)cj. rh_set=50;
	cj. rh_tmp=cj. rh_set;
	cj. mode=0;//0为自动 ff为手动
	cj. f_jdq=0;//关闭
	cj. f_fmq=0;
	//
}
//******************************************************
void key_mainloop(void)
{
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(!key_code)return;
	
	switch(key_code)
	{
		case S4://--
			switch(run_state)
			{
				case 10:
					if(cj.mode==0xff)cj.f_jdq=0;break;//手动模式//关闭
				break;
				case 20:if(--cj.rh_tmp>=99)cj.rh_tmp=0;break;
			}
			break;
		case S5://++
			switch(run_state)
			{
				case 10:
					if(cj.mode==0xff)cj.f_jdq=1;break;//手动模式//开启
				break;
				case 20:if(++cj.rh_tmp>=99)cj.rh_tmp=99;break;
			}
			break;	
		case S6:
			switch(run_state)
			{
				case 10:
					switch(cj.mode)
					{	
						case 0:run_state=20;break;//自动模式
						case 0xff:cj.f_fmq=~cj.f_fmq;break;//手动模式//开关fmq
					}
				break;
				case 20:run_state=10;
						write_eeprom(0,cj.rh_tmp);
						cj.rh_set=cj.rh_tmp;break;//保存
			}
			break;
		case S7:
			switch(run_state)
			{
				case 10:cj.mode=~cj.mode;break;//切换模式
			}
			break;
	}
	key_code = 0;
}
//******************************************************
void led_mainloop(void)
{	
	if (!f_100ms_led) return;
	f_100ms_led = 0;
	led_dat=0;
	
	jdq_proc();//继电器蜂鸣器led控制
	//
	P0 = ~led_dat;
	gate(LED);
}
