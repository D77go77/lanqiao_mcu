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

#include <STC15F2K60S2.H>
#include "stdio.h"
#include "intrins.h"

#include "onewire.h"
#include "iic.h"
#include "smg.h"
#include "ctrl.h"
#include "key.h"
#include "time.h"
#include "wave.h"


xdata struct _cj
{
	float t; //温度23.5
	int d; //距离25
	//
	int d_set;//距离参数
	int t_set;//温度参数
	//
	int d_cal;//距离校准
	int v_cal;//速度校准
	float dac_lim;
	//
	int d_save;//距离保存值
	u8 dac;//0~255
	
	
	u8 d_[20];//采集距离一组
	u8 v_[20];
	u8 index;
	u8 f_dac;
};

extern xdata struct _cj cj;
extern u8 bdata led_dat;

#endif 
