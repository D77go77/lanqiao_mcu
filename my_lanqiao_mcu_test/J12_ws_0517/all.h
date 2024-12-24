#ifndef ALL__
#define ALL__

#include <STC15F2K60S2.H>
#include "stdio.h"
#include "intrins.h"

typedef unsigned char u8;
typedef unsigned int u16;

#define CTRL 0xa0
#define LED 0x80
#define WEI 0xc0
#define DUAN 0xe0
#define CLOSE() P2&=0x1f
#define OPEN(a) P2|=a

#define FMQ 1<<6
#define JDQ 1<<4
#define MOTO 1<<5

#include "time.h"
#include "key.h"
#include "smg.h"
#include "wave.h"
#include "iic.h"
#include "ctrl.h"
#include "ds1302.h"







xdata struct caiji
{
	u16 d_dat;//距离数据
	u16 d_all;//数据总和
	u16 d_max;//数据总和最大值
	float d_aver;//平均值
	u16 d_min;//最小值
	u16 d_set;//距离参数  10~80    20
	u16 set_tmp;//参数缓存值
	int d_cnt;//测量成功次数
	int d_tim;//采集时间  2 3 5 7 9   2
	int tim_tmp;//缓存值
	u8 mode;//数据切换  1最大，2最小，3平均
	u8 f_cf;//触发切换定时
	u8 cf_3;//连续3次采集
	u8 adc;//now_adc
	u8 hour,min,sec;//时钟
};

extern xdata struct caiji cj;

extern u8 bdata led_dat;


#endif 
