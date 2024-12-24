#include "control.h"
u8 ctr_dat = 0;

void gate(u8 a)
{
	CLOSE();
	OPEN(a);
	CLOSE();
}

void on(u8 a)
{
	if((ctr_dat&a)!=0)return;
	ctr_dat|=a;
	P0=ctr_dat;
	gate(CTRL);	
}
void off(u8 a)
{
	if((ctr_dat&a)==0)return;
	ctr_dat &= ~a;
	P0 = ctr_dat;
	gate(CTRL);
}




