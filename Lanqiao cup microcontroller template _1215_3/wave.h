#ifndef WAVE_H
#define WAVE_H

#include "all.h"

sbit TX = P1^0;
sbit RX = P1^1;

u32 read_d();

void wave_proc();
	

#endif 
