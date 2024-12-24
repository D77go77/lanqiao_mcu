#ifndef __ALL_H__
#define __ALL_H__

#include "stc15f2k60s2.h"
#include "stdio.h"
#include "intrins.h"

typedef unsigned char u8 ;
typedef unsigned int u16 ;

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

#include "time.h"
#include "smg.h"
#include "key.h"
#include "control.h"

#include "iic.h"
#include "ds1302.h"
#include "onewire.h"
#include "uart.h"

#endif