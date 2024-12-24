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
		uu.f=1;
		uu.ton=1;
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
//	P_SW1 = (P_SW1 &~WEI)|LED;//P_SW1 = (P_SW1 & ~0xc0) | 0x80;		//UART1/USART1: RxD(P1.6), TxD(P1.7)
	ES = 1;				//使能串口1中断
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
 * 处理UART通信的函数
 * 该函数根据接收的数据决定响应的操作
 */
void uart_proc()
{
    // 检查是否有待处理的数据
    if(uu.f)
    {
        uu.f=0;
        // 根据接收的数据长度决定如何处理
        switch(uu.i)
        {
            case 3:
                // 当接收的数据以'A'开头，后面紧跟回车和换行符时，设置命令为1
                if(uu.buf[0]=='A'&&uu.buf[1]=='\r'&&uu.buf[2]=='\n')
                    uu.cmd=1;
                else
                    // 否则设置命令为2，表示接收到的数据不符合预期格式
                    uu.cmd=2;
                break;
        }
    }
    // 如果有命令待执行
    if(uu.cmd)
    {
        // 禁用串口中断，防止在发送过程中被中断
        ES=0;TI=1;
        // 根据命令执行相应的操作
        switch(uu.cmd)
        {
            case 1:printf("hello\r\n");break;
            case 2:printf("error\r\n");break;
        }
        // 重新启用串口中断，准备接收下一次数据
        ES=1;TI=0;
        // 重置UART相关的状态和缓冲区
        uu_init();
    }
}
//***************