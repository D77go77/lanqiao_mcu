#include "key.h"
u8 key_code = 0;

u8 getkey22(void)
{
	u8 tmp=0;
	P33=0;P32=1;
	P44=1;P42=1;P35=1;P34=1;
	if(!P35)  tmp=4;
	if(!P34)  tmp=5;
	
	P33=1;P32=0;
	P44=1;P42=1;P35=1;P34=1;
	if(!P35)  tmp=6;
	if(!P34)  tmp=7;
	
	return tmp;
}
u8 getkeybtn(void)
{
	u8 tmp=0;
	if(!P33)tmp=4;
	if(!P32)tmp=5;
	if(!P31)tmp=6;
	if(!P30)tmp=7;
	return tmp;
}

void key_proc(void)
{
	static u8 k_state = 0;
	u8 k_tmp = 0;

	//k_tmp = getkey22();
	k_tmp=getkeybtn();
	
	switch(k_state)
	{
		case 0:
			if(k_tmp) k_state = 1;
			break;
		case 1:
			if(k_tmp)
			{
				k_state = 2;
				key_code = k_tmp;
			}else k_state = 0;
			break;		
		case 2:
			if(!k_tmp)
			{
				key_code = 0;//ËÉ¿ªÖØÖÃ
				k_state = 0;
			}
			break;
	}
}