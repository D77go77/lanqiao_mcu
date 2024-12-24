#include "key.h"
u8 key_code = 0;

u8 getkey22(void)
{
	u8 tmp=0;
	P33=0;P32=1;
	P44=1;P42=1;P35=1;P34=1;
	if(!P35)  tmp=12;
	if(!P34)  tmp=16;
	
	P33=1;P32=0;
	P44=1;P42=1;P35=1;P34=1;
	if(!P35)  tmp=13;
	if(!P34)  tmp=17;
	
	return tmp;
}

void key_proc(void)
{
	static u8 k_state = 0;
	u8 k_tmp = 0;

	k_tmp = getkey22();
	
	
	switch(k_state)
	{
		case 0:
			if(k_tmp) k_state = 1;
			break;
		case 1:
			if(k_tmp)
			{
				key_code = k_tmp;//按下执行
				if(k_tmp == S17)
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
				key_code=S17OFF;//松开返回
				k_state = 0;
			}
		break;
	}
}