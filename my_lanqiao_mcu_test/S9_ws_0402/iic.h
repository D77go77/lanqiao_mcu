#ifndef _IIC_H_
#define _IIC_H_

#include "all.h"

sbit scl = P2^0;
sbit sda = P2^1;

void init_adc(u8 add);
u8 read_adc(void);
void write_dac(u8 dat);
void write_e2prom(u8 add,u8 dat);
u8 read_e2prom(u8 add);
#endif 
