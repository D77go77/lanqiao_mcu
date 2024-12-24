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
u8 bdata blink8_dat=0;
//
xdata struct _cj x;
bit f_s4L = 0;
//
//
u8 led1[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
u8 led2[4] = {0x18,0x24,0x42,0x81};
u8 run_state = 0;
bit f_led_t = 0;
//
void main_init(void);
void state_mainloop(void);
void key_mainloop(void);
//
void led_mainloop(void);
//
void adc_mainloop(void);
void blink8_mainloop(void);

void Delay10ms(void);
void e2prom_init(void);
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
		blink8_mainloop();
		adc_mainloop();
	}
}
//****************************
void main_init(void)
{
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	init_adc(0x03);//RB2

	e2prom_init();
	run_state = 10;
	x.l = 0;
	x.f_on = 0xff;//允许开关灯
	x.adc = 0;
	x.mode = 1;
	x.m_set = 1;

}
//****************************
void state_mainloop(void)
{
	if(!f_100ms_state)return;
	f_100ms_state = 0;
	switch(run_state)
	{
		case 10://显示界面
			sprintf(dis_str,"        ");
		break;
		case 20://设置界面//模式
			switch(x.m_set)
			{
				case 1:sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t1*100);break;
				case 2:sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t2*100);break;
				case 3:sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t3*100);break;
				case 4:sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t4*100);break;
			}
		break;
		case 21://设置界面//流转间隔
			sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t_set*100);
		break;
		case 30:
			sprintf(dis_str,"      -%1bu",x.l);
		break;
	}
	disp_tran();
}
//******************************
void key_mainloop(void)
{
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(!key_code)return;
	
	switch(key_code)
	{
		case S4:
			switch(run_state)
			{
				case 20://模式
				if(x.m_set++ >= 4)x.m_set = 4;
				break;
				case 21://时间
				if(x.t_set++ >= 12)x.t_set = 12;	//1200ms
				break;
			}
		break;
		case S5:
			switch(run_state)
			{
				case 20:
				if(x.m_set-- <= 1)x.m_set= 1;	
				break;
				case 21:
				if(x.t_set-- <= 4)x.t_set = 4;	//400ms
				break;
			}
		break;
		case S6:
			switch(run_state)
			{
				case 10:run_state = 20;blink8_dat = 0x07;
				break;
				case 20:run_state = 21;blink8_dat = 0xf0;
						switch(x.m_set)
						{
							case 1:x.t_set=x.t1;break;
							case 2:x.t_set=x.t2;break;
							case 3:x.t_set=x.t3;break;
							case 4:x.t_set=x.t4;break;
						}
				break;
				case 21:run_state = 10;
						blink8_dat = 0;
						switch(x.m_set)
						{
							case 1:write_e2prom(x.m_set,x.t1);x.t1=x.t_set;break;//写入时间间隔保存
							case 2:write_e2prom(x.m_set,x.t2);x.t2=x.t_set;break;//写入时间间隔保存
							case 3:write_e2prom(x.m_set,x.t3);x.t3=x.t_set;break;//写入时间间隔保存
							case 4:write_e2prom(x.m_set,x.t4);x.t4=x.t_set;break;//写入时间间隔保存
						}
				break;//保存返回
			}
		break;
		case S7:
			x.f_on = ~x.f_on;
		break;
		
		case S4L:
			switch(run_state)
			{
				case 10:run_state=40; break;
			}
		break;
	}
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	static int cnt_t = 0;
	static u8 i = 0;
	
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	switch(x.mode)
	{
		case 1:
			if(++cnt_t >= x.t1)//流转间隔判断
			{
				cnt_t = 0;f_led_t = 1;
			}else f_led_t = 0;
			led_dat = led1[i];
		break;
		case 2:
			if(++cnt_t >= x.t2)//流转间隔判断
			{
				cnt_t = 0;f_led_t = 1;
			}else f_led_t = 0;
			led_dat = led1[i];
		break;
		case 3:
			if(++cnt_t >= x.t3)//流转间隔判断
			{
				cnt_t = 0;f_led_t = 1;
			}else f_led_t = 0;
			led_dat = led2[i];
		break;
		case 4:
			if(++cnt_t >= x.t4)//流转间隔判断
			{
				cnt_t = 0;f_led_t = 1;
			}else f_led_t = 0;
			led_dat = led2[i];
		break;
		default:led_dat = 0;break;
	}
	
	if(!x.f_on)return;//S7启停,即i不在加减
	if(f_led_t)//流转间隔
	{
		f_led_t = 0;
		switch(x.mode)
		{
			case 1:			
				if(++i > 7) 
				{i=7;x.mode=2;}
			break;
			case 2:
				if(--i > 7) 
				{i=4;x.mode=3;}
			break;
			case 3:
				if(--i > 3) 
				{i=0;x.mode=4;}
			break;
			case 4:
				if(++i > 3) 
				{i=0;x.mode=1;}
			break;
		}
	}
}
//**********************************
void blink8_mainloop(void)
{
	u8 i=0,tmp = 0;
	
	if (!f_800ms_blink8) return;//500ms
	f_800ms_blink8 = 0;
	
	if(blink8_dat == 0)return;
	
	for(i = 0;i <8;i++)
	{
		tmp  = blink8_dat & (1 << i);
		if(tmp)
			dis_blink[i] = ~dis_blink[i];
		else
			dis_blink[i] = 0;
	}
}
//*********************************************
void adc_mainloop(void)//采集亮度
{
	u8 ad_temp = 0;
	if(!f_100ms_adc) return;
	f_100ms_adc = 0;
	
	ad_temp = read_adc();
	x.adc = (4 * (u16)ad_temp / 256)+1;
	x.l = x.adc;
}
//***************************
void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}
//***********************
void e2prom_init(void)
{
	//eeprom
	x.t1 = read_e2prom(0x01);
	if(x.t1 > 13 || x.t1 < 3)
	{
		write_e2prom(0x01,4);//初始化间隔
		Delay10ms();
	}
	x.t1 = read_e2prom(0x01);
	//
	x.t2 = read_e2prom(0x02);
	if(x.t2 > 13 || x.t2 < 3)
	{
		write_e2prom(0x02,4);//初始化间隔
		Delay10ms();
	}
	x.t2 = read_e2prom(0x02);
	//
	x.t3 = read_e2prom(0x03);
	if(x.t3 > 13 || x.t3 < 3)
	{
		write_e2prom(0x03,4);//初始化间隔
		Delay10ms();
	}
	x.t3 = read_e2prom(0x03);
	//
	x.t4 = read_e2prom(0x04);
	if(x.t4 > 13 || x.t4 < 3)
	{
		write_e2prom(0x04,4);//初始化间隔
		Delay10ms();
	}
	x.t4 = read_e2prom(0x04);
}