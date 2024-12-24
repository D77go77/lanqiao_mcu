#ifndef __DS1302_H__
#define __DS1302_H__

#include "all.h"

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;  

void rtc_init(void);
void rtc_run(u8 h,u8 m,u8 s);
unsigned char Read_Ds1302_Byte( unsigned char address );

#endif
