#ifndef __ALL__
#define __ALL__

#include "stc15f2k60s2.h"
#include <STC15F2K60S2.H>
#include "stdio.h"
#include "string.h"
#include "intrins.h"

typedef unsigned char u8;
typedef unsigned int u16;

#include "time.h"
#include "key.h"
#include "smg.h"
#include "ctrl.h"
#include "iic.h"

#define CLOSE() P2&=0x1f//关门
#define OPEN(a)	P2|=a		//开门
//
#define LED 0x80	//--100-Y4
#define CTRL 0xa0	//--101-Y5
#define W8 0xc0		//--110-Y6
#define D8 0xe0	  //--111-Y7
//
#define JDQ 1<<4			//0001_0000
#define MOTO 1<<5			//0010_0000
#define FMQ 1<<6			//0100_0000


xdata struct _cj
{
	u8 adc;//0~50
	float v;//电压值
	float v_set;//电压参数
	u8 cnt;
	//程序内变量
};
extern bdata u8 led_dat;

extern xdata struct _cj cj;

#endif 
