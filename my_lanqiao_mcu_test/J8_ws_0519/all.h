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
#include "iic.h"
#include "ctrl.h"
#include "wave.h"



xdata struct caiji
{
	u8 d_old;//�ϴ�����
	u8 d;//��������
	
	u8 d_set;//����ä��
	u8 d_tmp;//set_tmp

	u8 index;//���ݱ��
	u8 dat[5];//���ݱ���
	u8 f_wave;
	
	float dac;
};

extern xdata struct caiji cj;

extern u8 bdata led_dat;


#endif 
