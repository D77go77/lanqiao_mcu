#include "led8.h"
#define DP 0x7f
code unsigned char seg[] = 
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
0x8e, //F
0xff,	//Ãð
0xfe,	//'*' ÉÏºáa 1111_1110			
0xbf,	// '-'ÖÐºág 1011_1111
0xf7,	//'_' ÏÂºád 1111_0111 
0x89, //H
0xc7, //L
0xc8, //N
0x8c, //P
0xc1,	//U
};

u8 dis_blink[8]={0,0,0,0,0,0,0,0};
u8 dis_buf[8]={0,0,0,0,0,0,0,0};
u8 dis_str[11];
//*********************************************
void disp_tran(void);
void disp_scan(void);
//**********************************************
void disp_scan(void)
{
	static u8 i=0;
 	
	P0=0;
	gate(W8);
	
	P0=dis_buf[i] | dis_blink[i];
	gate(D8);	
	
	P0=1<<i;
	gate(W8);
	
	if(++i==8)i=0;
}
//**********************************************
void disp_tran(void)	
{
	u8 i = 0;
	u8 j = 0;
	u8 temp;
	for (i = 0; i <= 7; i++, j++)
	{
		switch (dis_str[j])
		{
		case '0':temp = seg[0];break;
		case '1':temp = seg[1];break;
		case '2':temp = seg[2];break;
		case '3':temp = seg[3];break;
		case '4':temp = seg[4];break;
		case '5':temp = seg[5];break;
		case '6':temp = seg[6];break;
		case '7':temp = seg[7];break;
		case '8':temp = seg[8];break;
		case '9':temp = seg[9];break;
		case 'A':temp = seg[10];break;
		case 'B':temp = seg[11];break;
		case 'C':temp = seg[12];break;
		case 'D':temp = seg[13];break;
		case 'E':temp = seg[14];break;
		case 'F':temp = seg[15];break;
		case ' ':temp = seg[16];break;
		case '*':temp = seg[17];break;		
		case '-':temp = seg[18];break;
		case '_':temp = seg[19];break;
		case 'H':temp = seg[20];break;
		case 'L':temp = seg[21];break;
		case 'N':temp = seg[22];break;
		case 'P':temp = seg[23];break;
		case 'U':temp = seg[24];break;
		default: temp = 0xff;break;
		}
		if (dis_str[j + 1] == '.')
		{
			temp &= DP;
			j++;
		}
		dis_buf[i] = temp;
	}
}