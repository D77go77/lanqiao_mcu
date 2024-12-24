#include "key.h"

u8 key_code = 0;
//
u8 getkey22(void)
{
	u8 temp = 0;
	
	P32 = 0;P33 = 1;
	P42 = 1;P44 = 1;
	if(!P42) temp+=9;
	if(!P44) temp+=5;	
	
	P32 = 1;P33 = 0;
	P42 = 1;P44 = 1;
	if(!P42) temp+=8;
	if(!P44) temp+=4;
	
	return temp;
}

void key_proc(void)
{
	static u8 key_state = 0;
	static u8 _key = 0;
	
	u8 key_temp = 0;
	
	key_temp = getkey22();
	
	switch(key_state)
	{
		case 0:
			if(key_temp)  key_state = 1;
			break;
		//
		case 1:
			if(key_temp)
			{
				key_state = 2;
			}else
			{
				key_state = 0;
			}
			break;
		//
		case 2:
			if(key_temp)//µ¥»÷
			{
				_key = key_temp;
				key_code = _key;
				key_state = 3;
				//
			}else//ËÉ¿ª
			{
				key_state = 0;
			}
			break;
		//
		case 3:
			if(!key_temp)   key_state = 0;
		break;	
	}
}

