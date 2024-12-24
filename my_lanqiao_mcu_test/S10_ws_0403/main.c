/*
1����ÿ��������򶼵���  ����ʱ�������ϴ�����
2�����̰�����ģ��ֻ���Ǿ���ʱ��д������ģ��
3����ĿҪ������ϸ������ע��ÿһ��ϸ��Ҫ��
4�������ú������޸ģ��ϸ�Ҫ��
5���ο���ֻ����˼·��ʵ��Ҫ��ȫ���Լ���
6����Ҫ����Ҫ��ע��
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
u8 cnt_5s = 0;//��������5��ˢ��
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
	
	tmp=read_e2prom(0x06);//0~5Ϊ�����ַ6Ϊ�жϵ�ַ
	if(!tmp)//tmp=0 ������Ч
	{
		for (i = 0; i < 6; i++) 
		{
			x.w[i] = read_e2prom(0x00 +i); //
		}
	}
	else//tmp����0��˵����һ��д
	{
		for (i = 0; i < 6; i++) 
		{
			write_e2prom(0x00 + i, 8);//8
			x.w[i] = 8;
		}
		write_e2prom(0x06,0x00);//ȷ����ͨ��
	}
	//
	run_state = 10;
	x.i = 0;
	x.n = 6;
}
//****************************
void state_mainloop(void)
{
	static u8 index = 0;//�ж���������
	u8 i = 0;
	//
	if(!f_100ms_state) return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10://��ʼ����ȫ��
			sprintf(dis_str,"        ");
		break;
		case 20://����
			sprintf(dis_str,"- "); // �����һ�����ַ�
			word_proc();
			if(x.i == 6)//����6λ����ʱ�ж�
			{
				for(i = 0;i<6;i++)
					if(x.w[i] == x.w_put[i]) index++;//�ж���ȷ
				//
				if(index == 6)//��ȷ//indexΪ�ж����뼸λ��ȷ,6λȫ������ȷ
				{
					run_state = 30;index=0;
				}else//����
				{
					run_state = 40; index=0;
				}
			}
		break;
		case 30://open
			sprintf(dis_str,"0   0PEN");
		break;
		case 40://fall
			sprintf(dis_str,"- "); // �����һ�����ַ�
			word_proc();
		break;
		case 50://set�޸�����
			sprintf(dis_str,"C ");
			word_proc();
		break;
	}
	disp_tran();
}
//******************************
void word_proc(void)//���������������ʾ//��λ��λ����
{
	u8 temp_str[6];//����������������
	u8 i = 0;
	//
	for(i = 0; i < x.n-x.i; i++)//
		strcat(dis_str, " "); // ������������Ŀո�
	for(i = 0; i < x.i; i++)
	{
		sprintf(temp_str,"%d",(int)x.w_put[i]); // �����ָ�ʽ���� temp_str
		strcat(dis_str, temp_str); // �� temp_str ���ӵ� dis_str
	}
}
//*************************************
void time_proc(void)//5��ʱ�䴦���Լ����ֺ�LED��ز���
{
	switch(run_state)
	{
		case 10://��ʼ
			off(JDQ);
		break;
		case 30://����

		case 50://�޸�����
			on(JDQ);
			if(!f_1s)return;
			f_1s = 0;
			if(++cnt_5s >= 5)
			{
				cnt_5s = 0;
				run_state = 10;//����
				led_dat = 0;//off led smg
				x.i = 0;//������������
			}
		break;
		case 40://����
			L1 = 1;
			if(!f_1s)return;
			f_1s = 0;
			if(++cnt_5s >= 5)
			{
				cnt_5s = 0;
				L1 = 0;
				x.i = 0;//������������
				run_state = 10;//����
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
	if(key_code==10)return;//10Ϊ��
	
	switch(run_state)//��������0~9�ж�
	{
		case 20://��������
			if(key_code>=0 && key_code<= 9)
			{
				if(x.i ==6)return;
				x.w_put[x.i]=key_code;
				x.i++;//����
			}
		break;
		case 50://�޸�����
			if(key_code>=0 && key_code<= 9)
			{
				cnt_5s = 0;//�����°���ˢ��5s
				if(x.i ==6)return;
				x.w_put[x.i]=key_code;
				x.i++;//����
			}
	}
	//
	switch(key_code)//��������޸ĺ�����
	{
		case S8://���
			switch(run_state)
			{
				case 20:
				case 50:
				x.w_put[x.i]=0;
				if(--x.i >6)x.i = 0;//����					
				break;
			}
		break;
		//
		case S12: //���ź��޸�����
			switch(run_state)
			{
				case 30:run_state = 50;x.i = 0;cnt_5s = 0;break;//x.i=0����dis_str�Ŀո�    ����ˢ��5scnt
				case 50:
					cnt_5s = 0;					
					if(x.i >=6)
					{
						write_e2prom(0x06,0);
						run_state =30;//���淵�ؿ���
						for(_i = 0;_i < 6;_i++)
						{
							x.w[_i] = x.w_put[_i];
							write_e2prom(_i,x.w[_i]);//д��eeprom����
						}
					}
				break;
			}
		break;
		//
		case S16://����
			switch(run_state)//��ʼ����
			{
				case 10:run_state = 20;break;
			}
		break;
		
		case S8L:
			switch(run_state)
			{
				case 20:
				case 50:
				x.i = 0;//���				
				break;
			}
		break;
				
	}
	key_code = 10;//10Ϊ��
}
//************************
void led_mainloop(void)
{
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	
	led_dat = 0;
	switch(run_state)
	{
		case 20:L7 = 1;break;//����
		case 50:L8 = 1;break;
	}
	time_proc();
	
	P0 = ~led_dat;
	gate(LED);	
}
//*********************


