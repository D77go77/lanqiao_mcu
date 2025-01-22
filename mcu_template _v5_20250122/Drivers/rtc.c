/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/								
#include "rtc.h"
//
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

/**
 * 设置实时时钟（RTC）的时间
 *
 * @param h 小时，范围0-23
 * @param m 分钟，范围0-59
 * @param s 秒钟，范围0-59
 *
 * 此函数通过写入DS1302芯片来设置RTC的时间。它首先停止RTC，然后分别设置小时、分钟和秒钟，
 * 最后重新启动RTC。
 */
void rtc_run(u8 h, u8 m, u8 s)
{
    // 停止RTC
    Write_Ds1302_Byte(0x8e, 0);
    // 设置小时
    Write_Ds1302_Byte(0x84, h);
    // 设置分钟
    Write_Ds1302_Byte(0x82, m);
    // 设置秒钟
    Write_Ds1302_Byte(0x80, s);
    // 重新启动RTC
    Write_Ds1302_Byte(0x8e, 0x80);
}

/**
 * 读取实时时钟（RTC）的时间
 *
 * 此函数从DS1302芯片读取当前的时间，并将其转换为十进制格式存储在全局变量cj中。
 */
void rtc_proc()
{
    idata u8 h, m, s;
    // 禁用中断
    EA = 0;
    // 读取小时
    h = Read_Ds1302_Byte(0x85);
    // 读取分钟
    m = Read_Ds1302_Byte(0x83);
    // 读取秒钟
    s = Read_Ds1302_Byte(0x81);
    // 恢复中断
    EA = 1;
    
    // 将BCD码转换为十进制并存储在cj中
    cj.h = (h >> 4) * 10 + (h & 0x0f);
    cj.m = (m >> 4) * 10 + (m & 0x0f);
    cj.s = (s >> 4) * 10 + (s & 0x0f);
}


