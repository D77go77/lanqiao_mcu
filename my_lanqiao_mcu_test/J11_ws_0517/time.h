#ifndef TIM_H
#define TIM_H

#include "all.h"

extern bit f_20ms_key;
extern bit f_100ms_state;
extern bit f_100ms_rtc;
extern bit f_100ms_adc;
extern bit f_100ms_led;
extern bit f_600ms_temp;


void Timer1_Init(void);


#endif 