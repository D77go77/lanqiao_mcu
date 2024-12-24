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
	u16 jdq_cnt;//继电器打开次数  0
	u16 wave_dat;//超声波数据
	u16 wave_set;//超声波参数   60   10cm~120cm
	u16 shidu_dat;//湿度数据  
	u8 shidu_set;//湿度参数  40  10~60
	u16 ping_dat;//频率数据
	u16 ping_set;//频率参数  0.9k 1k~12k
	
	u8 f_cm;//切换m和cm
	u8 f_khz;//切换k和个位
};

extern xdata struct _cj cj;
extern u8 bdata led_dat;

#endif 
