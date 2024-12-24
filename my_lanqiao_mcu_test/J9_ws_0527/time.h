#ifndef TIM_H
#define TIM_H

#include "all.h"


extern bit f_20ms_key;
extern bit f_100ms_state;
extern bit f_100ms_adc;
extern bit f_50ms_led;
extern bit f_100ms_freq;
extern bit f_800ms_tem;

void ne555_init(void);
void Timer2_Init(void);
void Timer1_Init(void);

extern u16 ping_dat;
#endif 
