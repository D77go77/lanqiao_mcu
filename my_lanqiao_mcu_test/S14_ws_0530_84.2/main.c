#include "all.h"
//led
volatile u8 bdata led_dat=0;
sbit L1=led_dat^0;
sbit L2=led_dat^1;
sbit L3=led_dat^2;
sbit L4=led_dat^3;
sbit L5=led_dat^4;
sbit L6=led_dat^5;
sbit L7=led_dat^6;
sbit L8=led_dat^7;

struct _now
{
	u8 h;
	u8 m;
	u8 s;
	//
	u16 f;//频率-->湿度
	u8 rh;//湿度
	u8 t;//温度
	u8 adc; //光敏电阻电压
}now;

struct _cj
{
	u8 h;
	u8 m;
	
	u8 t;
	u8 t_set;
	u8 t_max;
	float t_avr;
	
	u8 rh;
	u8 rh_max;
	float rh_avr;
	
	u8 cnt;//采集次数
}cj;


u8 run_state=11;
u8 run_state_save=11;

bit f_dark=1;  //1暗  0亮
bit f_wuxiao=0;
bit f_L6=0;

//******************************************************
void main_init(void);
//
void key_mainloop(void);
void led_mainloop(void);
void state_mainloop(void);
void temper_mainloop(void);
void rtc_mainloop(void);
void adc_mainloop(void);
void freq_mainloop(void);
void uart_mainloop(void);
void capture(void);
//******************************************************
void main(void)
{
	main_init();
	EA = 1;
	//
	while (1)
	{
		key_mainloop();
		state_mainloop();
		rtc_mainloop();		
		temper_mainloop();
		adc_mainloop();
		freq_mainloop();		
	}
}
//******************************************************
void led_mainloop(void)
{
	led_dat=0;
	
	switch(run_state)
	{
		case 11: L1=1; L2=0; L3=0; break;
		
		case 21:
		case 22:
		case 23: L1=0; L2=1; L3=0;break;
		
		case 41: L1=0; L2=0; L3=1;break;
	}
	if(cj.t>cj.t_set) L4=f_led;
	
	if(f_wuxiao) L5=1;
	
	if(f_L6) L6=1;
	
	P0 = ~led_dat;
	P2 = (P2 & 0x1f) | 0x80;
	P2 &= 0x1f;
}

//******************************************************
void state_mainloop(void)
{
	static u8 cnt_3=0;
	
	if(!f_100ms_state)return;
	f_100ms_state=0;

	switch(run_state)
	{
		case 11:   //时间界面
			sprintf(dis_str,"%02bu-%02bu-%02bu",now.h,now.m,now.s); break;
		
		case 21: //回显界面  温度
			if(!cj.cnt) sprintf(dis_str,"C       ");
			else sprintf(dis_str,"C %02bu-%4.1f",cj.t_max,cj.t_avr); break;
		case 22: //回显界面  湿度
			if(!cj.cnt) sprintf(dis_str,"H       ");
			else sprintf(dis_str,"H %02bu-%4.1f",cj.rh_max,cj.rh_avr); break;
		case 23: //回显界面  时间
			if(!cj.cnt) sprintf(dis_str,"F%02bu     ",cj.cnt,cj.h,cj.m);
			else sprintf(dis_str,"F%02bu%02bu-%02bu",cj.cnt,cj.h,cj.m); break;
		
		case 31: //参数界面
			sprintf(dis_str,"P     %02bu",cj.t_set); break;
		
		case 41: //温湿度界面
			if(++cnt_3>=30)
			{
				run_state=run_state_save;
				cnt_3=0;
			}
			if(f_wuxiao) sprintf(dis_str,"E  %02bu-AA",now.t);
			else sprintf(dis_str,"E  %02bu-%02bu",now.t,cj.rh); break;
	}
	disp_tran();

}

//******************************************************
void adc_mainloop(void)
{
	
	if (!f_100ms_adc)	return;
	f_100ms_adc = 0;
	//
	
	read_adc();
	now.adc = read_adc();
	
	if(now.adc>30) f_dark=0;//亮
	
	if(now.adc<30 && !f_dark && run_state!=41)
	{
		f_3s=0;
		
		f_dark=1;//暗
		
		run_state_save=run_state;
		run_state=41;
		
		if(!f_wuxiao) capture(); //有效采集
	}

}

//******************************************************
void rtc_mainloop(void)//获取时间
{
	u8 h,m,s;
//	static u8 s_old;
	
	if(!f_100ms_rtc) return;
	f_100ms_rtc=0;
	
	h=Read_Ds1302_Byte(0x85);
	m=Read_Ds1302_Byte(0x83);
	s=Read_Ds1302_Byte(0x81);
	
	now.h=(h>>4)*10+(h&0x0f);
	now.m=(m>>4)*10+(m&0x0f);
	now.s=(s>>4)*10+(s&0x0f);

}
//******************************************************
void freq_mainloop(void)
{	
	if(!f_1s)return;
	f_1s=0;
	
	now.f = (u16)th * 256 +tl;//读取频率

	if(now.f>=200 && now.f<=2000) //有效
	{
		now.rh=(90.0-10.0)/(2000.0-200.0)*(now.f-200.0)+10;
		f_wuxiao=0;
	}
	else 
	{
		f_wuxiao=1;
	}
}
//******************************************************
void temper_mainloop(void)
{
	u16 temp16 = 0;
	if (!f_800ms_t)		return;
	f_800ms_t = 0;
	//
	temp16 = read_tem16();
	now.t = temp16 /16.0;
	start_ds18b20();
}
//******************************************************
void main_init(void)
{
	off(JDQ); off(FMQ);
	led_dat = 0;	
	//
	start_ds18b20();//t
	init_adc(AD+DA); //光敏
	ds1302_run(0x10,0x30,0x00);
	//
	Timer0_Init();//频率计	
	Timer1_Init();	//2ms
	Uart1_Init();
	
	now.h=13;
	now.m=3;
	now.s=5;
	
	cj.t_max=0;
	cj.t_avr=0;
	
	cj.rh_max=0;
	cj.rh_avr=0;
	
	cj.cnt=0;
	now.h=0;
	cj.m=0;
	cj.t_set=30;
	cj.t=0;
	now.rh=0;
	
}
//******************************************************
void key_mainloop(void)
{
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(!key_code)return;
	
	
	switch(key_code)
	{
		case S4:
			switch(run_state)
			{
				case 11: run_state=21; break;
				
				case 21:
				case 22:
				case 23: run_state=31; break;
				
				case 31: run_state=11; break;
			}
			break;
		
		case S5:
			switch(run_state)
			{
				
				
				case 21: run_state=22; break;
				case 22: run_state=23; break;
				case 23: run_state=21; break;
				
				
			}
			break;
		
		case S8:
			switch(run_state)
			{
				case 31: if(++cj.t_set>=99) cj.t_set=99;
			}
			break;
		
		case S9:
			switch(run_state)
			{
				case 31: if(--cj.t_set>=99) cj.t_set=0;
			}
			break;
			
		case S9L:
			switch(run_state)
			{
				case 23://清除触发采集的相关变量
					cj.t_max=0;
					cj.t_avr=0;
					
					cj.rh_max=0;
					cj.rh_avr=0;
					
					cj.cnt=0;
					cj.h=0;
					cj.m=0;
					cj.t=0;
					cj.rh=0;
					
			}
	}
	
	key_code=0;
}


void capture(void)
{
	cj.cnt++;//第 cj.cnt 次采集
	
	if(now.t>cj.t && now.rh>cj.rh && cj.cnt>=2) f_L6=1; //L6点亮标志位
	else f_L6=0;
	
	cj.t=now.t;//触发采集
	cj.rh=now.rh;
	
	cj.h=now.h;//触发采集
	cj.m=now.m;
	
	cj.t_avr=(1.0*cj.t_avr*(cj.cnt-1)+cj.t)/cj.cnt;//求平均值
	cj.rh_avr=(1.0*cj.rh_avr*(cj.cnt-1)+cj.rh)/cj.cnt;
	
	if(cj.t_max<cj.t) cj.t_max=cj.t;//求最大值
	if(cj.rh_max<now.rh) cj.rh_max=cj.rh;
	
	
}


