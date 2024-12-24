#ifndef __ONEWIRE_H__ 
#define __ONEWIRE_H__

#include"all.h"

sbit DQ=P1^4;
void start_ds18b20(void);
u16 read_tem16(void); 

#endif