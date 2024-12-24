#include "uart.h"
xdata struct _uu uu;
void Uart1_Isr(void) interrupt 4
{
	if (TI)				//��⴮��1�����ж�
	{
		TI = 0;			//�������1�����ж�����λ
	}
	if (RI)				//��⴮��1�����ж�
	{
		RI = 0;			//�������1�����ж�����λ
		uu.buf[uu.i++]=SBUF;
		uu.f=1;
		uu.ton=1;
	}
}

void Uart1_Init(void)	//4800bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x8F;			//���ö�ʱ��ʼֵ
	T2H = 0xFD;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
//	P_SW1 = (P_SW1 &~WEI)|LED;//P_SW1 = (P_SW1 & ~0xc0) | 0x80;		//UART1/USART1: RxD(P1.6), TxD(P1.7)
	ES = 1;				//ʹ�ܴ���1�ж�
}

void uu_init()
{
	uu. f=0;
	uu. i=0;
	uu. t=0;
	uu. ton=0;
	uu. cmd=0;
}

/**
 * ����UARTͨ�ŵĺ���
 * �ú������ݽ��յ����ݾ�����Ӧ�Ĳ���
 */
void uart_proc()
{
    // ����Ƿ��д����������
    if(uu.f)
    {
        uu.f=0;
        // ���ݽ��յ����ݳ��Ⱦ�����δ���
        switch(uu.i)
        {
            case 3:
                // �����յ�������'A'��ͷ����������س��ͻ��з�ʱ����������Ϊ1
                if(uu.buf[0]=='A'&&uu.buf[1]=='\r'&&uu.buf[2]=='\n')
                    uu.cmd=1;
                else
                    // ������������Ϊ2����ʾ���յ������ݲ�����Ԥ�ڸ�ʽ
                    uu.cmd=2;
                break;
        }
    }
    // ����������ִ��
    if(uu.cmd)
    {
        // ���ô����жϣ���ֹ�ڷ��͹����б��ж�
        ES=0;TI=1;
        // ��������ִ����Ӧ�Ĳ���
        switch(uu.cmd)
        {
            case 1:printf("hello\r\n");break;
            case 2:printf("error\r\n");break;
        }
        // �������ô����жϣ�׼��������һ������
        ES=1;TI=0;
        // ����UART��ص�״̬�ͻ�����
        uu_init();
    }
}
//***************