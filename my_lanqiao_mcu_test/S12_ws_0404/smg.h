#ifndef __LED8_H__
#define __LED8_H__

#include "all.h"
extern u8 dis_buf[8];
extern u8 dis_str[11];
extern u8 dis_blink[8];

void disp_scan(void);
void disp_tran(void);
#endif
