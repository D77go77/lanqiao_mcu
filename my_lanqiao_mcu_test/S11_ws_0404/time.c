#include "time.h"
bit f_1s=0;
bit f_20ms_key = 0;
bit f_100ms_state = 0;
bit f_100ms_adc = 0;
bit f_100ms_led=0;
void timflag_proc(void);
void led_proc(void);
//******************************************
void PIT1(void) interrupt 3
{
	disp_scan();	
	timflag_proc();	
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
	static u16 cnt_1s = 0;
	
	if (++cnt_20ms >= 10)
	{
		cnt_20ms = 0;
		f_20ms_key = 1;
	}
	if (++cnt_100ms >= 50)
	{
		cnt_100ms = 0;
		f_100ms_state = 1;f_100ms_led=1;f_100ms_adc = 1;
	}
	if(++cnt_1s >= 500)
	{
		cnt_1s = 0;
		f_1s = 1;
	}
}

