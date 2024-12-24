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
u8 k_err_cnt = 0;//无效按键次数
//
void main_init(void);
void state_mainloop(void);
void key_mainloop(void);
void led_mainloop(void);
void adc_mainloop(void);
//
void cnt_proc(void);
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
		adc_mainloop();
	}
}
//****************************
void main_init(void)
{
	u8 tmp;
	off(JDQ);off(FMQ);
	led_dat = 0;	
	init_adc(0x03);
	Timer1_Init();
	
	tmp = read_e2prom(0x00);
	if(tmp>=50.0) tmp = 50;
	cj.v_set=(float)tmp/10.0;
	
	run_state = 10;
	cj.v=0;
	
	cj.cnt=0;
	cj.adc = 0;

}
//****************************
void state_mainloop(void)
{
	if(!f_100ms_state) return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10://数据界面
			sprintf(dis_str,"U    %4.2f",cj.v);
		break;
		case 20://参数界面
			sprintf(dis_str,"P    %4.2f",cj.v_set);
		break;
		case 30://计数界面
			sprintf(dis_str,"N%7bu",cj.cnt);
		break;
	}
	disp_tran();
	cnt_proc();
}
//************************
void cnt_proc(void)//计数次数控制
{
	static u8 f_down = 0;//检测沿
	static u8 f_up = 0;
	
	if(cj.v>cj.v_set)//
	f_up = 0xff;
	
	if(cj.v<cj.v_set && f_up)//下降沿检测
	f_down = 0xff;
	
	if(f_down && f_up)
	{
		cj.cnt++;
		f_down = f_up = 0;
	}
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
		case S12: 
			switch(run_state)
			{
			case 10:run_state=20;k_err_cnt = 0;break;
			case 20:run_state=30;write_e2prom(0,cj.v_set*10);k_err_cnt = 0;break;//参数界面保存
			case 30:run_state=10;k_err_cnt = 0;break;
			default:k_err_cnt++;break;//无效按键累加
			}

		break;
		case S13:
			switch(run_state)
			{
			case 30:cj.cnt = 0;k_err_cnt = 0;break;//清除
			default:k_err_cnt++;break;//无效按键累加
			}
			
		break;
		case S16: 
			switch(run_state)
			{
			case 20:
				cj.v_set+=0.5;k_err_cnt = 0;//清除无效按键累加
				if(cj.v_set>5.0)cj.v_set = 0;
			break;
			default:k_err_cnt++;break;//无效按键累加
			}
		break;
		case S17:
			switch(run_state)
			{
			case 20:
				cj.v_set-=0.5;k_err_cnt = 0;
				if(cj.v_set<0.0)cj.v_set = 5.0;
			break;
			default:k_err_cnt++;break;//无效按键累加
			}
		break;
		default:k_err_cnt++;break;//无效按键累加
	}
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	static u16 _cnt = 0;
	
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	if(cj.v > cj.v_set)//持续时间大于5s
	{
		if(++_cnt >= 50)
		{
			L1 = 1;
		}
	}else 
	{
		_cnt = 0;//否则清零
		L1 = 0;
	}
	
	if(cj.cnt%2)//计数值为奇数
		L2 = 1;
	else L2 = 0;
	
	if(k_err_cnt>=3)//无效按键大于3次S12 S13 S16 S17
		L3 = 1;
	else L3 = 0;
	
	P0 = ~led_dat;
	gate(LED);	
}
//*********************
void adc_mainloop(void)
{
	u8 tmp;
	
	if(!f_100ms_adc) return;
	f_100ms_adc = 0;

	tmp = read_adc();
	
	cj.adc = (u16)tmp*50.0/255; //10倍
	cj.v = (float)cj.adc/10.0;
}
//*************************


