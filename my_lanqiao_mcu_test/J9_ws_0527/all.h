#ifndef _ALL_H
#define _ALL_H


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

#include <STC15F2K60S2.H>
#include "stdio.h"
#include "intrins.h"

#include "iic.h"
#include "smg.h"
#include "ctrl.h"
#include "key.h"
#include "time.h"
#include "onewire.h"


xdata struct _cj
{

	float t;//�¶�
	float t_re;//�����¶�
	u16 f;//Ƶ��
	u16 f_re;
	float ad;//��ѹ
	float ad_re;
	float ad_set;
};

extern xdata struct _cj cj;
extern u8 bdata led_dat;

#endif 
