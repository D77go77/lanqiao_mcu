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
#include "ds1302.h"
#include "onewire.h"

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
	u8 h;//真实值
	u8 m;
	u8 s;
	
	u8 h_set;//闹钟值
	u8 m_set;
	u8 s_set;
	
	u8 h_tmp;//设置值(int)
	u8 m_tmp;
	u8 s_tmp;
	
	u8 t;//温度
	//程序内变量
};
extern bdata u8 led_dat;

extern xdata struct _cj cj;

#endif 
