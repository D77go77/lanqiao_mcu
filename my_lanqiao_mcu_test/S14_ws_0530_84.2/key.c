#include "key.h"
u8 key_code = 0;

//������ߵľ���2X2����
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
	static u8 k_state = 0;//����״̬
	static u8 _key=0;      //�����ݴ�ֵ	
	static u8 _cnt=0; 
	u8 k_tmp = 0;
	
	
	k_tmp = getkey22(); 
	
	switch (k_state)
	{
		case 0:
			if (k_tmp) k_state = 1;
			break;	
		
		case 1://ȥ����
			if (k_tmp) // ���а���
			{
				_key=k_tmp;//����
				if(_key==S9)//���ܳ���
				{
					_cnt=0;k_state=3;
				}
				else//��ͨ����
					k_state = 2;
			}
			else
				k_state = 0;
			break;
			
		case 2://��ͨ����
			if(!k_tmp)//�ɿ����ؼ���
			{
				key_code=_key;
				k_state=0;
			}
			break;
			
		case 3://������
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
			if(!k_tmp)//�ɿ����ؼ���
			{
				key_code=S9L;
				k_state=0;
			}
			break;						
	}	
}