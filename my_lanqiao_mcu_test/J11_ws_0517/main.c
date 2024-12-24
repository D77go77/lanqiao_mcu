#include "all.h"
xdata struct caiji cj;
//led
volatile u8 bdata led_dat = 0;
sbit L1 = led_dat ^ 0;
sbit L2 = led_dat ^ 1;
sbit L3 = led_dat ^ 2;
sbit L4 = led_dat ^ 3;
sbit L5 = led_dat ^ 4;
sbit L6 = led_dat ^ 5;
sbit L7 = led_dat ^ 6;
sbit L8 = led_dat ^ 7;
//
u8 run_state;
//
//**************************
void main_init();
void key_mainloop();
void state_mainloop();
void temp_mainloop();
void rtc_mainloop();
void adc_mainloop();
void led_mainloop();
//**************************
void main()
{
	main_init();
	EA=1;
	while(1)
	{
		key_mainloop();
		state_mainloop();
		
		rtc_mainloop();
		temp_mainloop();
		adc_mainloop();
		led_mainloop();
	}
}
//**************************
void main_init()
{
	Timer1_Init();
	
	off(JDQ);off(FMQ);
	CMOD=CH=CL=0;//PCA_INIT
	
	rtc_init();
	rtc_run(0x16,0x59,0x50);
	
	init_ad(0x41);
	
	led_dat=0;
	
	run_state = 10;
	cj. hour=16;
	cj.min=59;
	cj.sec=50;//时钟
	cj. t=0;//温度
	cj. h_set=17;//小时
	cj. h_tmp=17;
	cj. t_set=25;//温度
	cj. t_tmp=25;
	cj. led_set=4;//led
	cj. led_tmp=4;
	cj. f_dark=1;//亮暗
	cj. dac=0;//dac输出
	
	init_temp();
}
//**************************
void state_mainloop()
{
	if(!f_100ms_state)return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10: sprintf(dis_str,"%02bu-%02bu-%02bu",cj.hour,cj.min,cj.sec);
			break;
		case 11: sprintf(dis_str,"C    %4.1f",cj.t);
			break;
		case 12: sprintf(dis_str,"E %4.2f  %bu",cj.dac,cj.f_dark);
			break;
		case 20: sprintf(dis_str,"P1    %02bu",cj.h_tmp);//时间参数
			break;
		case 21: sprintf(dis_str,"P2    %02bu",cj.t_tmp);//温度参数
			break;
		case 22: sprintf(dis_str,"P3     %bu",cj.led_tmp);//led参数
			break;
	}
	disp_proc();	
}
//**************************
void key_mainloop()
{	
	if(!f_20ms_key)return ;
	f_20ms_key = 0;
	
	key_proc();
	
	if(!key_code)return;
	
	switch(key_code)
	{
		//
		case 4:
			switch(run_state)
			{
				case 10:
				case 11:
				case 12:run_state=20;
					break;
				case 20:
				case 21:
				case 22:
					cj.h_set=cj.h_tmp;
					cj.t_set=cj.t_tmp;
					cj.led_set=cj.led_tmp;//退出设置生效
					run_state=10;
					break;
			}
			break;
		//
		case 5:
			switch(run_state)
			{
				case 10:run_state=11;break;
				case 11:run_state=12;break;
				case 12:run_state=10;break;
				//
				case 20:run_state=21;break;
				case 21:run_state=22;break;
				case 22:run_state=20;break;
			}
			break;
		//
		case 8:
			switch(run_state)
			{
				case 20:if(++cj.h_tmp>=23)cj.h_tmp=23;break;
				case 21:if(++cj.t_tmp>=99)cj.t_tmp=99;break;
				case 22:if(++cj.led_tmp>=8)cj.led_tmp=8;break;
			}
			break;
		//
		case 9:			
			switch(run_state)
			{
				case 20:if(--cj.h_tmp>=23)cj.h_tmp=0;break;
				case 21:if(--cj.t_tmp>=99)cj.t_tmp=0;break;
				case 22:if(--cj.led_tmp<=4)cj.led_tmp=4;break;
			}
			break;
	}
	key_code = 0;
}
//**************************
void rtc_mainloop()
{
	u8 hour,min,sec;
	if(!f_100ms_rtc)return;
	f_100ms_rtc=0;
	
	hour=Read_Ds1302_Byte(0x85);
	min=Read_Ds1302_Byte(0x83);
	sec=Read_Ds1302_Byte(0x81);
	if(cj.sec==sec)return;
	cj.hour=10*(hour>>4)+(hour&0x0f);
	cj.min=10*(min>>4)+(min&0x0f);
	cj.sec=10*(sec>>4)+(sec&0x0f);
}
//**************************
#define LIGHT 50
#define DARK 15

u8 cnt_3=0;//led3秒计算
//**************************
void adc_mainloop()
{
	u8 adc_temp = 0;//adc
	
	if(!f_100ms_adc)return;
	f_100ms_adc=0;
	
	read_ad();
	adc_temp=read_ad();
	cj.adc=100 *adc_temp /255.0 ;
	cj.dac = cj.adc/20.0;
	
	
	if(!cj.f_dark && cj.adc<DARK)//亮转暗
	{
		cj.f_dark=1;cnt_3=0;//清除计数
	}
	if(cj.f_dark && cj.adc>LIGHT)
	{
		cj.f_dark=0;cnt_3=0;
	}
}

//**************************
void temp_mainloop()
{
	if(!f_600ms_temp)return;
	f_600ms_temp = 0;
	
	cj.t =  read_tem() ;
	if(cj.t>=99)cj.t=99;
	else if(cj.t<=0)cj.t=0;
	
	init_temp();
}
//**************************
void led_mainloop(void)
{	
	if (!f_100ms_led) return;
	f_100ms_led = 0;
	
	if(cj.h_set<8)
	{
		if(cj.h_set < cj.hour && cj.hour < 8)
			L1=1;
		else
			L1=0;
	}else
	{
		if(8 < cj.hour && cj.hour < cj.h_set)
			L1=0;
		else
			L1=1;
	}
	
	//
	if(cj.t < cj.t_set)
		L2=1;
	else
		L2=0;
	//
	if(cj.f_dark)
	{
		if(++cnt_3>=30)
		{
			cnt_3=0;L3=1;
		}
	}else
	{
		if(++cnt_3>=30)
		{
			cnt_3=0;L3=0;
		}
	}
	//
	if(cj.f_dark)
	{
		switch(cj.led_set)
		{
			case 4:L4=1;break;
			case 5:L5=1;break;
			case 6:L6=1;break;
			case 7:L7=1;break;
			case 8:L8=1;break;
		}
	}
	else
		L4=L5=L6=L7=L8=0;
}
//**************************









	