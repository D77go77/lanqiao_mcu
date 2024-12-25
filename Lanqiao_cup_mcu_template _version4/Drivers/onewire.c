/*	# 	单总线代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/
#include "onewire.h"
//
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}

//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}


void init_temp()
{
    init_ds18b20(); // 初始化DS18B20温度传感器
    Write_DS18B20(0xcc); // 发送跳过ROM命令，不读取特定传感器的序列号
    Write_DS18B20(0x44); // 发送转换温度命令，开始温度转换
}

float read_temp()
{
    u8 H,L; // 声明变量H和L用于存储温度数据的高低字节
    float tmp; // 声明变量tmp用于存储计算得到的温度值
    init_ds18b20(); // 初始化DS18B20温度传感器
    Write_DS18B20(0xcc); // 发送跳过ROM命令，不读取特定传感器的序列号
    Write_DS18B20(0xbe); // 发送读取温度命令，开始读取温度数据
    L=Read_DS18B20(); // 读取温度数据的低字节
    H=Read_DS18B20(); // 读取温度数据的高字节
    init_temp(); // 再次初始化温度传感器，准备下一次温度转换
    tmp=(u32)H<<8|L; // 将高字节和低字节组合成完整的温度数据
    return tmp*0.0625f; // 将温度数据转换为实际温度值并返回
}

void temp_proc()
{
    cj.t=read_temp(); // 更新全局变量cj.t为当前温度值
}

