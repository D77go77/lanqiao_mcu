#ifndef UART_H
#define UART_H

#include "all.h"


void Uart1_Init(void);

void uu_init();


idata struct _uu
{
	u8 f 	: 1;    // 串口接收标志位 (1 bit)
	u8 ton 	: 1;    // 串口超时处理允许标志 (1 bit)
	u8 cmd 	: 3;    // 串口处理命令 (最多支持 0-7 的数值) (6 bits)
	u8 i 	: 3;    // 串口索引 (1 byte)
	u8 t 	: 8;    // 串口超时计数 (1 byte)
	u8 buf[11];    	// 串口接收缓存区 (11 bytes)
};

extern idata struct _uu uu;

void uart_proc();

void uart_timeout(void);







#endif 

