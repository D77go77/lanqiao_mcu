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
u8 run_state = 10;
//
//************************************************
void main_init();
//
void state_mainloop();
void key_mainloop();
void wave_mainloop();
void temper_mainloop();
void uart_mainloop();
void led_mainloop();
//************************************************
void main()
{
	main_init();
	EA = 1;	
	while(1)
	{
		key_mainloop();
		state_mainloop();
		
		wave_mainloop();
		temper_mainloop();	
		uart_mainloop();
		led_mainloop();

		
	}
}
//************************************************
void main_init()
{
	u8 tmp1,tmp2;
	tmp1 = r_e2prom(0x00);
	tmp2 = r_e2prom(0x01);
	
	cj.cnt = (u16)tmp1 <<8 | tmp2;
	
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	Uart1_Init();//串口
	CMOD=CH=CL=0;//PCA
	init_ad(0x40);
	
	cj.t = 0;
	cj.d = 0;
	cj.t_set = cj.t_tmp =30;
	cj.d_set = cj.d_tmp =35;
	
	cj.f_dac = 0xff;
		
	init_temp();
}
//************************************************
void state_mainloop()
{	
	switch(run_state)
	{
		case 10:sprintf(dis_str, "C %7.2f", cj.t);break;//数据界面温度
		case 11:sprintf(dis_str, "L     %02bu", cj.d);break;//距离
		case 12:sprintf(dis_str, "N  %5u", cj.cnt);break;//次数
		//
		case 20:sprintf(dis_str, "P  1  %02bu", cj.t_tmp);break;//参数界面温度
		case 21:sprintf(dis_str, "P  2  %02bu", cj.d_tmp);break;//距离
	}
	disp_proc();
}
//************************************************
void key_mainloop()
{
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	
	switch(key_code)
	{
		case 12:
			switch(run_state)
			{
				case 10:
				case 11:
				case 12:
					cj.d_tmp = cj.d_set;
					cj.t_tmp = cj.t_set;
					run_state = 20;
				break;
				case 20:
				case 21:
					run_state = 10;
					if((cj.d_tmp != cj.d_set) || (cj.t_tmp != cj.t_set))//参数数值发生变化
					{
						cj.cnt++;
						w_e2prom(0x00,cj.cnt>>8);
						w_e2prom(0x01,cj.cnt&0x00ff);
						cj.d_set=cj.d_tmp ;
						cj.t_set=cj.t_tmp ;
					};//保存在eeprom
				break;
			}
			break;
			//
		case 13:
			switch(run_state)
			{
				case 10:run_state = 11;break;
				case 11:run_state = 12;break;
				case 12:run_state = 10;break;
				case 20:run_state = 21;break;
				case 21:run_state = 20;break;
			}
			break;
			//
		case 16:
			switch(run_state)
			{
				case 20:cj.t_tmp -= 2;if(cj.t_tmp >= 99)cj.t_tmp = 0;break;
				case 21:cj.d_tmp -= 5;if(cj.d_tmp >= 99);cj.d_tmp = 0;break;
			}
			break;
			//
		case 17:
			switch(run_state)
			{
				case 20:cj.t_tmp += 2;if(cj.t_tmp >= 99)cj.t_tmp = 99;break;
				case 21:cj.d_tmp += 5;if(cj.d_tmp >= 99);cj.d_tmp = 99;break;
			}
			break;
			//
		case S12L:
				cj.cnt = 0;
				w_e2prom(0x00,cj.cnt>>8);
				w_e2prom(0x01,cj.cnt&0x00ff);
			break;			
		case S13L:
				cj.f_dac = ~cj.f_dac;
			break;
	}
	key_code = 0;
}
//************************************************
void temper_mainloop()
{
	//ES = 0;
	if (!f_500ms_temp)return;
	f_500ms_temp = 0;
	
	cj.t = read_tem();
	init_temp();
	//ES = 1;
}
//***************************
void wave_mainloop()//
{		
	float dist_f;	
	if (!f_100ms_wave) return;
	f_100ms_wave = 0;
	
	dist_f=read_d()*0.017f;
	if(dist_f>=99.0f)dist_f=99.0f;
	
	if(dist_f>=10 && dist_f<=50)
	{
		cj.d = dist_f;
	}else
		cj.d = 0;
	
	
	if(cj.f_dac)
	{
		if(cj.d<=cj.d_set)
			write_dac(102);
		else
			write_dac(204);	
	}else
		write_dac(20);
}
//************************************************
void uart_mainloop()
{
	if(uu.f)
	{
		uu.f=0;
		switch(uu.i)
		{
			case 4:
				if(uu.buf[0]=='S'&&uu.buf[1]=='T'&&uu.buf[2]=='\r'&&uu.buf[3]=='\n')
					uu.cmd=1;
			break;
			case 6:
				if(uu.buf[0]=='P'&&uu.buf[1]=='A'&&uu.buf[2]=='R'&&uu.buf[3]=='A'&&uu.buf[4]=='\r'&&uu.buf[5]=='\n')
					uu.cmd=2;
				else
					uu.cmd=3;
			break;
		}
	}
	
	if(uu.cmd)
	{
		ES=0;TI=1;
		switch(uu.cmd)
		{
			case 1:printf("$%2bu,%4.2f\r\n",cj.d,cj.t);break;
			case 2:printf("#%2bu,%2bu\r\n",cj.d_set,cj.t_set);break;
			case 3:printf("ERROR\r\n");break;
		}
		ES=1;TI=0;
		uu_init();
		
	}
}
//**************************************************
void led_mainloop()//
{
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	if(cj.t>cj.t_set)
		L1=1;
	else
		L1=0;
	
	if(cj.d<cj.d_set)
		L2=1;
	else
		L2=0;
	
	if(cj.f_dac)
		L3=1;
	else
		L3=0;
	
}
//**************************

