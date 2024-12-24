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
u8 bdata blink8_dat=0;
//
xdata struct _cj x;
//
//
u8 led1[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
u8 led2[4] = {0x18,0x24,0x42,0x81};
u8 run_state = 0;
bit f_led_t = 0;
//
void main_init(void);
void state_mainloop(void);
void key_mainloop(void);
//
void led_mainloop(void);
//
void adc_mainloop(void);
void blink8_mainloop(void);

void Delay10ms(void);
void e2prom_init(void);
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
		blink8_mainloop();
		adc_mainloop();
	}
}
//****************************
void main_init(void)
{
	u8 i;//tmp
	
	off(JDQ);off(FMQ);
	led_dat = 0;	
	Timer1_Init();
	init_adc(0x03);//RB2

	//eeprom
	for(i = 1;i<5;i++)
	{
		x.t[i] = read_e2prom(i);
		if(x.t[i] < 4||x.t[i] > 12)
		{
			x.t[i]=4;
			write_e2prom(i,4);//��ʼ�����
			Delay10ms();
		}
	}
	
	run_state = 10;
	x.light = 0;
	x.f_on = 0;//�������ص�
	x.adc = 0;
	x.mode = 1;
	x.m_set = 1;

}
//****************************
void state_mainloop(void)
{
	if(!f_100ms_state)return;
	f_100ms_state = 0;
	switch(run_state)
	{
		case 10://��ʾ����
			sprintf(dis_str,"        ");
		break;
		case 20://���ý���//ģʽ
			sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t[x.m_set]*100);
		break;
		case 21://���ý���//��ת���
			sprintf(dis_str,"-%1bu- %4d",x.m_set,x.t_set*100);
		break;
		case 30://������ʾ����
			sprintf(dis_str,"      -%1bu",x.light);
		break;
	}
	disp_tran();
}
//******************************
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
				case 20:
				if(--x.m_set <= 1)x.m_set = 1;	
				break;
				case 21:
				if(--x.t_set <= 4)x.t_set = 4;	//400ms
				break;
				case 10:run_state=30;break;//������ʾ����
			}
		break;
		case S5://++
			switch(run_state)
			{
				case 20://ģʽ
				if(++x.m_set >= 4)x.m_set = 4;
				break;
				case 21://ʱ��
				if(++x.t_set >= 12)x.t_set = 12;	//1200ms
				break;
			}
		break;
		case S6:
			switch(run_state)
			{
				case 10:run_state = 20;blink8_dat = 0x07;//�������˸//�����޸�ģʽ
						x.m_set=1;//��ģʽ���ó�ʼ����Ϊ1
				break;
				//
				case 20:run_state = 21;blink8_dat = 0xf0;//�������˸//�����޸�ʱ��
						x.t_set=x.t[x.m_set];//��ģʽ��Ӧ����ʵʱ�丳����ʾ�ϵ�����ʱ��
				break;
				//
				case 21:run_state = 10;blink8_dat = 0;//����ܲ���˸//���淵��
						write_e2prom(x.m_set,x.t_set);//д��2402ʱ��������
						x.t[x.m_set]=x.t_set;//��ʱ������ֵ�������ʵʱ��//�޸���תʱ��
				break;
			}
		break;
		case S7:
			x.f_on = ~x.f_on;//��ͣled
		break;
		
		case S4OFF://�ɿ����س�ʼ����
			switch(run_state)
			{
				case 30:run_state=10; break;//�ɿ�������ʾ
			}
		break;
	}
	key_code = 0;
}
//*************************
void led_mainloop(void)
{
	static int t = 0;
	static u8 i = 0;
	
	if(!f_100ms_led)return;
	f_100ms_led = 0;
	if(!x.f_on)return;//S7��ͣ,��i���ڼӼ�
	if(++t <= x.t[x.mode])return;//��ת����ж�
	t = 0;
	
	switch(x.mode)
	{
		case 1:
			led_dat = led1[i];if(++i > 7) {i=7;x.mode=2;}
		break;
		case 2:
			led_dat = led1[i];if(--i > 7) {i=4;x.mode=3;}
		break;
		case 3:
			led_dat = led2[i];if(--i > 3) {i=0;x.mode=4;}
		break;
		case 4:
			led_dat = led2[i];if(++i > 3) {i=0;x.mode=1;}
		break;
	}
}
//**********************************
void blink8_mainloop(void)
{
	u8 i=0,tmp = 0;
	
	if (!f_800ms_blink8) return;//500ms
	f_800ms_blink8 = 0;
	
	if(blink8_dat == 0)return;
	
	for(i = 0;i <8;i++)
	{
		tmp  = blink8_dat & (1 << i);
		if(tmp)
			dis_blink[i] = ~dis_blink[i];
		else
			dis_blink[i] = 0;
	}
}
//*********************************************
void adc_mainloop(void)//�ɼ�����
{
	if(!f_100ms_adc) return;
	f_100ms_adc = 0;
	
	x.adc  = read_adc();
	x.light = (4 * (u16)x.adc / 256)+1;
}
//***************************
void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}
//***********************
