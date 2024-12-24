#include "ctrl.h"
u8 ctrl=0x70;

void gate(u8 a)
{
	CLOSE();
	OPEN(a);
	CLOSE();
}

void on(u8 a)
{
	if((ctrl&a)!=0)return ;
	ctrl|=a;
	P0=ctrl;
	gate(CTRL);

}

void off(u8 a)
{
	if((ctrl&a)==0)return ;
	ctrl&=~a;
	P0=ctrl;
	gate(CTRL);

}