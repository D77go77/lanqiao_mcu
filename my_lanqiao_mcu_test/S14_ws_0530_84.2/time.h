#ifndef __TIME_H__
#define __TIME_H__

#include "all.h"

extern bit f_1s;
extern bit f_20ms_key ;
extern bit f_100ms_adc ;
extern bit f_100ms_state ;
extern bit f_100ms_led;
extern bit f_100ms_rtc ;
extern bit f_200ms_uart ;
extern bit f_800ms_t;
extern bit f_3s;
extern bit f_led;
extern u8 th;
extern u8 tl;

void Timer1_Init(void);		//2ºÁÃë@12.000MHz
void Timer0_Init(void);		//¼ÆÊýÆ÷
void led_mainloop(void);
#endif