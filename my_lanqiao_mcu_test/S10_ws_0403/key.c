#include "key.h"
u8 key_code = 10;

u8 getkey24(void)//仿真
{
	u8 tmp=10;
	
	P34=1; P35=1; P42=1; P44=1;
	
	P32=0; P33=1;
	if(!P44)tmp=8;
	if(!P42)tmp=9;
//	if(!P35)tmp=13;
//	if(!P34)tmp=17;
	
	P32=1; P33=0;
	if(!P44)tmp=1;
	if(!P42)tmp=11;
	if(!P35)tmp=12;
	if(!P34)tmp=16;

	
	return tmp;
}

u8 getkey44(void)//真实
{
	u8 tmp=10;
	
	P34=1; P35=1; P42=1; P44=1;
	
	P30=0; P31=1; P32=1; P33=1;
	if(!P44)tmp=0;
	if(!P42)tmp=1;
	if(!P35)tmp=2;
	if(!P34)tmp=3;
	
	P30=1; P31=0; P32=1; P33=1;
	if(!P44)tmp=4;
	if(!P42)tmp=5;
	if(!P35)tmp=6;
	if(!P34)tmp=7;
	
	P30=1; P31=1; P32=0; P33=1;
	if(!P44)tmp=8;
	if(!P42)tmp=9;
//	if(!P35)tmp=13;
//	if(!P34)tmp=17;
	
	P30=1; P31=1; P32=1; P33=0;
//	if(!P44)tmp=0;
	if(!P42)tmp=11;
	if(!P35)tmp=12;
	if(!P34)tmp=16;
	
	return tmp;
}
void key_proc(void)
{
	static u8 k_state = 0;
	static u8 _key = 0;
	static u8 _cnt = 0;
	u8 k_tmp = 0;
	
//	k_tmp = getkey24();
	k_tmp = getkey44();
	
	switch(k_state)
	{
		case 0:
			if(k_tmp!=10) k_state = 1;
			break;
		case 1:
			if(k_tmp!=10)
			{
				_key=k_tmp;//保存
				if(_key==S8)//可能长按
				{
					_cnt=0;k_state=3;
				}else //普通按键
				{
					k_state = 2;	
					key_code = _key;
				}

			}else k_state = 0;
			break;		
		case 2://普通按键
			if(k_tmp==10)
			{
				k_state = 0;
			}
			break;
		case 3://特殊长按
			if(k_tmp !=10)
			{
				if(++_cnt>=100)
				{
					k_state=4;
					key_code=S8L;
				}
			}
			else
			{
				key_code=_key;
				k_state=0;			
			}
		break;
		case 4://特殊长按
			if(k_tmp==10)//松开返回键码
			{
				k_state=0;
			}
		break;
	}
}