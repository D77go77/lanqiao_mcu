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
u8 run_state=10;
bit f_led_1=0;
//
void main_init();
void state_mainloop();
void key_mainloop();
void wave_mainloop();
void adc_mainloop();
void led_mainloop();
//***********************
void main()
{
	main_init();
	EA=1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		wave_mainloop();
		adc_mainloop();
		led_mainloop();
	}
}
//***********************
void main_init()
{
	off(JDQ);off(FMQ);
	led_dat=0;
	Timer1_Init();
	
	cj. d_old=0;//上次数据
	cj. d=0;//本次数据
	cj. d_set=cj.d_tmp=r_e2prom(0x00);//测量盲区

	cj. index=1;//数据编号
	cj. dat[4]=r_e2prom(4);
	cj. dat[1]=r_e2prom(1);
	cj. dat[2]=r_e2prom(2);
	cj. dat[3]=r_e2prom(3);;//数据保存使用1~4位
	
	cj.f_wave=0;
	
	cj.dac=0;
}
//***********************
void state_mainloop()
{
	if(!f_100ms_state)return;
	f_100ms_state=0;
	
	switch(run_state)
	{
		case 10:sprintf(dis_str,"C %03bu%03bu",cj.d,cj.d_old);break;//测距显示
		case 20:sprintf(dis_str,"%bu    %03bu",cj.index,cj.dat[cj.index]);break;//数据回显
		case 30:sprintf(dis_str,"F    %3bu",cj.d_tmp);break;//参数设置
	}
	disp_proc();
}
//***********************
void key_mainloop()
{
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	switch(key_code)
	{
		case 4:
			if(run_state==10)cj.f_wave=1;//启动
		break;
			//
		case 5:
			switch(run_state)
			{
				case 10:cj.index=1;run_state = 20;break;
				case 20:run_state = 10;break;
				case 30:cj.index=1;run_state = 20;break;
			}
		break;
			//
		case 6:
			switch(run_state)
			{
				case 10:run_state = 30;break;
				case 20:run_state = 30;break;
				case 30:
					run_state = 10;
					cj.d_set=cj.d_tmp;
					w_e2prom(0x00,cj.d_set);//保存
				break;
			}
			//
		break;
		case 7:
			switch(run_state)
			{
				case 20:
					if(++cj.index>4)cj.index=1;
				break;
				case 30:
					cj.d_tmp+=10;
					if(cj.d_tmp>30)cj.d_tmp=0;
				break;
			}
		break;
	}
	key_code = 0;
}
//***********************
void wave_mainloop()
{
	static u8 _index=1;//索引
	
	if(!cj.f_wave)return;
	cj.f_wave=0;
	f_led_1=1;
	cj.d_old=cj.d;//保存上次
	cj.d = read_d()*0.017f;
	cj.dat[_index]=cj.d;
	w_e2prom(_index,cj.d);//保存
	if(++_index>4)_index=1;
}
//***********************
void adc_mainloop()
{
	if(!f_100ms_adc)return;
	f_100ms_adc=0;
	
	if(cj.d <= cj.d_set)
	{
		write_dac(0);
	}else
	{
		cj.dac = (cj.d-cj.d_set)*0.02;
		if(cj.dac>=5)
			write_dac(255);
		else
			write_dac(51*cj.dac);
	}
}
//***********************
void led_mainloop()
{
	static u8 L1_cnt=0;
	static u8 t_cnt=0;
	
	if(!f_100ms_led)return;
	f_100ms_led=0;
	
	if(f_led_1)
	{
		if(++t_cnt>=10)
		{
			t_cnt=0;
			L1=!L1;
			if(++L1_cnt>5)
			{
				L1_cnt=0;
				L1=0;
				f_led_1=0;
			}
		}
	}
	//
	switch(run_state)
	{
		case 10:L7=0;L8=0;break;
		case 20:L7=0;L8=1;break;
		case 30:L7=1;L8=0;break;
	}
}
//***********************


















