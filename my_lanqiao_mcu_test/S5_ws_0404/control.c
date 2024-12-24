#include <control.h>

u8 crl_dat =0;

void crl_on(u8 a)
{

	crl_dat |=a;
	P0 = crl_dat;
	gate(CTL);

}

void crl_off(u8 a)
{

	
	crl_dat &=~ a;
	P0 = crl_dat;
	gate(CTL);

}