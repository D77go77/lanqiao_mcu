#include "wave.h"
void de(u8 a);
float read_d()
{
	u16 d;
	u8 num=16;
	
	TX=0;
	while(num--)
	{
		de(12);
		TX=!TX;
	}
	CH=CL=0;
	CR=1;
	while( RX&& CH<0x20 );//�޶�dֵ��Χ0-5888������෶Χ0-100cm

	CR=0;
	
	d=((u16)CH<<8|CL)+156;//156����У׼���������
	return d;
}

void de(u8 a)
{
	while(a--);
}

