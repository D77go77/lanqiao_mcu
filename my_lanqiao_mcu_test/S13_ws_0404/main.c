/*
1����ÿ��������򶼵���  ����ʱ�������ϴ�����
2�����̰�����ģ��ֻ���Ǿ���ʱ��д������ģ��
3����ĿҪ������ϸ������ע��ÿһ��ϸ��Ҫ��
4�������ú������޸ģ��ϸ�Ҫ��
5���ο���ֻ����˼·��ʵ��Ҫ��ȫ���Լ���
6����Ҫ����Ҫ��ע��
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
bit f_jdq = 0;//�̵���������־
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
	ds1302_init();
	ds1302_run(0x23,0x59,0x50);
	
	run_state = 10;
	cj. t=0;//�¶�
	cj. t_set=23;//�¶Ȳ���
	cj. t_tmp=23;//�¶Ȳ�������ֵ
	cj.mode = 0;//0Ϊ�¶�ģʽ��ffΪʱ��ģʽ
	cj. h=0x23;
	cj. m=0x59;
	cj. s=0x50;
	
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
		case 10://�¶Ƚ���
			sprintf(dis_str,"U1   %4.1f",cj.t);
		break;
		case 20://ʱ�����
			if(key_code == 17)
				sprintf(dis_str,"U2 %02bu-%02bu",cj.m,cj.s);
			else sprintf(dis_str,"U2 %02bu-%02bu",cj.h,cj.m);
		break;
		case 30://��������
			sprintf(dis_str,"U3    %02bu",cj.t_tmp);
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
	
	switch(key_code)
	{
		case S12: //�л�����
			switch(run_state)
			{
				case 10:run_state=20;break;
				case 20:run_state=30;break;
				case 30:run_state=10;cj.t_set=cj.t_tmp;break;//��������ֵ
			}
		break;
		case S13://�л�ģʽ
			cj.mode = ~cj.mode;
			off(JDQ);f_jdq = 0;//�л�ģʽ�����dqj�����
		break;
		case S16: 
			switch(run_state)
			{
				case 30:if(++cj.t_tmp > 99)cj.t_tmp = 10;break;
			}
		break;
		case S17:
			switch(run_state)
			{
				case 30:if(--cj.t_tmp < 10)cj.t_tmp = 99;break;
			}
		break;
	}
	if(key_code==S17 && run_state==20)return;//�����˳�
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	static _cnt5s = 0;
	if(!f_100ms_led)return;
	f_100ms_led = 0;

	if(!cj.mode) L2=1;//�¶ȿ���ģʽ��L2��
	else L2=0;
	
	if(cj.m == 0&&cj.s == 0) L1=1;//����L1��
	if(L1)
	{if(++_cnt5s>=50) L1=0;}//5�����
	
	mode_proc();
	
	P0 = ~led_dat;
	gate(LED);	
}
//*************************
void mode_proc(void)//ģʽ�Լ��̵�������
{
	static u8 _cnt = 0;//����5s
	switch(cj.mode)
	{
		case 0://�¶ȿ���
			if(cj.t > cj.t_set)
			{
				on(JDQ);f_jdq = 1;
			}else 
			{off(JDQ);f_jdq=0;}
		break;
		case 0xff://ʱ�����
			if(cj.m == 0&&cj.s == 0)
			{
				on(JDQ);f_jdq = 1;
			}
			if(f_jdq)
			{
				if(++_cnt >= 50)
				{
					_cnt = 0;off(JDQ);f_jdq=0;
				}
			}
		break;
	}
	if(f_jdq)
		L3 = ~L3;
	else L3=0;
}
//**************************8
void temper_mainloop(void)
{
	u16 tmp;
	if(!f_800ms_temp)return;
	f_800ms_temp = 0;

	tmp=read_tem16();
	cj.t = (float)tmp*0.0625f;
	start_ds18b20();
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
	if(s_old==s)return;//��û�и��£�˵��û��һ�룬ֱ���ˡ�
	s_old=s;
	
	cj.h = (h>>4)*10 + (h&0x0f);
	cj.m = (m>>4)*10 + (m&0x0f);
	cj.s = (s>>4)*10 + (s&0x0f);
}
//****************************
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

