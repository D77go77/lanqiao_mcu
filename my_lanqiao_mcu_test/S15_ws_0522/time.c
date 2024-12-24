#include "time.h"

//TIM
bit f_20ms_key = 0;
bit f_100ms_led = 0;
bit f_100ms_state = 0;
bit f_100ms_adc = 0;
bit f_100ms_wave = 0;
bit f_100ms_rtc = 0;
bit f_100ms_freq=0;
//
u16 freq=0;
//
void led_proc();
void tim_proc();
//************************
void ne555_Init(void)		
{
//	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x05;
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x00;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
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
void Timer1_Isr(void) interrupt 3//2ms
{
	tim_proc();
	disp_scan();	
}
//***********************************
void tim_proc()
{
	static u8 cnt_20ms=0;
	static u8 cnt_100ms=0;
	static u16 cnt_1s=0;

	if(++cnt_20ms>=10)
	{
		cnt_20ms=0;f_20ms_key=1;
	}
	if(++cnt_100ms>=50)
	{
		cnt_100ms=0;f_100ms_state=1;f_100ms_wave=1;f_100ms_led=~f_100ms_led;f_100ms_rtc=1;f_100ms_adc=1;f_100ms_freq=1;
		led_proc();
	}
	if(++cnt_1s>=500)
	{
		cnt_1s=0;
		freq=(u16)TH0<<8|TL0;
		TH0=TL0=0;
	}
}
//*****************************

//******************



