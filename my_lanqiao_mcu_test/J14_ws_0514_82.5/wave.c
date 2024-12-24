#include "wave.h"
void de(u8 a);
float read_d()
{
	u16 dist=0;
	u8 num=16;
	TX=0;
	while(num--)
	{
		de(12);
		TX=!TX;
	}
	CR=0;
	CH=CL=0;
	CR=1;
	while(RX && CH<0x20);
	CR=0;
	
	dist= ((u16)CH<<8|CL);
	
	return dist;
}

void de(u8 a)
{
	while(a--);
}
