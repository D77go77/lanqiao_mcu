#include "key.h"

u8 key_code=0;

//**************************************
u8 getkey22()
{
	u8 tmp=0;
	
	P44=0;P42=1;
	P35=1;P34=1;
	if(!P33)tmp+=4;
	if(!P32)tmp+=5;
	P44=1;P42=0;
	P35=1;P34=1;
	if(!P33)tmp+=8;
	if(!P32)tmp+=9;
	return tmp;
}
//**************************************
void key_proc(void)
{
	static u8 k_state = 0;//����״̬
	static u8 _key=0;      //�����ݴ�ֵ	
	static u8 _cnt=0; 
	u8 k_tmp = 0;
	
	
	//k_tmp = getkey14(); 
	k_tmp = getkey22(); 
	
	switch (k_state)
	{
		case 0:
			if (k_tmp) k_state = 1;
			break;	
		
		case 1://ȥ����
			if (k_tmp) // ���а���
				k_state = 2;
			else
				k_state = 0;
			break;	
			
		case 2://��������
			if (k_tmp) // ���а���
			{
				_key=k_tmp;//����
				if(_key==S89)//���ܳ���
				{
					_cnt=0;k_state=3;
				}
			}
			else
			{
				k_state = 0;
				key_code=_key;				
			}
			break;
			
			
		case 3://˫��S89
			if(k_tmp)
			{
				if(k_tmp!=S89)//���뱣֤�Ƕ�Ӧ��˫��������ʱ����0
					_cnt=0;
				else if(++_cnt>=100)
					{k_state=4;key_code=S89;}
			}
			else//˫��ʱ�䲻�������ܲ�������
			{
				k_state=0;			
			}
			break;
			
		case 4:
			if(!k_tmp)
			{				
				k_state=0;
			}
			break;					
	}	
}