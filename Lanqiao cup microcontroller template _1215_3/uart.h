#ifndef UART_H
#define UART_H

#include "all.h"


void Uart1_Init(void);

void uu_init();


xdata struct _uu
{
	u8 f;
	u8 i;
	u8 t;
	u8 ton;
	u8 cmd;
	u8 buf[3];
};

extern xdata struct _uu uu;

void uart_proc();









#endif 

