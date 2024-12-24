#include "time.h"
bit f_2ms_led = 0;
bit f_1s=0;
bit f_20ms_key = 0;
bit f_100ms_adc = 0;
bit f_100ms_state = 0;
bit f_100ms_led=0;
bit f_100ms_rtc = 0;
bit f_200ms_uart = 0;
bit f_800ms_blink8 = 0;
void timflag_proc(void);
void led_proc(void);
//******************************************
void PIT1(void) interrupt 3
{
	disp_scan();	
	timflag_proc();	
	led_proc();
}
//
void Timer1_Init(void)		//2����@12.000MHz
{
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x40;				//���ö�ʱ��ʼֵ
	TH1 = 0xA2;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1 = 1;				//ʹ�ܶ�ʱ��1�ж�
}
//
void timflag_proc(void)	
{
	static u8 cnt_20ms = 0;
	static u8 cnt_100ms = 0;
	static u8 cnt_200ms = 0;	
	static u16 cnt_800ms = 0;
	
	if (++cnt_20ms >= 10)
	{
		cnt_20ms = 0;
		f_20ms_key = 1;
	}
	if (++cnt_100ms >= 50)
	{
		cnt_100ms = 0;
		f_100ms_adc = 1;f_100ms_state = 1;f_100ms_led=1;f_100ms_rtc=1;
	}
	if (++cnt_200ms >= 100)
	{
		cnt_200ms = 0;
		f_200ms_uart = 1;
	}
	if (++cnt_800ms >= 400)
	{
		cnt_800ms = 0;
		f_800ms_blink8 = 1;
	}
}

void led_proc(void)
{
	static u8 t=0;
	
	if(++t==4) t=0;
	switch(x.light)
	{
		case 1:
			if(t==0) P0=~led_dat;
			else P0=0XFF;
		break;
		
		case 2:
			if(t==0 || t==1) P0=~led_dat;
			else P0=0XFF;
		break;
		
		case 3:
			if(t==3) P0=0xff;
			else P0=~led_dat;
		break;
		
		case 4:
			P0=~led_dat;
		break;
	}
	gate(LED);
}


