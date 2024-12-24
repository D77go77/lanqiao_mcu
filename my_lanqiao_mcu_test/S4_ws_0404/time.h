#ifndef _TIME_H
#define _TIME_H_

#include "all.h"
extern bit f_1s;
extern bit f_20ms_key ;
extern bit f_100ms_adc ;
extern bit f_100ms_state ;
extern bit f_100ms_led;
extern bit f_100ms_rtc;
extern bit f_1s_blink8;
void Timer1_Init(void);
void Timer2_Init(void);

#endif 
