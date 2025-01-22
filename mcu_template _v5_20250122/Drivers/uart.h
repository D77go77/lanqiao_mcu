#ifndef UART_H
#define UART_H

#include "all.h"


void Uart1_Init(void);

void uu_init();


idata struct _uu
{
	u8 f 	: 1;    // ���ڽ��ձ�־λ (1 bit)
	u8 ton 	: 1;    // ���ڳ�ʱ���������־ (1 bit)
	u8 cmd 	: 3;    // ���ڴ������� (���֧�� 0-7 ����ֵ) (6 bits)
	u8 i 	: 3;    // �������� (1 byte)
	u8 t 	: 8;    // ���ڳ�ʱ���� (1 byte)
	u8 buf[11];    	// ���ڽ��ջ����� (11 bytes)
};

extern idata struct _uu uu;

void uart_proc();

void uart_timeout(void);







#endif 

