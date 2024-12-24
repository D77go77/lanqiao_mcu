#include <key.h>

u8 key_code =0;

u8 get_key34()
{
	u8 temp =0;
	
	P30 =0;P31 =1;P32 =1;P33 =1;
	P44 =1;P42 =1;P35 =1;
	if(!P44) temp +=10;
	if(!P42) temp +=1;
	if(!P35) temp +=2;
		
	P30 =1;P31 =0;P32 =1;P33 =1;
	P44 =1;P42 =1;P35 =1;
	if(!P44) temp +=3;
	if(!P42) temp +=4;
	if(!P35) temp +=5;
	
	P30 =1;P31 =1;P32 =0;P33 =1;
	P44 =1;P42 =1;P35 =1;
	if(!P44) temp +=6;
	if(!P42) temp +=7;
	if(!P35) temp +=8;
	
	P30 =1;P31 =1;P32 =1;P33 =0;
	P44 =1;P42 =1;P35 =1;
	if(!P44) temp +=9;
	if(!P42) temp +=11;
	if(!P35) temp +=12;
	
	return temp;
}

u8 get_key24()
{
	u8 temp =0;
	
	P32 =0;P33 =1;
	P44 =1;P42 =1;P35 =1;P34 =1;
	if(!P44) temp +=2;
	if(!P42) temp +=3;
	if(!P35) temp +=8;
	if(!P34) temp +=10;
	
	P32 =1;P33 =0;
	P44 =1;P42 =1;P35 =1;P34 =1;
	if(!P44) temp +=9;
	if(!P42) temp +=11;
	if(!P35) temp +=12;
	if(!P34) temp +=1;
	
	return temp;
}

void key_proc()
{
	static u8 key_state =0;
	u8 key_temp =0;
	
//	key_temp = get_key24();
	key_temp = get_key34();
	
	switch(key_state)
	{
		case 0:
				if(key_temp)
			{
				key_state =1;
			}
			break;
			
		case 1:
			if(key_temp )
			{
				key_code = key_temp;				
				key_state =2;
			}else key_state =0;
			break;
			
		case 2:
				if(!key_temp )
				{
					key_state =0;
				}
			break;
	}
}
