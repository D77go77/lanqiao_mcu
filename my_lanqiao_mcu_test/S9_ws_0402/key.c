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
	
	k_tmp = getkey22();
	//k_tmp=getkeybtn();
	
	switch(k_state)
	{
		case 0:
			if(k_tmp) k_state = 1;
			break;
		case 1:
			if(k_tmp)
			{
				k_state = 2;
				if(k_tmp == S4)
				{
					k_state = 3;//特殊按键处理
				}else k_state =2;
			}else k_state = 0;
			break;		
		case 2:
			if(k_tmp)
			{
				
			}else k_state=0;
		break;
		case 4:
			if(!k_tmp)
			{
				key_code = 0;//松开重置
				k_state = 0;
			}
			break;
		case 3:
			if(k_tmp)
			{
				key_code=S4L;
			}else k_state = 0;
	}
}