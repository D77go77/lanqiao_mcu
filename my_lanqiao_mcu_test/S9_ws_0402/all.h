#ifndef __ALL__
#define __ALL__

#include "stc15f2k60s2.h"
#include <STC15F2K60S2.H>
#include "stdio.h"
#include "intrins.h"

typedef unsigned char u8;
typedef unsigned int u16;

#include "time.h"
#include "key.h"
#include "led8.h"
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
	int t1;//流转时间400~1200  4*100
	int t2;
	int t3;
	int t4;
	
	u8 l;//亮度等级
	u8 f_on;
	u8 adc;
	u8 mode;
	u8 m_set;//模式设置
	int t_set;//时间设置值
	u8 i;//index
	//程序内变量
};
extern bdata u8 led_dat;

extern xdata struct _cj x;

#endif 
