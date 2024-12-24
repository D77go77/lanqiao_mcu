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

	rtc_run(0x15,0x33,0x45);
	
	run_state = 10;
	cj. t=0;//�¶�
	cj. t_set=23;//�¶Ȳ���
	cj.mode = 0;//0Ϊ�¶�ģʽ��ffΪʱ��ģʽ
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
		case 10://�¶Ƚ���
			sprintf(dis_str,"U1   %4.1f",cj.t);
		break;
		case 20://ʱ�����//hm
			sprintf(dis_str,"U2 %02bu-%02bu",cj.h,cj.m);
		break;
		case 21://ms
			sprintf(dis_str,"U2 %02bu-%02bu",cj.m,cj.s);
		break;
		case 30://��������
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
		case S12: //�л�����
			switch(run_state)
			{
				case 10:run_state=20;break;
				case 20:run_state=30;break;
				case 30:run_state=10;
				break;
			}
		break;
		//
		case S13://�л�ģʽ//0Ϊ�¶ȿ��ƣ�ffΪʱ�����
			cj.mode = ~cj.mode;
			off(JDQ);f_jdq = 0;//�л�ģʽ�����jdq�����
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
				case 20:run_state = 21;break;//���½���ms
			}
		break;
		//
		case S17OFF:
			switch(run_state)//�ɿ�����hm
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
		L1=1;//����L1��
	if(L1)
	{	
		if(++t>=50)
		{
			t=0;L1=0;
		}			
	}//L1����5�����
	
	mode_proc();//ģʽ�Լ��̵�������
	
	P0 = ~led_dat;
	gate(LED);	
}
//*************************
void mode_proc(void)//ģʽ�Լ��̵�������
{
	static u8 t = 0;//����5s
	switch(cj.mode)
	{
		case 0://�¶ȿ���
			L2=1;
			if(cj.t > cj.t_set)//�ɼ�ֵ>����ֵ//����
			{
				on(JDQ);f_jdq = 1;
			}else 
			{	
				off(JDQ);f_jdq=0;
			}
		break;
		case 0xff://ʱ�����
			L2=0;
			if(cj.m == 0&&cj.s == 0)//���㱨��
			{
				on(JDQ);f_jdq = 1;
			}
			if(f_jdq)
			{
				if(++t >= 50)//5s���jdq
				{
					t = 0;off(JDQ);f_jdq=0;
				}
			}
		break;
	}
	if(f_jdq)//jdq����˸
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

