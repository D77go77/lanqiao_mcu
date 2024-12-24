#include "key.h"

u8 key_code=0;
//**************************************
u8 getkeybtn()
{	
	u8 tmp = 0;
	//P30=1;P31=1;P32=1;P33=1;
	if(!P30)tmp=7;
	if(!P31)tmp=6;
	if(!P32)tmp=5;
	if(!P33)tmp=4;
	
  return tmp;
}
//***********
u8 getkey22()
{
    u8 key=0;
    
    P32=0;P33=1;
    P42=1;P44=1;
    if(!P42)key=6;
    if(!P44)key=5;
    
    P32=1;P33=0;
    P42=1;P44=1;
    if(!P42)key=7;
    if(!P44)key=4;

    return key;
}
//**************************************
void key_proc()
{
	static u8 k_state = 0;
	static u16 k_tim=0;
	static u8 _key=0;
	u8 k_tmp = 0;
	
	//k_tmp = getkey22(); 
	k_tmp = getkeybtn(); 
	
	switch (k_state)
	{
		case 0:
			if (k_tmp)
				k_state = 1;
			break;		
		//	
		case 1:
			if (k_tmp) // 还有按键
				k_state = 2;
			else
				k_state = 0;
			break;
		//	
		case 2://单 判断特殊
			if (k_tmp)	
			{
				_key = k_tmp;
				if(_key == 7)//可能长按
				{
					k_state = 3;
					k_tim = 0;
				}else 
				{
					k_state = 4;
				}
			}
			else//按键松开
			{
				k_state = 0;		
			}

			break;
		// 	
		case 3://长按
			if(k_tmp)
			{
				if(++k_tim >= 50)
				{
					k_state = 30;
				}
			}
			else
			{
				k_state = 0;		
				key_code = 7;	
			}
			break;
		//	
		case 30://长释放
			if(!k_tmp)
			{
				k_state = 0;
				key_code = S77;//松开后执行
			}
			break;
		//
		case 4://普通释放
			if(!k_tmp) 
			{
				k_state = 0;
				key_code = _key;//单键按下执行
			}
			break;
	}
}