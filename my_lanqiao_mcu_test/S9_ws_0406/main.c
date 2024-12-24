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
	u8 i;//tmp
	
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	init_adc(0x03);//RB2

	//eeprom
	for(i = 1;i<5;i++)
	{
		x.t[i] = read_e2prom(i);
		if(x.t[i] < 4||x.t[i] > 12)
		{
			x.t[i]=4;
			write_e2prom(i,4);//初始化间隔
			Delay10ms();
		}
	}
	
	run_state = 10;
	x.light = 0;
	x.f_on = 0;//不允许开关灯
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
			sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t[x.m_set]*100);
		break;
		case 21://设置界面//流转间隔
			sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t_set*100);
		break;
		case 30://亮度显示界面
			sprintf(dis_str,"      -%1bu",x.light);
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
		case S4://--
			switch(run_state)
			{
				case 20:
				if(--x.m_set <= 1)x.m_set = 1;	
				break;
				case 21:
				if(--x.t_set <= 4)x.t_set = 4;	//400ms
				break;
				case 10:run_state=30;break;//按下显示亮度
			}
		break;
		case S5://++
			switch(run_state)
			{
				case 20://模式
				if(++x.m_set >= 4)x.m_set = 4;
				break;
				case 21://时间
				if(++x.t_set >= 12)x.t_set = 12;	//1200ms
				break;
			}
		break;
		case S6:
			switch(run_state)
			{
				case 10:run_state = 20;blink8_dat = 0x07;//数码管闪烁//进入修改模式
						x.m_set=1;//将模式设置初始设置为1
				break;
				//
				case 20:run_state = 21;blink8_dat = 0xf0;//数码管闪烁//进入修改时间
						x.t_set=x.t[x.m_set];//将模式对应的真实时间赋给显示上的设置时间
				break;
				//
				case 21:run_state = 10;blink8_dat = 0;//数码管不闪烁//保存返回
						write_e2prom(x.m_set,x.t_set);//写入2402时间间隔保存
						x.t[x.m_set]=x.t_set;//将时间设置值保存给真实时间//修改流转时间
				break;
			}
		break;
		case S7:
			x.f_on = ~x.f_on;//启停led
		break;
		
		case S4OFF://松开返回初始界面
			switch(run_state)
			{
				case 30:run_state=10; break;//松开返回显示
			}
		break;
	}
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	static int t = 0;
	static u8 i = 0;
	
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	if(!x.f_on)return;//S7启停,即i不在加减
	if(++t <= x.t[x.mode])return;//流转间隔判断
	t = 0;
	
	switch(x.mode)
	{
		case 1:
			led_dat = led1[i];if(++i > 7) {i=7;x.mode=2;}
		break;
		case 2:
			led_dat = led1[i];if(--i > 7) {i=4;x.mode=3;}
		break;
		case 3:
			led_dat = led2[i];if(--i > 3) {i=0;x.mode=4;}
		break;
		case 4:
			led_dat = led2[i];if(++i > 3) {i=0;x.mode=1;}
		break;
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
	if(!f_100ms_adc) return;
	f_100ms_adc = 0;
	
	x.adc  = read_adc();
	x.light = (4 * (u16)x.adc / 256)+1;
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
