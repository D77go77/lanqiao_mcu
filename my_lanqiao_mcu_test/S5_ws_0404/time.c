#include <time.h>

bit f_20ms_key =0;
bit f_100ms_state =0;
bit f_100ms_temp =0;
bit f_100ms_led =0;


void time_mainloop();


void Timer1_Init(void)		//2����@12.000MHz
{
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x40;				//���ö�ʱ��ʼֵ
	TH1 = 0xA2;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1 =1;
}

void T1_isr() interrupt 3
{
	time_mainloop();
	disp_scan();

}

void time_mainloop()
{
	static cnt_20ms =0;
	static cnt_100ms =0;

	
	if(++cnt_20ms >=10)
	{
		cnt_20ms =0;
		f_20ms_key =1;
	}
	
	if(++cnt_100ms >=50)
	{
		cnt_100ms =0;
		f_100ms_state =1;
		f_100ms_temp =1;		
		f_100ms_led =1;
	}
	
	
}

