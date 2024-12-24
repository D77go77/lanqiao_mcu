#ifndef __ICC_H__
#define __ICC_H__

#include "all.h"
sbit scl=P2^0;
sbit sda=P2^1;

void init_adc(u8 port);
u8 read_adc(void);
void write_dac(u8 dat);
void write_eeprom(u8 adr,u8 val);
u8 read_eeprom(u8 adr);

#endif