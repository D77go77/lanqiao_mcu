#ifndef __ONEWIRE_H__ 
#define __ONEWIRE_H__

#include"all.h"
sbit DQ=P1^4;
void init_temp(void);
float read_tem(void); 
#endif