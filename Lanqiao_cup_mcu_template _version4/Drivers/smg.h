#ifndef SMG_H
#define SMG_H

#include "all.h"

extern u8 dis_buf[8];
extern u8 dis_blink[8];
extern u8 dis_str[11];

void disp_scan();
void disp_proc();

void smg_proc();

extern u8 f_wei;

#endif 


