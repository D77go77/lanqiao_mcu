#include <led8.h>
u8 disp_blink[8]={0,0,0,0,0,0,0,0};
u8 disp_buf[8]={0,0,0,0,0,0,0,0};
u8 disp_str[11]= {0};
code unsigned char Seg_Table[] ;
void disp_scan()
{
	static u8 i =0;
	
	P0 =0;
	gate(W8);
	
	P0 = disp_buf[i] |disp_blink[i];
	gate(D8);
	
	P0 = 1<<i;
	gate(W8);
	
	if(++i>=8) i=0;
	
}

code unsigned char Seg_Table[] = 
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
0xbf,  //-
0xC1, //U
0x8c, //P
0xC8  //N
};

void disp_tran()
{
	u8 i,j;
	u8 temp =0;
	
	for(i=0;i<8;i++,j++)
	{
		switch(disp_str[j])
		{
			case '0':temp = Seg_Table[0];break;
			case '1':temp = Seg_Table[1];break;
			case '2':temp = Seg_Table[2];break;
			case '3':temp = Seg_Table[3];break;
			case '4':temp = Seg_Table[4];break;
			case '5':temp = Seg_Table[5];break;
			case '6':temp = Seg_Table[6];break;
			case '7':temp = Seg_Table[7];break;
			case '8':temp = Seg_Table[8];break;
			case '9':temp = Seg_Table[9];break;
			case 'A':temp = Seg_Table[10];break;
			case 'b':temp = Seg_Table[11];break;
			case 'C':temp = Seg_Table[12];break;
			case 'd':temp = Seg_Table[13];break;
			case 'E':temp = Seg_Table[14];break;
			case 'F':temp = Seg_Table[15];break;
			case '-':temp = Seg_Table[16];break;
			case 'U':temp = Seg_Table[17];break;
			case 'P':temp = Seg_Table[18];break;
			case 'N':temp = Seg_Table[19];break;
			case ' ':temp = 0xff;break;
			default:temp = 0xff;break;
		
		}
		if(disp_str[j+1]=='.')
		{
			temp &= 0x7f;
			j++;
		}
		disp_buf[i] = temp;
	}
}