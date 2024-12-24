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
u8 run_state;
//
//**************************
void main_init();
void time_mainloop();
void key_mainloop();
void state_mainloop();
void rtc_mainloop();
void adc_mainloop();
void wave_mainloop();
void led_mainloop();
float deal_dac_out(float _dat);
void calcu_wave();
void calcu();//�ɼ���ֵ����
void calcu_dac();//����dac���
//**************************
void main()
{
	main_init();
	EA=1;
	while(1)
	{
		key_mainloop();
		state_mainloop();
		rtc_mainloop();
		adc_mainloop();
		wave_mainloop();
		led_mainloop();
	}
}
//**************************
void main_init()
{
	Timer1_Init();
	
	off(JDQ);off(FMQ);
	CMOD=CH=CL=0;//PCA_INIT
	
	rtc_init();
	rtc_run(0x20,0x20,0x00);
	
	init_ad(0x41);
	
	run_state = 10;
	
	cj.d_dat = 0;//�������� //>=80
	cj.d_all = 0;
	cj.d_cnt = 0;
	cj.d_max = 0;//���ֵ
	cj.d_aver = 0;//ƽ��ֵ
	cj.d_min = 0;//��Сֵ
	cj.d_set = 20;//�������  10~80    20
	cj.set_tmp=20;
	cj.d_tim = 2;//�ɼ�ʱ��  2 3 5 7 9   2
	cj.tim_tmp=2;
	cj.mode = 1;//�����л�  1���2��С��3ƽ��
	cj.f_cf = 0xff;//�����л���ʱ
	cj.cf_3 = 0;//����
	cj.adc = 0;//now_adc
	cj.hour=20;
	cj.min=20;
	cj.sec=0;
}
//**************************
void state_mainloop()
{
	if(!f_100ms_state)return;
	f_100ms_state = 0;
	
	switch(run_state)
	{
		case 10: //ʱ����� 
			sprintf(dis_str,"%02bu-%02bu-%02bu",cj.hour,cj.min,cj.sec);
			break;
		//
		case 11: //���
			if(cj.f_cf)
			{
				sprintf(dis_str,"LC   %3d",cj.d_dat);
			}else
			{
				sprintf(dis_str,"LF   %3d",cj.d_dat);	
			}
			break;
		//
		case 12: //���ݼ�¼
			if(cj.mode == 1)
			{
				sprintf(dis_str,"H*  %4d",cj.d_max);
			}else if(cj.mode == 2)
			{
				sprintf(dis_str,"H-  %5.1f",cj.d_aver);
			}else if(cj.mode == 3)
			{
				sprintf(dis_str,"H_  %4d",cj.d_min);
			}
			break;
		//
		case 20: //����ʱ��
			sprintf(dis_str,"P1    0%1d",cj.tim_tmp);
			break;
		//
		case 21: //��������
			sprintf(dis_str,"P2    %2d",cj.set_tmp);
			break;
		//
	}
	disp_proc();	
}
//**************************
void key_mainloop()
{	
	if(!f_20ms_key)return ;
	f_20ms_key = 0;
	
	key_proc();
	
	if(!key_code)return;
	
	switch(key_code)
	{
		//
		case 4:
			switch(run_state)
			{
				case 10://ʱ����ʾ�����
				case 11://������ʾ
				case 12:run_state=20;break;//���ݼ�¼
				case 20:
				case 21:
						cj.d_tim = cj.tim_tmp;//����ʱ��
						cj.d_set=cj.set_tmp ;//�������
						run_state=10;break;//�������÷���
			}
			break;
		//
		case 5:
			switch(run_state)
			{
				case 10:run_state=11;break;//ʱ����ʾ�����ģʽ
				case 11:run_state=12;cj.mode = 1;break;//��ൽ������ʾ����
				case 12:run_state=10;break;//���ݼ�¼
				case 20:run_state=21;break;//��������ʱ��//����������ʾ
				case 21:run_state=20;break;//�������þ���
			}
			break;
		//
		case 8:
			switch(run_state)
			{
				case 11:cj.f_cf =~ cj.f_cf;break;//�л���ʱ����ģʽ
				case 12:if(++cj.mode > 3) cj.mode = 1;break;//������ʾ
			}
			break;
		//
		case 9:			
			switch(run_state)
			{
				case 20:
					switch(cj.tim_tmp)
					{
						case 2: cj.tim_tmp = 3;break;
						case 3: cj.tim_tmp = 5;break;
						case 5: cj.tim_tmp = 7;break;
						case 7: cj.tim_tmp = 9;break;
						case 9: cj.tim_tmp = 2;break;
					}
					break;//��������ʱ��
				case 21:
					cj.set_tmp += 10;
					if(cj.set_tmp > 80) cj.set_tmp = 10;break;//�������þ���
			}
			break;
	}
	key_code = 0;
}
//**************************
void rtc_mainloop()
{
	u8 hour,min,sec;
	if(!f_100ms_rtc)return;
	f_100ms_rtc=0;
	
	hour=Read_Ds1302_Byte(0x85);
	min=Read_Ds1302_Byte(0x83);
	sec=Read_Ds1302_Byte(0x81);
	if(cj.sec==sec)return;
	cj.hour=10*(hour>>4)+(hour&0x0f);
	cj.min=10*(min>>4)+(min&0x0f);
	cj.sec=10*(sec>>4)+(sec&0x0f);
}
//**************************
bit f_led_L6 = 0;//�ж�L6�Ƿ�����

#define LIGHT 50
#define DARK 15
bit f_dark=1;
//**************************
void adc_mainloop()
{
	u8 adc_temp = 0;//adc
	
	if(!f_100ms_adc)return;
	f_100ms_adc=0;
	
	read_ad();
	adc_temp=read_ad();
	cj.adc=100.0 *adc_temp /255.0 ;
	
	
	if(cj.f_cf)//����
	{
		if(!f_dark && cj.adc<DARK && run_state >=20)//��ת��
		{
			f_dark=1;
		}
		if(f_dark && cj.adc>LIGHT)
		{
			f_dark=0;
		}
	}else//��ʱ
	{
		if(cj.adc>LIGHT)
			f_dark=0;
		else if(cj.adc<DARK)
			f_dark=1;
	}
	
	if(!f_dark) //�ⳡ��L6��
	{f_led_L6 = 1;}
	else { f_led_L6 = 0; }
	
	calcu_dac();
	calcu();

}
//**************************
void calcu_dac()//����dac���
{
	if(cj.d_dat <= 10)//dac���
	{
		write_dac(51);	
	}
	else if(cj.d_dat >= 80)
	{
		write_dac(255);
	}else
	{
		write_dac(deal_dac_out(1.0*cj.d_dat)/5.0 * 255);//dac���
	}
}
//**************************
float deal_dac_out(float _dat)
{
	float temp = 0;
	temp = (4.0*_dat)/70.0 + (3/7.0);
	return temp + 0.1;
}
//**************************
void calcu()//�ɼ���ֵ����
{
	if(cj.d_dat >= cj.d_max)  cj.d_max = cj.d_dat;//����
	if(cj.d_dat <= cj.d_min)  cj.d_min = cj.d_dat;
	
	//ƽ��ֵ���һ�μ��㴦��
	if (cj.d_cnt == 1)
	{
		cj.d_aver = cj.d_all / (float)cj.d_cnt;
		cj.d_min = cj.d_dat;
	}else if(cj.d_cnt > 1)
	{
		cj.d_aver = cj.d_all / (float)cj.d_cnt;
	}
}
//*******************************
void wave_mainloop()
{
	static u8 old_sec=0xff;//��ʱģʽ����//�ϴδ�������
	
	if(!f_100ms_wave) return;
	f_100ms_wave = 0;
	
	if(run_state >= 20)return;//����ģʽ������
	
	if(cj.f_cf)//����
	{
		if(!f_dark && cj.adc<DARK)//��ת��
		{
			f_dark=1;
			calcu_wave();
		}
		
	}else//��ʱ
	{
		if(!(cj.sec % cj.d_tim)) 
		{
			if(cj.sec ==old_sec)return;//������һ��
			old_sec=cj.sec;
			calcu_wave();
		}
	}
}
//**************************
void calcu_wave()//����ɼ�d
{
	cj.d_dat = read_d()*0.017f;
	if(cj.d_dat>=999)cj.d_dat=999;
	cj.d_cnt++;
	cj.d_all += cj.d_dat;
	
	if(cj.f_cf)return;//����ʱģʽ
	
	if((cj.d_dat <= (cj.d_set + 5)) && (cj.d_dat >= (cj.d_set - 5)))//�����ɼ�����L5
	{ 
		if(++cj.cf_3 >= 3) cj.cf_3 = 3;
	}
	else{cj.cf_3 = 0;}
}
//**************************
void led_mainloop(void)
{	
	if (!f_100ms_led) return;
	f_100ms_led = 0;
	
	switch(run_state)
	{
		case 10:
			L1 = 1;L2 = 0;L3 = 0;L4 = 0;L5 = 0;
		break;
		//
		case 11:
			L1 = 0;L2 = 1;L3 = 0;
			if(cj.f_cf)
			{
				L4 = 1;
			}else L4 = 0;
			
			if(cj.cf_3 >= 3)
			{
				L5 = 1;
			}else
			{
				L5 = 0;
			}
		break;
		//
		case 12:
			L2 = 0;L1 = 0;L3 = 1;L4 = 0;L5 = 0;
		break;
		case 20:
			L2 = 0;L1 = 0;L3 = 0;L4 = 0;L5 = 0;
		break;
		case 21:
			L2 = 0;L1 = 0;L3 = 0;L4 = 0;L5 = 0;
		break;
	}

	if(f_led_L6)
		L6 = 1;
	else
		L6 = 0;
}
//**************************
	