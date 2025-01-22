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

idata u8 dis_buf[8]={0,0,0,0,0,0,0,0};
idata u8 dis_str[11];
//**************************************************************//
void disp_scan()
{
    static idata u8 i=0;                                // 使用静态变量 i 记忆当前扫描位置
    P0=0;                                         // 关闭所有数码管
    gate(WEI);                                    // 设置位选信号
    P0=dis_buf[i];                                // 显示当前位数码管的数据
    gate(DUAN);                                   // 设置段选信号
    P0=1<<i;                                      // 选择当前位数码管
    gate(WEI);                                    // 设置位选信号
    if(++i==8)i=0;                                // 更新扫描位置，若达到最大值则重置为0
}
//**************************************************************//
void disp_proc()
{
    idata u8 i,j,tmp=0;                                 // 初始化索引变量和临时变量
    for(i=0,j=0;i<8;i++,j++)                          // 循环处理每一位数码管
    {
        switch(dis_str[j])                          // 根据字符设置对应的数码管显示值
        {
            case '0':tmp=smg[0];break;              // 字符 '0' 对应的数码管显示值
            case '1':tmp=smg[1];break;              // 字符 '1' 对应的数码管显示值
            case '2':tmp=smg[2];break;              // 字符 '2' 对应的数码管显示值
            case '3':tmp=smg[3];break;              // 字符 '3' 对应的数码管显示值
            case '4':tmp=smg[4];break;              // 字符 '4' 对应的数码管显示值
            case '5':tmp=smg[5];break;              // 字符 '5' 对应的数码管显示值
            case '6':tmp=smg[6];break;              // 字符 '6' 对应的数码管显示值
            case '7':tmp=smg[7];break;              // 字符 '7' 对应的数码管显示值
            case '8':tmp=smg[8];break;              // 字符 '8' 对应的数码管显示值
            case '9':tmp=smg[9];break;              // 字符 '9' 对应的数码管显示值
            case 'A':tmp=smg[10];break;             // 字符 'A' 对应的数码管显示值
            case 'b':tmp=smg[11];break;             // 字符 'b' 对应的数码管显示值
            case 'C':tmp=smg[12];break;             // 字符 'C' 对应的数码管显示值
            case 'd':tmp=smg[13];break;             // 字符 'd' 对应的数码管显示值
            case 'E':tmp=smg[14];break;             // 字符 'E' 对应的数码管显示值
            case 'F':tmp=smg[15];break;             // 字符 'F' 对应的数码管显示值
            case '-':tmp=0xbf;break;                // 字符 '-' 对应的数码管显示值
            default:tmp=0xff;break;                 // 默认情况下的数码管显示值
        }
        if(dis_str[j+1]=='.')                       // 检查下一个字符是否为小数点
        {
            tmp&=0x7f;                              // 如果有小数点，则点亮小数点
            j++;                                    // 跳过小数点字符
        }
        dis_buf[i]=tmp;                             // 将处理后的值存入显示缓冲区
    }
}
//**************************************************************//
idata bit f_wei=0;//smg闪烁标志
void smg_proc()
{
    switch(cj.state)                                // 根据状态选择不同的显示格式
    {
        case 10:
            if(f_wei)//数码管闪烁
                sprintf(dis_str,"F%02bu-%02bubb",cj.m,cj.s); // 格式化字符串，显示频率和时间
            else
                sprintf(dis_str,"   -%02bubb",cj.s);         // 格式化字符串，显示时间
            break;
        case 20:
//            if(f_wei)
                sprintf(dis_str,"d    %3bu",cj.key);          // 格式化字符串，显示按键值
//            else
//                sprintf(dis_str,"E  %5u",cj.freq);           // 格式化字符串，显示频率
            break;
        case 30:sprintf(dis_str,"b%3.1f %3bu",cj.t,cj.d);break; // 格式化字符串，显示温度和距离
        case 40:sprintf(dis_str,"A    %3bu",cj.ad);break;     // 格式化字符串，显示AD值
    }
    disp_proc();                                    // 调用 disp_proc 函数进行显示处理
}







