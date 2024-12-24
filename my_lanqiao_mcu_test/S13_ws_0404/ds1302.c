/*	# 	DS1302����Ƭ��˵��
	1. 	���ļ������ṩ���������빩����ѡ����ɳ�����Ʋο���
	2. 	����ѡ�ֿ������б�д��ش�����Ըô���Ϊ������������ѡ��Ƭ�����͡������ٶȺ�����
		�жԵ�Ƭ��ʱ��Ƶ�ʵ�Ҫ�󣬽��д�����Ժ��޸ġ�
*/								
 
#include "ds1302.h"

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

void ds1302_init(void)
{
	Write_Ds1302_Byte(0x8e,0x00);
	Write_Ds1302_Byte(0x8c,0x24);
	Write_Ds1302_Byte(0x8a,0x03);
	Write_Ds1302_Byte(0x88,0x03);
	Write_Ds1302_Byte(0x86,0x27);
	Write_Ds1302_Byte(0x84,0);
	Write_Ds1302_Byte(0x82,0);
	Write_Ds1302_Byte(0x80,0);
	Write_Ds1302_Byte(0x8e,0x80);
}
void ds1302_run(u8 h,u8 m,u8 s)
{
	Write_Ds1302_Byte(0x8e,0x00);
	Write_Ds1302_Byte(0x84,h);
	Write_Ds1302_Byte(0x82,m);
	Write_Ds1302_Byte(0x80,s);
	Write_Ds1302_Byte(0x8e,0x80);
}
 