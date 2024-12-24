#include "all.h"
xdata struct _cj cj;
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
//
void main_init();
void state_mainloop();
void key_mainloop();
void temper_mainloop();
void adc_mainloop();
void freq_mainloop();
void led_mainloop();
void data_put();//存储
void data_get();//读取
//**********************
void main()
{
	main_init();
	EA=1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		
		temper_mainloop();
		adc_mainloop();
		freq_mainloop();
		led_mainloop();
	}
}
//**********************
void main_init()
{
	off(JDQ);off(FMQ);
	led_dat=0;
	ne555_init();
	Timer1_Init();
	init_ad(0x03);
	
	data_get();
	run_state=12;
	cj.t=0;
	//cj.t_re=0;
	cj.f=0;
	//cj.f_re=0;
	cj.ad=0;
	//cj.ad_re=0;
	//cj.ad_set=1.0;
	init_temp();
}
//*************************
void state_mainloop()
{	
	switch(run_state)
	{
		case 10:sprintf(dis_str, "U     %3.1f", cj.ad);break;//显示电压
		case 11:sprintf(dis_str, "F %6u", cj.f);break;//f
		case 12:sprintf(dis_str, "C   %5.2f", cj.t);break;//温度
		//
		case 20:sprintf(dis_str, "HC%7.2f", cj.t_re);break;//温度回显
		case 21:sprintf(dis_str, "HU%7.1f", cj.ad_re);break;//ad回显
		case 22:sprintf(dis_str, "HF%6u", cj.f_re);break;//f回显
		case 30:sprintf(dis_str, "P     %3.1f", cj.ad_set);break;//设置
	}
	disp_proc();
}
//*************************
void key_mainloop()
{
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	key_proc();
	switch(key_code)
	{
		case 4:
			switch(run_state)
			{
				case 10:run_state=11;break;
				case 11:run_state=12;break;
				case 12:run_state=10;break;
				case 20:run_state=21;break;
				case 21:run_state=22;break;
				case 22:run_state=20;break;
			}
			break;
			//
		case 5://e2prom
			cj.t_re=cj.t;
			cj.f_re=cj.f;
			cj.ad_re=cj.ad;
			data_put();
			break;
			//
		case 6:
			switch(run_state)
			{
				case 10:case 11:case 12:run_state=20;break;
				case 30:cj.ad_set+=0.1;if(cj.ad_set>5.0)cj.ad_set=0.1;break;
					
			}
			break;
			//
		case 7:
			switch(run_state)
			{
				case 10:case 11:case 12:
				case 20:case 21:case 22:
										run_state=30;break;
				case 30:
					run_state=10;
					w_e2prom(0x08,cj.ad_set*10);//*10后存储
					break;
			}
			break;
			//
		case S6L:
			switch(run_state)
			{
				case 30:
					cj.ad_set+=0.1;if(cj.ad_set>5.0)cj.ad_set=0.1;break;
				break;
			}
	}
	key_code = 0;
}
//************************
void adc_mainloop()
{
	u8 adc_temp = 0;//adc
	if(!f_100ms_adc)return;
	f_100ms_adc=0;
	
	adc_temp=read_ad();
	cj.ad=5.00 *adc_temp /255.0 ;
}
//*************************
void temper_mainloop()
{
	if (!f_800ms_tem)return;
	f_800ms_tem = 0;
	
	cj.t = read_tem();
	init_temp();
}
//*************************
void freq_mainloop()
{
	if(!f_100ms_freq)return;
	f_100ms_freq=0;
	
	cj.f=ping_dat;
}
//************************
void led_mainloop()
{
	static u8 cnt=0;
	if(!f_50ms_led)return;
	f_50ms_led=0;
	
	switch(run_state)
	{
		case 12:L1=1;L2=0;L3=0;break;
		case 11:L1=0;L2=1;L3=0;break;
		case 10:L1=0;L2=0;L3=1;break;
	}

	if(cj.ad>cj.ad_set)
	{
		if(++cnt>=4)
		{
			cnt=0;
			L8=!L8;
		}
	}else L8=0;
}
//*************************
void data_put()//存储
{
	u8 t_tmp1,t_tmp2;
	u16 t_tmp;
	u8 f_tmp1,f_tmp2;
	u8 ad_tmp1;
	
	t_tmp1=cj.t;
	t_tmp = 100*cj.t;//先取整温度*100
	t_tmp2= t_tmp% 100;
	w_e2prom(0x00,t_tmp1);//整数
	w_e2prom(0x01,t_tmp2);//小数
	
	f_tmp1=cj.f>>8;
	f_tmp2=cj.f&0x00ff;
	w_e2prom(0x03,f_tmp1);//高8
	w_e2prom(0x04,f_tmp2);//低8
	
	ad_tmp1=cj.ad*10;
	w_e2prom(0x06,ad_tmp1);
}
//*************************
void data_get()
{
	u8 t_tmp1,t_tmp2;
	u8 f_tmp1,f_tmp2;
	u8 ad_tmp1,ad_tmp2;
	
	t_tmp1=r_e2prom(0x00);
	t_tmp2=r_e2prom(0x01);
	cj.t_re=t_tmp1 + 1.0*t_tmp2/100.0;
	
	f_tmp1=r_e2prom(0x03);
	f_tmp2=r_e2prom(0x04);
	cj.f_re=(u16)f_tmp1 <<8 | f_tmp2;
	
	ad_tmp1=r_e2prom(0x06);
	cj.ad_re=1.0*ad_tmp1/10.0;
	
	ad_tmp2=r_e2prom(0x08);//*10后存储
	cj.ad_set=1.0*ad_tmp2/10.0;

}
//*************************
//*************************
//*************************
//*************************










