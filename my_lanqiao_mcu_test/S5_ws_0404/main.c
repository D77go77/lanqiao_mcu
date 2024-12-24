/*
1）把每个赛题程序都当成  竞赛时的最终上传程序
2）工程包含的模块只能是竞赛时会写出来的模块
3）题目要逐行仔细分析，注意每一个细节要求。
4）程序编好后，认真修改，严格要求。
5）参考的只能是思路，实现要完全靠自己。
6）重要内容要有注释
*/
#include <all.h>
//*******************
xdata struct _cj cj;
u8 state =10;
u8 ledms =0;
//*******************
volatile u8 bdata led_dat =0;
sbit L1 = led_dat ^0;
sbit L2 = led_dat ^1;
sbit L3 = led_dat ^2;
sbit L4 = led_dat ^3;
sbit L5 = led_dat ^4;
sbit L6 = led_dat ^5;
sbit L7 = led_dat ^6;
sbit L8 = led_dat ^7;
//*******************
void init();
void state_mainloop();
void temp_mainloop();
void key_mainloop();
void led_mainloop();
//*******************
void main()
{
	init();
	EA =1;
	while(1)
	{
		state_mainloop();
		temp_mainloop();
		key_mainloop();
		led_mainloop();
	}
}
//*******************
void init()
{
	Timer1_Init();
	cj.gap =0;
	cj.t =0;

	cj.min = 20;
	cj.max = 30;
	start_18B20();
}
//*******************
void state_mainloop()
{
	if(!f_100ms_state)return;
	f_100ms_state =0;
	
	switch(state)
	{
		case 10:sprintf(disp_str,"-%bu-   %2bu",cj.gap,cj.t);break; //温度显示界面
		case 20:sprintf(disp_str,"-    -  ");break;//温度设定界面
		case 21:sprintf(disp_str,"-    - %bu",cj.max_10);break;//温度设定界面
		case 22:sprintf(disp_str,"-    -%bu%bu",cj.max_10,cj.max_1);break;//温度设定界面
		case 23:sprintf(disp_str,"- %bu  -%bu%bu",cj.max_10,cj.max_1,cj.min_10);break;//温度设定界面
		case 24:sprintf(disp_str,"-%bu%bu  -%bu%bu",cj.max_10,cj.max_1,cj.min_10,cj.min_1);break;//温度设定界面
	}
	
	disp_tran();
}
//*******************
void key_mainloop()
{
	if(!f_20ms_key)return;
	f_20ms_key =0;
	
	key_proc();
	
	switch(key_code)
	{
		case SET:
					switch(state)
					{
						case 10:state =20;break;
						case 24:
									cj.max = cj.max_10*10 +cj.max_1;
									cj.min = cj.min_10*10 +cj.min_1;
									state = 10;break;
					}
			break;
					
		case CLEAR:
					switch(state)
					{
						case 24:state = 23;cj.min_1 =0;break;
						case 23:state = 22;cj.min_10 =0;break;
						case 22:state = 21;cj.max_1 =0;break;
						case 21:state = 20;cj.max_10 =0;break;
					}
			break;					
	}
	
	switch(state)
	{
		case 20:
				if(key_code>0 &&key_code<=10)
				{
					if(key_code ==10)
					{
						key_code -= 10;
						cj.max_10 = key_code;
						state = 21;
					}else
					cj.max_10 = key_code;state = 21;
				}
			break;
				
		case 21:
				if(key_code>0 &&key_code<=10)
				{
					if(key_code ==10)
					{
						key_code -= 10;
						cj.max_1 = key_code;state = 22;
					}else
					cj.max_1 = key_code;state = 22;
				}
			break;
				
		case 22:
				if(key_code>0 &&key_code<=10)
				{
					if(key_code ==10)
					{
						key_code -= 10;
						cj.min_10 = key_code;state = 23;
					}else
					cj.min_10 = key_code;state = 23;
				}
			break;
				
		case 23:
				if(key_code>0 &&key_code<=10)
				{
					if(key_code ==10)
					{
						key_code -= 10;
						cj.min_1 = key_code;state = 24;
					}else
					cj.min_1 = key_code;state = 24;
				}
			break;
	}
	
	key_code=0;
}
//*******************
void temp_mainloop()
{
	u16 dat =0;
	u8 tempi =0;
	
	if(!f_100ms_temp)return;
	f_100ms_temp =0;
	
	dat = read_tem16();
	
	tempi = dat/16;
	cj.t = tempi;
	
	if(cj.t <cj.min)
	{
		cj.gap =0;crl_off(JDQ);ledms = 8;
	}
	if(cj.t>=cj.min &&cj.t<=cj.max)
	{
		cj.gap =1;crl_off(JDQ);ledms = 4;
	}
	if(cj.t>cj.max)
	{
		cj.gap =2;crl_on(JDQ);ledms = 2;
	}
	
	start_18B20();
}
//*******************
void led_mainloop()
{
	static cnt=0;
	static bit f_blink =0;
	if(!f_100ms_led)return;
	f_100ms_led =0;
	
	if(++cnt >= ledms&&!f_blink)
	{
		cnt =0;
		L1 =0;
		f_blink =1;
	}else if(++cnt >= ledms&&f_blink)
	{
		cnt =0;
		f_blink =0;
		L1 =1;
	}
	
	if(cj.max<cj.min)
	{
		L2 =1;
	}
	else L2 =0;
	
	P0 = ~led_dat;
	gate(LED);
}