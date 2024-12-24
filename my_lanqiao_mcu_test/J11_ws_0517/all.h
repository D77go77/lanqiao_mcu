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
#include "ds1302.h"



xdata struct caiji
{

	u8 hour,min,sec;//时钟
	float t;//温度
	u8 h_set;//小时
	u8 h_tmp;
	u8 t_set;//温度
	u8 t_tmp;
	u8 led_set;//led
	u8 led_tmp;
	u8 f_dark;//亮暗
	float dac;//dac输出
	u8 adc;
};

extern xdata struct caiji cj;

extern u8 bdata led_dat;


#endif 
