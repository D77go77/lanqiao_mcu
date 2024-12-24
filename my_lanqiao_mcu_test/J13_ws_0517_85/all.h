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
#include "wave.h"


xdata struct _cj
{
	u16 jdq_cnt;//�̵����򿪴���  0
	u16 wave_dat;//����������
	u16 wave_set;//����������   60   10cm~120cm
	u16 shidu_dat;//ʪ������  
	u8 shidu_set;//ʪ�Ȳ���  40  10~60
	u16 ping_dat;//Ƶ������
	u16 ping_set;//Ƶ�ʲ���  0.9k 1k~12k
	
	u8 f_cm;//�л�m��cm
	u8 f_khz;//�л�k�͸�λ
};

extern xdata struct _cj cj;
extern u8 bdata led_dat;

#endif 
