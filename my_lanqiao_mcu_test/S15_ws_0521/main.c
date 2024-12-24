#include "all.h"
xdata struct caiji cj;
//
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
u8 run_state=10;
bit f_cj=0; //0-未采集 1-采集
//
void main_init();
void state_mainloop();
void key_mainloop();
void rtc_mainloop();
void led_mainloop();
void freq_mainloop();
void dac_out();
//******************
void main()
{
	main_init();
	EA=1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		rtc_mainloop();
		freq_mainloop();
		led_mainloop();
	}
}
//*******************
void main_init()
{
	off(JDQ); off(FMQ);
	ne555_Init();
	Timer1_Init();
	
	cj.f=-100;
	cj.f_lim=2000;
	cj.f_jiao=0;
	cj.f_max=0;
	cj.h_save=0;
	cj.m_save=0;
	cj.s_save=0;
	cj.h=20;
	cj.m=20;
	cj.s=20;
	
	rtc_init();
	rtc_run(0x00,0x00,0x00);
}
//*******************
void state_mainloop()
{
	if(!f_100ms_state) return;
	f_100ms_state=0;
	
	switch(run_state)
	{
		case 10: //数据显示界面 频率
			if(cj.f>0) sprintf(dis_str,"F  %5ld",cj.f);
			else sprintf(dis_str,"F     LL");
		break;
		
		case 20: //参数界面 超限参数
			sprintf(dis_str,"P1  %4u",cj.f_lim_tmp);
		break;
		
		case 21: //参数界面 校准值
			sprintf(dis_str,"P2  %4d",cj.f_jiao_tmp);
		break;
		
		case 30: //时间界面
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.h,cj.m,cj.s);
		break;
		
		case 40: //回显界面 最大频率
			if(f_cj) sprintf(dis_str,"HF %5u",cj.f_max);
			else sprintf(dis_str,"HF      ");
		break;
		
		case 41: //回显界面 时间
			if(f_cj) sprintf(dis_str,"HA%02bu%02bu%02bu",cj.h_save,cj.m_save,cj.s_save);
			else sprintf(dis_str,"HA      ");
		break;
	}
	disp_proc();
}
//*******************
void key_mainloop()
{
	if(!f_20ms_key) return;
	f_20ms_key=0;
	key_proc();
	
	if(!key_code) return;
	
	switch(key_code)
	{
		case 4:
			switch(run_state)
			{
				case 10: 
					run_state=20; 
					cj.f_lim_tmp=cj.f_lim; cj.f_jiao_tmp=cj.f_jiao;
				break;
				
				case 20:
				case 21: 
					run_state=30; 
					cj.f_lim=cj.f_lim_tmp; cj.f_jiao=cj.f_jiao_tmp;
				break;
				
				case 30: run_state=40; break;
					
				case 40:
				case 41: run_state=10; break;
			}
		break;
		
		case 5:
			switch(run_state)
			{
				case 20: run_state=21; break;
				case 21: run_state=20; break;
				case 40: run_state=41; break;
				case 41: run_state=40; break;
			}
		break;
		
		case 8:
			switch(run_state)
			{
				case 20: cj.f_lim_tmp+=1000; if(cj.f_lim_tmp>9000) cj.f_lim_tmp=9000; break;
				case 21: cj.f_jiao_tmp+=100; if(cj.f_jiao_tmp>900) cj.f_jiao_tmp=900; break;
			}
		break;
		
		case 9:
			switch(run_state)
			{
				case 20: cj.f_lim_tmp-=1000; if(cj.f_lim_tmp<1000) cj.f_lim_tmp=1000; break;
				case 21: cj.f_jiao_tmp-=100; if(cj.f_jiao_tmp<-900) cj.f_jiao_tmp=-900; break;
			}
		break;	
	}
	
	key_code=0;
}
//*******************
void rtc_mainloop()
{
	u8 h,m,s;
	static u8 old_s;
	
	if(!f_100ms_rtc) return;
	f_100ms_rtc=0;
	
	h=Read_Ds1302_Byte(0x85);
	m=Read_Ds1302_Byte(0x83);
	s=Read_Ds1302_Byte(0x81);

	if(old_s==s) return;
	old_s=s;
	
	cj.h=((h>>4)*10)+(h&0x0f);
	cj.m=((m>>4)*10)+(m&0x0f);
	cj.s=((s>>4)*10)+(s&0x0f);
}
//*******************
void freq_mainloop()
{
	if(!f_100ms_freq) return;
	f_100ms_freq=0;
	
	cj.f=(long int)freq+cj.f_jiao;
	
	if(cj.f>=0) dac_out();
	else write_dac(0);
	
	if(cj.f>cj.f_max)
	{
		f_cj=1;
		cj.f_max=cj.f;
		cj.h_save=cj.h_save;
		cj.m_save=cj.m_save;
		cj.s_save=cj.s_save;
	}
}
//*******************
void dac_out()
{
	if(cj.f<=500) cj.da_v=1;
	else if(cj.f>=cj.f_lim) cj.da_v=5;
	else cj.da_v=4.0/(cj.f_lim-500.0)+1.0;
	
	cj.da_dat=cj.da_v/5.0*255;
	write_dac(cj.da_dat);
}
//*******************
void led_mainloop()
{
	if(!f_100ms_led)return;
	f_100ms_led=0;
	led_dat=0;
	
	switch(run_state)
	{
		case 10: L1=!L1; break;
	}
	
	if(cj.f>cj.f_lim) L2=!L2;
	
	if(cj.f<0) L2=1;
	
}
//*******************

//*******************

//*******************




