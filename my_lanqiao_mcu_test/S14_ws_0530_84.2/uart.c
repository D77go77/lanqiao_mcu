#include "uart.h"
void Uart1_Init(void)	//4800bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x8F;			//���ö�ʱ��ʼֵ
	T2H = 0xFD;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	
	P_SW1 = (P_SW1 & ~0xc0) | 0x80;
	TI=1;
}
