#ifndef IIC_H
#define IIC_H

#include "all.h"

sbit sda = P2^1;
sbit scl = P2^0;

void init_ad(u8 add);
u8 read_ad();
void w_dac(u8 tmp);
void w_e2p(u8 add,u8 val);
u8 r_e2p(u8 add);


void adc_proc();
	
#endif 
