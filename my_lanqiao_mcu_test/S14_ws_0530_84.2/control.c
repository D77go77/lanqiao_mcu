#include "control.h"
u8 ctr_dat = 0;
//*********************************************
void gate(u8 a)
{
	CLOSE();
	OPEN(a);
	CLOSE();
}
//on(JDQ),on(FMQ),on(MOTO) ָ����͵�ƽ  
//off(JDQ),off(FMQ),off(MOTO) ָ����ߵ�ƽ 
//PWM�����off(MOTO)��ʱ�����ռ�ձ�ʱ�䡣
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




