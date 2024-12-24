#ifndef TIM_H
#define TIM_H

#include "all.h"

extern u8 f_on;
extern bit f_freq;
extern bit f_20ms_key;
extern bit f_100ms_state;
extern bit f_100ms_adc;
extern bit f_50ms_led;
extern bit f_100ms_wave;

void ne555_init(void);
void Timer2_Init(void);
void Timer1_Init(void);

extern u8 pwm_set;

extern u16 ping_dat;
#endif 
