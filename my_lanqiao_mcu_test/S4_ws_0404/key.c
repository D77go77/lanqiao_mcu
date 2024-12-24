#include "key.h"
u8 key_code = 0;

u8 getkey22(void)
{
	u8 tmp=0;

	P32=0;P33=1;
	P42=1;P44=1;
	if(!P42)
		tmp= 7;
	if(!P44)
		tmp =3;
	
	P32=1;P33=0;
	P42=1;P44=1;
	if(!P42)
		tmp =5;
	if(!P44)
		tmp =1;

	return tmp;
}
//***************************
u8 getkeybtn(void)
{
	u8 tmp=0;
	P33=P32=P31=P30=1;
	if(!P33)tmp=1;
	if(!P32)tmp=3;
	if(!P31)tmp=5;
	if(!P30)tmp=7;
	return tmp;
}
//****************************
void key_proc(void)
{
	static u8 k_state = 0;//按键状态
	u8 k_tmp = 0;
	
	
	//k_tmp = getkey22(); 
	k_tmp=getkeybtn();
	
	switch (k_state)
	{
		case 0:
			if (k_tmp) k_state = 1;
			break;	
		
		case 1://去抖动
			if (k_tmp) // 还有按键
			{
				key_code=k_tmp;
				k_state = 2;
			}
			else
				k_state = 0;
			break;
			
		case 2://普通按键
			if(!k_tmp)//松开返回键码
			{
				k_state=0;
			}
			break;
							
	}	
}