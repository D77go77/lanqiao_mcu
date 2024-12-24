#ifndef __ALL_H__
#define __ALL_H__

#include <STC15F2K60S2.H>
#include <intrins.h>
#include <stdio.h>

typedef unsigned char u8;
typedef unsigned int u16;
xdata struct _cj
{
	u8 max_10;
	u8 max_1;
	u8 min_10;
	u8 min_1;
	u8 max;
	u8 min;
	u8 gap;
	u8 t;
};

extern xdata struct _cj cj;
#define LED 0x80
#define CTL 0xa0
#define D8 0xe0
#define W8 0xc0
#define JDQ 1<<4
#define FMQ 1<<6
#define gate(a) {P2 =(P2 &0x1f)|a;P2 &=0x1f;}
extern u8 bdata led_dat;
#include <time.h>
#include <key.h>
#include <led8.h >
#include <control.h>
#include <onewire.h>

#endif