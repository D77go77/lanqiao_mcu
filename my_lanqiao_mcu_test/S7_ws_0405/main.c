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
		led_mainloop();
		temper_mainloop();
	}
}
//****************************
void main_init(void)
{
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	Timer2Init();

	
	run_state = 10;
	
	cj. t=0;//�¶�
	cj. mode=1;;//ģʽ
	cj. tim=0;//��ʱʱ��
	
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
		case 10://���Ƚ���
			sprintf(dis_str,"-%1bu- %04bu",cj.mode,cj.tim);
		break;
		case 20://���½���
			sprintf(dis_str,"-4-  %02buC",cj.t);
		break;
	}
	disp_tran();
}
//************************
void key_mainloop(void)
{
	static u8 _i = 0;//������ʱ�ۼ�
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(!key_code)return;
	f_led = 0;//����������ر���˸
	switch(key_code)
	{
		case S4:
			switch(cj.mode)//�л�ģʽ
			{
				case 1:cj.mode=2;break;
				case 2:cj.mode=3;break;
				case 3:cj.mode=1;break;
			}
		break;
		case S5:
			_i++;
			switch(_i)//��ʱ
			{
				case 1:cj.tim=60;break;
				case 2:cj.tim=120;break;
				case 3:cj.tim=0;_i=0;break;
			}
		break;
		case S7: 
			switch(run_state)//�л�����
			{
				case 10:run_state=20;break;
				case 20:run_state=10;break;
			}
		break;
		case S6: //���ʱ��
			cj.tim = 0; _i=0;
		break;
	}
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	led_dat = 0;
	switch(cj.mode)
	{
		case 1:L1 = 1;break;
		case 2:L2 = 1;break;
		case 3:L3 = 1;break;
	}
	
	if(!cj.tim) led_dat=0;
	
	P0 = ~led_dat;
	gate(LED);	
}
//************************************
void temper_mainloop(void)
{
	u16 tmp;
	if(!f_800ms_temp)return;
	f_800ms_temp = 0;

	tmp=read_tem16();
	cj. t = (float)tmp*0.0625f;
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
