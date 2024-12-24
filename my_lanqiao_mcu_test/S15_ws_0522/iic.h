#ifndef IIC_H
#define IIC_H

#include "all.h"
sbit scl=P2^0;
sbit sda=P2^1;

void init_ad(u8 add);

u8 read_ad();

void write_dac(u8 add);
u8 r_e2prom(u8 add);

#endif 
