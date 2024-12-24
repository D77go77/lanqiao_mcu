#include "control.h"
u8 ctr_dat = 0;
//*********************************************
void gate(u8 a)
{
	CLOSE();
	OPEN(a);
	CLOSE();
}
//on(JDQ),on(FMQ),on(MOTO) 指输出低电平  
//off(JDQ),off(FMQ),off(MOTO) 指输出高电平 
//PWM输出：off(MOTO)的时间代表占空比时间。
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




