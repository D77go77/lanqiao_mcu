#include "time.h"
bit f_20ms_key=0;
bit f_100ms_state=0;
bit f_50ms_led=0;
bit f_100ms_freq=0;
bit f_100ms_adc=0;
bit f_800ms_tem=0;

u16 ping_dat=0;

void tim_proc();
void led_proc();
void ne555_proc();
//*****************************
void ne555_init(void)	// ��ʱ��0
{
    TMOD |= 0x05;       
	
	TL0 = 0;				//���ö�ʱ��ʼֵ
	TH0 = 0;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
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
//**********************************
void Timer1_Isr(void) interrupt 3//2ms
{
	tim_proc();
	disp_scan();	
	ne555_proc();
}
//

//*****************************
void tim_proc()
{
	static u8 cnt_20ms=0;
	static u8 cnt_50ms=0;
	static u8 cnt_100ms=0;
	static u16 cnt_800ms=0;

	if(++cnt_20ms>=10)
	{
		cnt_20ms=0;f_20ms_key=1;
	}
	if(++cnt_50ms>=25)
	{
		cnt_50ms=0;f_50ms_led=1;
	}
	if(++cnt_100ms>=50)
	{
		cnt_100ms=0;f_100ms_state=1;f_100ms_freq=1;f_100ms_adc=1;
		led_proc();
	}

	if(++cnt_800ms>=400)
	{
		cnt_800ms=0;f_800ms_tem=1;
	}
}
//*****************************
void led_proc()
{
	P0=~led_dat;
	gate(LED);
}
//*********************
void ne555_proc()
{
	static u16 cnt_1000ms = 0;

	if(++cnt_1000ms >= 500)
	{
		cnt_1000ms = 0;			
		TR0 = 0;
		ping_dat = (u16)TH0 * 256 +TL0;//��ȡƵ��
		TR0 = 1;
		TH0 = 0;TL0 = 0;
	}
}








