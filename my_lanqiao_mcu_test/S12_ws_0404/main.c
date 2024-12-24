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
	cj. t=0;//�¶�
	cj. t_set=25;//�¶Ȳ���
	cj.t_tmp = cj.t_set;
	cj. v=0;//DAC���
	cj. mode=0;//ģʽ//0Ϊ1 ffΪ2
	
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
			sprintf(dis_str,"C   %5.2f",cj.t);
		break;
		case 20://��������
			sprintf(dis_str,"P     %2bu",cj.t_tmp);
		break;
		case 30://dac�������
			sprintf(dis_str,"A   %5.2f",cj.v);
		break;
	}
	disp_tran();
	dac_proc();//dac�������
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
		case S4: //�л�����
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
				case 20:if(++cj.t_tmp>=99)cj.t_tmp=99;break;//�ۼ�
			}
		break;
		case S9: 
			switch(run_state)
			{
				case 20:if(--cj.t_tmp>=99)cj.t_tmp=0;break;//�ۼ�
			}
		break;
		case S5:
			cj.mode =~cj.mode;
		break;
	}
	key_code = 0;
}
//*************************
void dac_proc(void)//dac���ģʽ���� 
{
	switch(cj.mode)//dac���ģʽ
	{
		case 0:
			if(cj.t<cj.t_set)//ģʽ1;ʵʱ�¶�С�ڲ��������0v
			{write_dac(0);cj.v = 0;}
			else
			{write_dac(255);cj.v = 5.0;}
		break;
		case 0xff:
			if(cj.t<=20)
			{write_dac(255/5);cj.v = 1.0;}//���1v
			else if(cj.t>=40)
			{write_dac(4*255/5);cj.v = 4.0;}//���4v
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
	if(!cj.mode)//ģʽ1������mode=0;
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

