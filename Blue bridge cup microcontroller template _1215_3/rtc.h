#ifndef RTC_H
#define RTC_H

#include "all.h"

sbit SCK = P1^7;
sbit SDA = P2^3;
sbit RST = P1^3;

void rtc_run(u8 h,u8 m,u8 s);

void rtc_proc();

#endif 
