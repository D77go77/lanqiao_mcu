#include "smg.h"
code unsigned char smg[] =
{
0xc0, //0
0xf9, //1
0xa4, //2
0xb0, //3
0x99, //4
0x92, //5
0x82, //6
0xf8, //7
0x80, //8
0x90, //9
0x88, //A
0x83, //b
0xc6, //C
0xa1, //d
0x86, //E
0x8e //F
};
u8 dis_blink[8]={0,0,0,0,0,0,0,0};
u8 dis_buf[8]={0,0,0,0,0,0,0,0};
u8 dis_str[11];

void disp_scan()
{
	static u8 i=0;
	P0=0;
	gate(WEI);
	P0=dis_buf[i]|dis_blink[i];
	gate(DUAN);
	P0=1<<i;
	gate(WEI);
	if(++i==8)i=0;
}

void disp_proc()
{
	u8 i=0,j=0,tmp=0;
	for(i=0;i<8;i++,j++)
	{
		switch(dis_str[j])
		{
			case '0':tmp=smg[0];break;
			case '1':tmp=smg[1];break;
			case '2':tmp=smg[2];break;
			case '3':tmp=smg[3];break;
			case '4':tmp=smg[4];break;
			case '5':tmp=smg[5];break;
			case '6':tmp=smg[6];break;
			case '7':tmp=smg[7];break;
			case '8':tmp=smg[8];break;
			case '9':tmp=smg[9];break;
			case 'A':tmp=smg[10];break;
			case 'b':tmp=smg[11];break;
			case 'C':tmp=smg[12];break;
			case 'd':tmp=smg[13];break;
			case 'E':tmp=smg[14];break;
			case 'F':tmp=smg[15];break;
			case ' ':tmp = 0xff;break;
			case '*':tmp = 0xfe;break;
			case '-':tmp = 0xbf;break;
			case '_':tmp = 0xf7;break;
			case 'H':tmp = 0x89;break;
			case 'L':tmp = 0xc7;break;
			case 'N':tmp = 0xc8;break;
			case 'P':tmp = 0x8c;break;
			case 'U':tmp = 0xc1;break;
			default:tmp=0xff;break;
		}
		if(dis_str[j+1]=='.')
		{
			tmp&=0x7f;
			j++;
		}
		dis_buf[i]=tmp;
	}

}












