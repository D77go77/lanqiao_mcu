#ifndef __ICC_H__
#define __ICC_H__

#include "all.h"
sbit scl=P2^0;
sbit sda=P2^1;
#define AD_R 0x91
#define AD_W 0x90
 
#define DA 0
#define AD 1

void init_adc(u8 port);
u8 read_adc(void);
void write_dac(u8 dat);

#endif