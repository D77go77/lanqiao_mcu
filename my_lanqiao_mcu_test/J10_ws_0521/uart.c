#include "uart.h"
 xdata struct _uu uu;
void Uart1_Isr(void) interrupt 4
{
	if (TI)				//检测串口1发送中断
	{
		TI = 0;			//清除串口1发送中断请求位
	}
	if (RI)				//检测串口1接收中断
	{
		RI = 0;			//清除串口1接收中断请求位
		uu.buf[uu.i++]=SBUF;
		uu.f=1;uu.ton=1;
	}
}

void Uart1_Init(void)	//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x8F;			//设置定时初始值
	T2H = 0xFD;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
		//P_SW1 = (P_SW1 & ~0xc0) | 0x80;
	ES = 1;				//使能串口1中断
}

void uu_init()
{
	uu. f=0;
	uu. i=0;
	uu. t=0;
	uu. ton=0;
	uu.cmd=0;
}
