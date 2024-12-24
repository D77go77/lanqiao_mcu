#include "key.h"
u8 key_code = 0;

//下面左边的矩阵2X2按键
unsigned char getkey22(void)
{
	u8 key;
	
	P44=1; P42=1; 
	
	P32=0; P33=1;
	if(!P44) key=5;
	if(!P42) key=9;
	
	P32=1; P33=0;
	if(!P44) key=4;
	if(!P42) key=8;
	
	return key;
}

void key_proc(void)
{
	static u8 k_state = 0;//按键状态
	static u8 _key=0;      //按键暂存值	
	static u8 _cnt=0; 
	u8 k_tmp = 0;
	
	
	k_tmp = getkey22(); 
	
	switch (k_state)
	{
		case 0:
			if (k_tmp) k_state = 1;
			break;	
		
		case 1://去抖动
			if (k_tmp) // 还有按键
			{
				_key=k_tmp;//保存
				if(_key==S9)//可能长按
				{
					_cnt=0;k_state=3;
				}
				else//普通按键
					k_state = 2;
			}
			else
				k_state = 0;
			break;
			
		case 2://普通按键
			if(!k_tmp)//松开返回键码
			{
				key_code=_key;
				k_state=0;
			}
			break;
			
		case 3://长按键
			if(k_tmp)
			{
				if(++_cnt>=100)k_state=4;
			}
			else
			{
				key_code=_key;
				k_state=0;			
			}
			break;
			
		case 4:
			if(!k_tmp)//松开返回键码
			{
				key_code=S9L;
				k_state=0;
			}
			break;						
	}	
}