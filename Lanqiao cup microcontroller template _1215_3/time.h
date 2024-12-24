#ifndef TIM_H
#define TIM_H

#include "all.h"

void Timer1_Init(void);
void Timer0_Init(void);

extern u8 f_on;
extern u8 f_wei;

extern idata unsigned long int uwTick;


#endif 

