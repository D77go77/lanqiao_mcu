#include "key.h"
u8 key_code = 0;

u8 getkey22(void)
{
	u8 tmp=0;
	
	P35=1;P34=1;
	P33=0;P32=1;
	if(!P35)tmp=12;
	if(!P34)tmp=16;	
	
	P35=1;P34=1;
	P33=1;P32=0;	
	if(!P35)tmp=13;
	if(!P34)tmp=17;	

	return tmp;
}

void key_proc(void)
{
	static u8 k_state = 0;
	static u8 _key=0;      
	static u8 _cnt=0; 
	u8 k_tmp = 0;
	
	k_tmp = getkey22();	
	
	switch (k_state)
	{
		case 0:
			if (k_tmp) k_state = 1;
			break;	
		
		case 1:
			if (k_tmp)
			{
				_key=k_tmp;
				if(_key==13)//���ܳ���
				{
					_cnt=0;k_state=3;
				}
				else if(_key==12)
				{
					_cnt=0;k_state=5;
				}
				else//�ռ�
					k_state = 2;
			}
			else
				k_state = 0;
			break;
			
		case 2://�ռ�
			if(!k_tmp)//�ɿ�����
			{
				key_code=_key;
				k_state=0;
			}
			break;
			
		case 3://������ S13L
			if(k_tmp)
			{
				if(++_cnt>=50)
				{
					k_state=4;//1s				
					key_code=S13L;
				}
			}
			else
			{
				key_code=_key;
				k_state=0;			
			}
			break;
			
		case 4:
			if(!k_tmp)k_state=0;
			break;	
			//
		case 5://������S12L
			if(k_tmp)
			{
				if(++_cnt>=50)
				{
					k_state=4;//1s				
					key_code=S12L;
				}
			}
			else
			{
				key_code=_key;
				k_state=0;			
			}
			break;			
	}	
}