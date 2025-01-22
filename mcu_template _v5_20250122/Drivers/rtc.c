/*	# 	DS1302����Ƭ��˵��
	1. 	���ļ������ṩ���������빩����ѡ����ɳ�����Ʋο���
	2. 	����ѡ�ֿ������б�д��ش�����Ըô���Ϊ������������ѡ��Ƭ�����͡������ٶȺ�����
		�жԵ�Ƭ��ʱ��Ƶ�ʵ�Ҫ�󣬽��д�����Ժ��޸ġ�
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
 * ����ʵʱʱ�ӣ�RTC����ʱ��
 *
 * @param h Сʱ����Χ0-23
 * @param m ���ӣ���Χ0-59
 * @param s ���ӣ���Χ0-59
 *
 * �˺���ͨ��д��DS1302оƬ������RTC��ʱ�䡣������ֹͣRTC��Ȼ��ֱ�����Сʱ�����Ӻ����ӣ�
 * �����������RTC��
 */
void rtc_run(u8 h, u8 m, u8 s)
{
    // ֹͣRTC
    Write_Ds1302_Byte(0x8e, 0);
    // ����Сʱ
    Write_Ds1302_Byte(0x84, h);
    // ���÷���
    Write_Ds1302_Byte(0x82, m);
    // ��������
    Write_Ds1302_Byte(0x80, s);
    // ��������RTC
    Write_Ds1302_Byte(0x8e, 0x80);
}

/**
 * ��ȡʵʱʱ�ӣ�RTC����ʱ��
 *
 * �˺�����DS1302оƬ��ȡ��ǰ��ʱ�䣬������ת��Ϊʮ���Ƹ�ʽ�洢��ȫ�ֱ���cj�С�
 */
void rtc_proc()
{
    idata u8 h, m, s;
    // �����ж�
    EA = 0;
    // ��ȡСʱ
    h = Read_Ds1302_Byte(0x85);
    // ��ȡ����
    m = Read_Ds1302_Byte(0x83);
    // ��ȡ����
    s = Read_Ds1302_Byte(0x81);
    // �ָ��ж�
    EA = 1;
    
    // ��BCD��ת��Ϊʮ���Ʋ��洢��cj��
    cj.h = (h >> 4) * 10 + (h & 0x0f);
    cj.m = (m >> 4) * 10 + (m & 0x0f);
    cj.s = (s >> 4) * 10 + (s & 0x0f);
}


