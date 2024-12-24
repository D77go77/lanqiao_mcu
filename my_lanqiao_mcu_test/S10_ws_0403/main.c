/*
1）把每个赛题程序都当成  竞赛时的最终上传程序
2）工程包含的模块只能是竞赛时会写出来的模块
3）题目要逐行仔细分析，注意每一个细节要求。
4）程序编好后，认真修改，严格要求。
5）参考的只能是思路，实现要完全靠自己。
6）重要内容要有注释
*/
#include "all.h"
//
volatile u8 bdata led_dat = 0;
sbit L1 = led_dat ^0;
sbit L2 = led_dat ^1;
sbit L3 = led_dat ^2;
sbit L4 = led_dat ^3;
sbit L5 = led_dat ^4;
sbit L6 = led_dat ^5;
sbit L7 = led_dat ^6;
sbit L8 = led_dat ^7;
//
xdata struct _cj x;
//
u8 run_state = 0;
u8 cnt_5s = 0;//按键重置5秒刷新
//
void main_init(void);
void state_mainloop(void);
void key_mainloop(void);
void led_mainloop(void);
//
void Delay5ms(void);
void word_proc(void);
void time_proc(void);
//*******************************
void main(void)
{
	main_init();
	EA = 1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		led_mainloop();
	}
}
//****************************
void main_init(void)
{
	u8 tmp,i;
	
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	
	tmp=read_e2prom(0x06);//0~5为密码地址6为判断地址
	if(!tmp)//tmp=0 数据有效
	{
		for (i = 0; i < 6; i++) 
		{
			x.w[i] = read_e2prom(0x00 +i); //
		}
	}
	else//tmp不是0，说明第一次写
	{
		for (i = 0; i < 6; i++) 
		{
			write_e2prom(0x00 + i, 8);//8
			x.w[i] = 8;
		}
		write_e2prom(0x06,0x00);//确认已通电
	}
	//
	run_state = 10;
	x.i = 0;
	x.n = 6;
}
//****************************
void state_mainloop(void)
{
	static u8 index = 0;//判断密码索引
	u8 i = 0;
	//
	if(!f_100ms_state) return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10://初始界面全灭
			sprintf(dis_str,"        ");
		break;
		case 20://输入
			sprintf(dis_str,"- "); // 先添加一个连字符
			word_proc();
			if(x.i == 6)//输入6位密码时判断
			{
				for(i = 0;i<6;i++)
					if(x.w[i] == x.w_put[i]) index++;//判断正确
				//
				if(index == 6)//正确//index为判断密码几位正确,6位全对算正确
				{
					run_state = 30;index=0;
				}else//错误
				{
					run_state = 40; index=0;
				}
			}
		break;
		case 30://open
			sprintf(dis_str,"0   0PEN");
		break;
		case 40://fall
			sprintf(dis_str,"- "); // 先添加一个连字符
			word_proc();
		break;
		case 50://set修改密码
			sprintf(dis_str,"C ");
			word_proc();
		break;
	}
	disp_tran();
}
//******************************
void word_proc(void)//数码管输入密码显示//移位退位处理
{
	u8 temp_str[6];//缓存输入密码数组
	u8 i = 0;
	//
	for(i = 0; i < x.n-x.i; i++)//
		strcat(dis_str, " "); // 添加所需数量的空格
	for(i = 0; i < x.i; i++)
	{
		sprintf(temp_str,"%d",(int)x.w_put[i]); // 将数字格式化到 temp_str
		strcat(dis_str, temp_str); // 将 temp_str 连接到 dis_str
	}
}
//*************************************
void time_proc(void)//5秒时间处理以及部分和LED相关操作
{
	switch(run_state)
	{
		case 10://初始
			off(JDQ);
		break;
		case 30://开门

		case 50://修改密码
			on(JDQ);
			if(!f_1s)return;
			f_1s = 0;
			if(++cnt_5s >= 5)
			{
				cnt_5s = 0;
				run_state = 10;//返回
				led_dat = 0;//off led smg
				x.i = 0;//重置输入密码
			}
		break;
		case 40://错误
			L1 = 1;
			if(!f_1s)return;
			f_1s = 0;
			if(++cnt_5s >= 5)
			{
				cnt_5s = 0;
				L1 = 0;
				x.i = 0;//重置输入密码
				run_state = 10;//返回
			}
		break;
	}
}
//************************
void key_mainloop(void)
{
	u8 _i = 0;
	if(!f_20ms_key)return;
	f_20ms_key = 0;
	
	key_proc();
	if(key_code==10)return;//10为空
	
	switch(run_state)//按键输入0~9判断
	{
		case 20://输入密码
			if(key_code>=0 && key_code<= 9)
			{
				if(x.i ==6)return;
				x.w_put[x.i]=key_code;
				x.i++;//限制
			}
		break;
		case 50://修改密码
			if(key_code>=0 && key_code<= 9)
			{
				cnt_5s = 0;//开门下按键刷新5s
				if(x.i ==6)return;
				x.w_put[x.i]=key_code;
				x.i++;//限制
			}
	}
	//
	switch(key_code)//按键清除修改和输入
	{
		case S8://清除
			switch(run_state)
			{
				case 20:
				case 50:
				x.w_put[x.i]=0;
				if(--x.i >6)x.i = 0;//限制					
				break;
			}
		break;
		//
		case S12: //开门后修改密码
			switch(run_state)
			{
				case 30:run_state = 50;x.i = 0;cnt_5s = 0;break;//x.i=0重置dis_str的空格    重置刷新5scnt
				case 50:
					cnt_5s = 0;					
					if(x.i >=6)
					{
						write_e2prom(0x06,0);
						run_state =30;//保存返回开门
						for(_i = 0;_i < 6;_i++)
						{
							x.w[_i] = x.w_put[_i];
							write_e2prom(_i,x.w[_i]);//写入eeprom密码
						}
					}
				break;
			}
		break;
		//
		case S16://输入
			switch(run_state)//初始界面
			{
				case 10:run_state = 20;break;
			}
		break;
		
		case S8L:
			switch(run_state)
			{
				case 20:
				case 50:
				x.i = 0;//清空				
				break;
			}
		break;
				
	}
	key_code = 10;//10为空
}
//************************
void led_mainloop(void)
{
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	led_dat = 0;
	switch(run_state)
	{
		case 20:L7 = 1;break;//输入
		case 50:L8 = 1;break;
	}
	time_proc();
	
	P0 = ~led_dat;
	gate(LED);	
}
//*********************


