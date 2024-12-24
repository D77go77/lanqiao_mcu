#ifndef __LED8_H__
#define __LED8_H__

#include <all.h>
extern u8 disp_blink[8];
extern u8 disp_str[11];
extern u8 disp_buf[8];
void disp_scan();
void disp_tran();

#endif