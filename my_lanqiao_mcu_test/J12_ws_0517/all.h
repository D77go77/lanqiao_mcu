#ifndef ALL__
#define ALL__

#include <STC15F2K60S2.H>
#include "stdio.h"
#include "intrins.h"

typedef unsigned char u8;
typedef unsigned int u16;

#define CTRL 0xa0
#define LED 0x80
#define WEI 0xc0
#define DUAN 0xe0
#define CLOSE() P2&=0x1f
#define OPEN(a) P2|=a

#define FMQ 1<<6
#define JDQ 1<<4
#define MOTO 1<<5

#include "time.h"
#include "key.h"
#include "smg.h"
#include "wave.h"
#include "iic.h"
#include "ctrl.h"
#include "ds1302.h"







xdata struct caiji
{
	u16 d_dat;//��������
	u16 d_all;//�����ܺ�
	u16 d_max;//�����ܺ����ֵ
	float d_aver;//ƽ��ֵ
	u16 d_min;//��Сֵ
	u16 d_set;//�������  10~80    20
	u16 set_tmp;//��������ֵ
	int d_cnt;//�����ɹ�����
	int d_tim;//�ɼ�ʱ��  2 3 5 7 9   2
	int tim_tmp;//����ֵ
	u8 mode;//�����л�  1���2��С��3ƽ��
	u8 f_cf;//�����л���ʱ
	u8 cf_3;//����3�βɼ�
	u8 adc;//now_adc
	u8 hour,min,sec;//ʱ��
};

extern xdata struct caiji cj;

extern u8 bdata led_dat;


#endif 
