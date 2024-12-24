#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

#include <all.h>

sbit DQ = P1^4;

u16 read_tem16();
void start_18B20();

#endif