#include "key.h"
u8 key_code = 0;

u8 getkey22(void)
{
	u8 tmp = 0;
	
	P44=0;P42=1;
	P33=1;P32=1;
	if(!P33)tmp=1;
	if(!P32)tmp+=3;
//	if(!P31)tmp+=5;
//	if(!P30)tmp+=7;
	
	P44=1;P42=0;
	P33=1;P32=1;
	if(!P33)tmp+=5;
	if(!P32)tmp+=7;
	
	return tmp;
}
u8 getkeybtn(void)
{
	u8 tmp=0;
	P33 = P32 = P31 = P30 = 1;
	if(!P33)tmp=1;
	if(!P32)tmp=3;
	if(!P31)tmp=5;
	if(!P30)tmp=7;
	return tmp;
	
}

void key_proc(void)
{
	static u8 k_state = 0;
	u8 k_tmp = 0;
	
	k_tmp = getkey22();//仿真
	//k_tmp=getkeybtn();
	
	switch(k_state)
	{
		case 0:
			if(k_tmp) k_state = 1;
			break;
		case 1:
			if(k_tmp)
			{
				key_code = k_tmp;//按下执行
				if(k_tmp == S4)
				{
					k_state = 3;//特殊按键处理
				}else k_state =2;//普通按键处理
			}
			else k_state = 0;
			break;		
		case 2://普通
			if(!k_tmp)k_state = 0;
		break;
			//
		case 3://特殊
			if(!k_tmp)
			{
				key_code=S4OFF;//松开返回
				k_state = 0;
			}
		break;
			//
	}
}