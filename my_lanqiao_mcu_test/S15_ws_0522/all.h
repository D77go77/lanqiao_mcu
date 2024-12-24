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
#include "ds1302.h"



xdata struct caiji
{
	float da_v;
	u8 da_dat;
	
	long int f;
	u16 f_lim;
	int f_jiao;
	u8 h;
	u8 m;
	u8 s;
	u16 f_max;
	u8 h_save;
	u8 m_save;
	u8 s_save;
	u16 f_lim_tmp;
	int f_jiao_tmp;
};

extern xdata struct caiji cj;

extern u8 bdata led_dat;


#endif 
