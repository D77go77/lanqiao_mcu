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
u8 run_state;
u8 _index=0;
//
void main_init();
void state_mainloop();
void key_mainloop();
void led_mainloop();
void tem_mainloop();
void wave_mainloop();
void adc_mainloop();
u8 calc_dac_out(u16 _dat);
//****************************
void main(void)
{
	main_init();
	EA=1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		tem_mainloop();
		wave_mainloop();
		adc_mainloop();
		led_mainloop();
	}
}
//****************************
void main_init()
{
	off(JDQ);off(FMQ);
	led_dat  = 0;
	run_state = 10;
	
	cj. t = 0; //温度23.5
	cj. d = 0; //距离25
	cj. d_set = 40;//距离参数
	cj. t_set = 30;//温度参数
	cj. d_cal = 0;//距离校准
	cj. v_cal = 340;//速度校准
	cj. dac_lim = 1;
	cj. d_save = 0 ;//距离保存值
	cj. dac =  0;//0~255
	cj.index=0;
	cj.f_dac=0;
	cj.d_[0]=cj.d_[1]=cj.d_[2]=cj.d_[3]=cj.d_[4]=cj.d_[5]=cj.d_[6]=cj.d_[7]=cj.d_[8]=cj.d_[9]=0;
	
	Timer1_Init();
	CMOD=CH=CL=0;//PCA_init
	init_temp();//temp
	init_ad(0x43);//adc
}
//****************************
void state_mainloop()
{
	static u8 cnt_6s = 0;
	if(!f_100ms_state)return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10:
			sprintf(dis_str,"%4.1f-%4d",cj.t,cj.d);
		break;
		case 11:
			sprintf(dis_str,"%4.1f- %4.2f",cj.t,cj.d/100.0);
		break;
		case 20:
			sprintf(dis_str,"P1    %02d",cj.d_set);
		break;
		case 21:
			sprintf(dis_str,"P2    %02d",cj.t_set);
		break;
		case 30:
			sprintf(dis_str,"F1   %3d",cj.d_cal);
		break;
		case 31:
			sprintf(dis_str,"F2  %4d",cj.v_cal);
		break;
		case 32:
			sprintf(dis_str,"F3    %2.1f",cj.dac_lim);
		break;
		case 40:
			sprintf(dis_str,"%03.1f-%4d",cj.t,cj.d);
			if(++cnt_6s == 60)
			{
				run_state = 10;cnt_6s = 0;_index=0;
			}
		break;		
		case 41:
			sprintf(dis_str,"%03.1f- %4.2f",cj.t,cj.d/100.0);
			if(++cnt_6s == 60)
			{
				run_state = 11;cnt_6s = 0;//_index=0;
			}
		break;	
	}
	disp_proc();
}
//****************************
void key_mainloop()
{
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	
	if(!key_code)return;
	
	switch(key_code)
	{
		case 4:
			switch(run_state)
			{
				case 10:
				case 11:run_state = 20;break;
				case 20:
				case 21:run_state = 30;break;
				case 30:
				case 31:
				case 32:run_state = 10;f_1000ms_wave=1;break;
			}
			break;
			//
		case 5:
			switch(run_state)
			{
				case 10:run_state = 11;break;
				case 11:run_state = 10;break;
				case 20:run_state = 21;break;
				case 21:run_state = 20;break;
				case 30:run_state = 31;break;
				case 31:run_state = 32;break;
				case 32:run_state = 31;break;
			}
			break;
			//
		case 8:
			switch(run_state)
			{
				case 10:cj.index=0;run_state = 40;break;
				case 11:cj.index=0;run_state = 41;break;//6s
				case 20:cj.d_set += 10;if(cj.d_set >90)cj.d_set = 90;break;
				case 21:cj.t_set += 1;if(cj.t_set >80)cj.t_set = 80;break;
				case 30:cj.d_cal += 5;if(cj.d_cal >90)cj.d_cal = 90;break;
				case 31:cj.v_cal += 10;if(cj.v_cal >9990)cj.v_cal = 9990;break;
				case 32:cj.dac_lim += 0.1;if(cj.dac_lim >2.0)cj.dac_lim = 2.0;break;
			}
			break;
			//
		case 9:
			switch(run_state)
			{
				case 10:
				case 11:
					cj.f_dac=1;
					f_1000ms_adc=1;
//						cj.dac = calc_dac_out(cj.d_save);
//						write_dac(cj.dac);//dac
						break;
				case 20:cj.d_set -= 10;if(cj.d_set <10)cj.d_set = 10;break;
				case 21:cj.t_set -= 1;if(cj.t_set <0)cj.t_set = 0;break;
				case 30:cj.d_cal -= 5;if(cj.d_cal <-90)cj.d_cal = -90;break;
				case 31:cj.v_cal -= 10;if(cj.v_cal <10)cj.v_cal = 10;break;
				case 32:cj.dac_lim -= 0.1;if(cj.dac_lim <0.1)cj.dac_lim = 0.1;break;
			}
			break;
			//
		case S89:
			run_state = 10;
			cj.t = 0;
			cj.d = 0;
			cj.d_set = 40;
			cj.t_set = 30;
			cj.d_cal = 0;
			cj.v_cal = 340;
			cj.dac_lim = 1;
			cj.d_save = 0;
			cj.dac = 0;
			break;
	}
	key_code = 0;
}
//****************************
void tem_mainloop()
{
	if(!f_800ms_tem)return;
	f_800ms_tem = 0;
	
	cj.t =  read_tem() ;
	if(cj.t>=80)cj.t=80;
	else if(cj.t<=0)cj.t=0;
	
	init_temp();
}
//****************************
void wave_mainloop()
{
	int dist_i;
	float dist_f;
	
	if(!f_1000ms_wave)return;
	f_1000ms_wave = 0;
	
	
	dist_f = read_d()/20000.0*cj.v_cal + cj.d_cal;
	dist_i = dist_f + 0.5 ;
	if(dist_i >= 90)
		dist_i = 90;
	else if(dist_i <= 10)
		dist_i = 10;
	
	cj.d = dist_i;
	
	if((cj.d >= cj.d_set - 5 && cj.d <= cj.d_set +5) && cj.t <= cj.t_set)
	{
		on(JDQ);
	}else
	{
		off(JDQ);
	}
	
	if(!(run_state==40 || run_state==41))return;//非记录的时候，不记录
	cj.d_[cj.index]=cj.d;//记录一组
	cj.v_[cj.index]=calc_dac_out(cj.d_[cj.index]);
	++cj.index;
}
//****************************
u8 calc_dac_out(u16 _dat)
{
	float _v;
	u8 tmp;
	if(_dat > 90)
		tmp = 255;//5
	else if(_dat < 10)
		tmp = 255* cj.dac_lim / 5;
	else
	{
		_v = (cj.dac_lim - 5.0 )* (10.0-_dat)/80.0 + cj.dac_lim;
		tmp = 255 *_v /5;
	}
	return tmp;
}
//****************************
void led_mainloop()
{
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	switch(run_state)
	{
		case 10:
		case 11:
		case 40:
		case 41:
			led_dat = cj.d;
		break;
		//
		case 20:
		case 21:
			led_dat = 0;
			L8 = 1;
		break;
		//
		case 30:
		case 31:
		case 32:
			L8=0;
			L1 = !L1;
		break;
	}
}
//****************************
void adc_mainloop()
{	
	if(run_state==40 || run_state==41)return;//记录的时候不输出
	
	if(!f_1000ms_adc)return;
	f_1000ms_adc=0;
	
	if(cj.f_dac)//允许输出
	{
		if(cj.index)//采集到数据
		{
			write_dac(cj.v_[_index]);//dac

			if(++_index>=cj.index)//输出结束
			{
				_index=0;
				//cj.f_dac=0;
			}
		}
//		else 
//		write_dac(calc_dac_out(0));
	}
//	else 
//	write_dac(0);
}
//****************************










