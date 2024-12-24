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
#include "onewire.h"
#include "iic.h"
#include "ctrl.h"
#include "wave.h"
#include "uart.h"

xdata struct caiji
{
	float t;//温度 两位小数
	u8 d;//距离 0~99
	//
	u8 t_set;//温度参数0~99
	u8 d_set;//
	//
	u8 t_tmp;//参数数据
	u8 d_tmp;
	//
	u16 cnt;//0~65535   参数变动次数
	u8 f_dac;//0xff 0 //开/关
};

extern xdata struct caiji cj;

extern u8 bdata led_dat;


#endif 
