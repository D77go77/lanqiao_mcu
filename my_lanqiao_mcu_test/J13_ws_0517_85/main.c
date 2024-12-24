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
//
void main_init();
void state_mainloop();
void key_mainloop();
void wave_mainloop();
void adc_mainloop();
void led_mainloop();
void freq_mainloop(void);
//****************************
void main(void)
{
	main_init();
	EA=1;
	while(1)
	{
		
		key_mainloop();
		state_mainloop();
		wave_mainloop();
		adc_mainloop();
		led_mainloop();
		freq_mainloop();
	}
}
//****************************
void main_init()
{
	u8 e2p_tep = 0;
	e2p_tep = r_e2prom(0x05);//	确认是否为第一次上电
	if(!e2p_tep)
	{
		cj.jdq_cnt = r_e2prom(0x00);	//非第一次上电
	}else cj.jdq_cnt = 0;//第一次上电
	
	ne555_init();
	CMOD=CH=CL=0;//PCA_init
	Timer2_Init();
	Timer1_Init();
	init_ad(0x43);
	
	off(JDQ);off(FMQ);
	
	run_state = 10;
	//cj.jdq_cnt = 0;//jdq打开次数  0
	cj.wave_dat = 0;//超声波数据
	cj.wave_set = 60;//超声波参数   60
	cj.shidu_dat = 0;//湿度数据
	cj.shidu_set = 40;//湿度参数  40
	cj.ping_dat = 0;
	cj.ping_set = 90;//频率参数  90* 100
	cj.f_cm = 0;
	cj.f_khz = 0;
}
//****************************
void state_mainloop()
{
	if (!f_100ms_state)return;
	f_100ms_state = 0;
	cj.ping_dat = ping_dat;
	switch (run_state)
	{
		case 10: // 频率
			if(!cj.f_khz)
			{
				sprintf(dis_str,"F %6u",cj.ping_dat);				
			}else
			{
				sprintf(dis_str,"F %7.1f",cj.ping_dat/1000.0);		
			}		
			break;
		//
		case 20: // 湿度
			sprintf(dis_str,"H     %2d",(u16)cj.shidu_dat);	
			break;
		//
		case 30: // 测距
			if(!cj.f_cm)
			{
				sprintf(dis_str,"A    %3d",cj.wave_dat);				
			}else
			{
				sprintf(dis_str,"A    %4.2f",cj.wave_dat/100.00);		
			}	
			break;
		//
		case 40: // 频率参数
			sprintf(dis_str,"P1   %4.1f",cj.ping_set/10.0);	
			break;
		//
		case 41: // 湿度参数
			sprintf(dis_str,"P2    %2d",(int)cj.shidu_set);	
			break;
		//
		case 42: // 距离参数
			sprintf(dis_str,"P3    %3.1f",cj.wave_set/100.0);	
			break;
		//
	}	
	disp_proc();
}
//****************************
void key_mainloop()
{
	if (!f_20ms_key) return;
	f_20ms_key = 0;
	key_proc();
	
	if(!key_code)return;
	
	switch(key_code)
	{
		case 4:
			switch(run_state)
			{
				case 10:run_state = 20;break;
				case 20:run_state = 30;break;
				case 30:run_state = 40;break;
				
				case 40:
				case 41:
				case 42:run_state = 10;break;
			}
			break;
		//	
		case 5:
			switch(run_state)
			{
				case 40:run_state = 41;break;
				case 41:run_state = 42;break;
				case 42:run_state = 40;break;
			}
			break;
		//
		case 6:
			switch(run_state)
			{
				case 30:cj.f_cm = ~cj.f_cm;break;
				case 40:cj.ping_set += 5;if(cj.ping_set > 120) cj.ping_set = 10;break;
				case 41:cj.shidu_set += 10;if(cj.shidu_set > 60) cj.shidu_set = 10;break;
				case 42:cj.wave_set += 10;if(cj.wave_set > 120) cj.wave_set = 10;break;
			}
			break;			
		//	
		case 7:
			switch(run_state)
			{
				case 10:cj.f_khz = ~cj.f_khz;break;
				case 40:cj.ping_set -= 5;if(cj.ping_set < 10) cj.ping_set = 120;break;
				case 41:cj.shidu_set -= 10;if(cj.shidu_set < 10) cj.shidu_set = 60;break;
				case 42:cj.wave_set -= 10;if(cj.wave_set < 10) cj.wave_set = 120;break;
			}
			break;	
		//
		case S77:
			switch(run_state)//仅湿度显示下
			{
				case 20:cj.jdq_cnt = 0;	w_e2prom(0x00,0);//保存继电器次数			
			}
			break;
	}
	key_code=0;
}
//******************************
void adc_mainloop(void)
{
	static bit f_jdq_on = 0;
	
	u8 temp = 0;
	float tmp=0;
	
	if(!f_100ms_adc)return;
	f_100ms_adc = 0;
	
	read_ad();
	temp = read_ad();
	cj.shidu_dat = 99*(u16)temp /255;
	
	//dac输出
	if(cj.shidu_dat >= 80)
	{
		write_dac(255);
	}else if(cj.shidu_dat <= cj.shidu_set)
	{
		write_dac(51);
	}else 
	{
		tmp = (4.0*((u16)cj.shidu_dat-80.0)/(80.0-(float)cj.shidu_set)+5);//函数计算
		temp = tmp*51;//转化为ad数
		write_dac(temp);//转换为16进制
	}
	
	//继电器开关
	if(cj.wave_dat > cj.wave_set && f_jdq_on==0)
	{
		on(JDQ);f_jdq_on = 1;cj.jdq_cnt++;
		w_e2prom(0x05,0);//确认程序已上电且保存值
		w_e2prom(0x00,(u8)cj.jdq_cnt);//保存继电器次数
	}else if(cj.wave_dat < cj.wave_set && f_jdq_on==1)
	{
		off(JDQ);f_jdq_on = 0;cj.jdq_cnt++;
		w_e2prom(0x05,0);//确认程序已上电且保存值
		w_e2prom(0x00,(u8)cj.jdq_cnt);//保存继电器次数
	}
}
//****************************
void wave_mainloop()
{
	if(!f_100ms_wave)return;
	f_100ms_wave = 0;
	

	cj.wave_dat = read_d()*0.017f;//读超声波
	if(cj.wave_dat >=200)cj.wave_dat =200;//限幅
}
//****************************
void led_mainloop()
{
	if (!f_50ms_led)return;
	f_50ms_led = 0;
	
	led_dat=0;
	switch(run_state)
	{
		case 10:
			L1=1;
		break;
		
		case 20:
			L2=1;
		break;
		
		case 30:
			L3=1;
		break;
		
		case 40:
			L1=f_on;
		break;
		
		case 41:
			L2=f_on;
		break;
		
		case 42:
			L3=f_on;
		break;
	}
	//
	if(cj.ping_dat > cj.ping_set*100)
		L4 = 1;

	if(cj.shidu_dat > cj.shidu_set)
		L5 = 1;

	if(cj.wave_dat > cj.wave_set)
		L6 = 1;

}
//****************************
void freq_mainloop(void)
{
	if(!f_freq) return;
	f_freq=0;
	
	if(cj.ping_dat > cj.ping_set*100)pwm_set = 4;
	else pwm_set = 1;
}










