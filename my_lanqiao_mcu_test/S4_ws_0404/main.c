/*
1����ÿ��������򶼵���  ����ʱ�������ϴ�����
2�����̰�����ģ��ֻ���Ǿ���ʱ��д������ģ��
3����ĿҪ������ϸ������ע��ÿһ��ϸ��Ҫ��
4�������ú������޸ģ��ϸ�Ҫ��
5���ο���ֻ����˼·��ʵ��Ҫ��ȫ���Լ���
6����Ҫ����Ҫ��ע��
*/
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
//
xdata struct _cj cj;
//var
u8 run_state=10;
//******************************************************
void main_init(void);
//
void key_mainloop(void);
void led_mainloop(void);
void state_mainloop(void);
void rtc_mainloop(void);
void adc_mainloop(void);
void jdq_proc(void);
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
		adc_mainloop();
		led_mainloop();
	}
}
//******************************************************
void state_mainloop(void)
{
	if(!f_100ms_state)return;
	f_100ms_state=0;

	switch(run_state)
	{
		case 10://ʱ��ʪ�Ƚ���
			sprintf(dis_str,"%02bu-%02bu %02bu",cj.h,cj.m,cj.rh);
		break;
		
		case 20://�趨����
			sprintf(dis_str,"--    %02bu",cj.rh_tmp);
		break;
	}
	disp_tran();	
}

//******************************************************
void jdq_proc(void)
{
	switch(cj.mode)
	{
		case 0://�Զ�ģʽ
			L1=1;
			if(cj.rh < cj.rh_set) 
			{
				on(JDQ);
			}
			else off(JDQ);
		break;
		case 0xff://�ֶ�ģʽ
			L2=1;
			if(cj.rh < cj.rh_set && cj.f_fmq)
			{
				on(FMQ);
			}else off(FMQ);
			//
			if(cj.f_jdq)
			{
				on(JDQ);
			}else off(JDQ);
		break;
	}
}
//*******************************
void adc_mainloop(void)
{
	u8 tmp;
	if (!f_100ms_adc)	return;
	f_100ms_adc = 0;
	//
	tmp=read_adc();//��λ����ȡ
	cj.rh=(u16)tmp*99/255;
}
//******************************************************
void rtc_mainloop(void)//��ȡʱ��
{
	u8 h,m,s;
	static u8 s_old=0;
	
	if(!f_100ms_rtc)return;
	f_100ms_rtc=0;
	//P12=0;	
	h=Read_Ds1302_Byte(0x85);
	m=Read_Ds1302_Byte(0x83);
	s=Read_Ds1302_Byte(0x81);
	//P12=1;
	if(s_old==s)return;//��û�и��£�˵��û��һ�룬ֱ���ˡ�
	s_old=s;
	
	cj.h = (h>>4)*10 + (h&0x0f);
	cj.m = (m>>4)*10 + (m&0x0f);
	cj.s = (s>>4)*10 + (s&0x0f);
}
//******************************************************
void main_init(void)
{
	off(JDQ);off(FMQ);
	led_dat = 0;	
	ds1302_init();//hms
	ds1302_run(0x08,0x30,0x00);//hms
	init_adc(0x03); //Rb2
	Timer1_Init();
	
	run_state=10;
	cj. h=8;
	cj. m=30;
	cj. s=0;
	
	cj. rh=0;//ʪ��
	cj. rh_set = read_eeprom(0);//��ȡ�趨ֵ0~99
	if(cj. rh_set > 99)cj. rh_set=50;
	cj. rh_tmp=cj. rh_set;
	cj. mode=0;//0Ϊ�Զ� ffΪ�ֶ�
	cj. f_jdq=0;//�ر�
	cj. f_fmq=0;
	//
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
		case S4://--
			switch(run_state)
			{
				case 10:
					if(cj.mode==0xff)cj.f_jdq=0;break;//�ֶ�ģʽ//�ر�
				break;
				case 20:if(--cj.rh_tmp>=99)cj.rh_tmp=0;break;
			}
			break;
		case S5://++
			switch(run_state)
			{
				case 10:
					if(cj.mode==0xff)cj.f_jdq=1;break;//�ֶ�ģʽ//����
				break;
				case 20:if(++cj.rh_tmp>=99)cj.rh_tmp=99;break;
			}
			break;	
		case S6:
			switch(run_state)
			{
				case 10:
					switch(cj.mode)
					{	
						case 0:run_state=20;break;//�Զ�ģʽ
						case 0xff:cj.f_fmq=~cj.f_fmq;break;//�ֶ�ģʽ//����fmq
					}
				break;
				case 20:run_state=10;
						write_eeprom(0,cj.rh_tmp);
						cj.rh_set=cj.rh_tmp;break;//����
			}
			break;
		case S7:
			switch(run_state)
			{
				case 10:cj.mode=~cj.mode;break;//�л�ģʽ
			}
			break;
	}
	key_code = 0;
}
//******************************************************
void led_mainloop(void)
{	
	if (!f_100ms_led) return;
	f_100ms_led = 0;
	led_dat=0;
	
	jdq_proc();//�̵���������led����
	//
	P0 = ~led_dat;
	gate(LED);
}
