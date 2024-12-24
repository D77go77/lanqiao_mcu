/*	# 	�����ߴ���Ƭ��˵��
	1. 	���ļ������ṩ���������빩����ѡ����ɳ�����Ʋο���
	2. 	����ѡ�ֿ������б�д��ش�����Ըô���Ϊ������������ѡ��Ƭ�����͡������ٶȺ�����
		�жԵ�Ƭ��ʱ��Ƶ�ʵ�Ҫ�󣬽��д�����Ժ��޸ġ�
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
    init_ds18b20(); // ��ʼ��DS18B20�¶ȴ�����
    Write_DS18B20(0xcc); // ��������ROM�������ȡ�ض������������к�
    Write_DS18B20(0x44); // ����ת���¶������ʼ�¶�ת��
}

float read_temp()
{
    u8 H,L; // ��������H��L���ڴ洢�¶����ݵĸߵ��ֽ�
    float tmp; // ��������tmp���ڴ洢����õ����¶�ֵ
    init_ds18b20(); // ��ʼ��DS18B20�¶ȴ�����
    Write_DS18B20(0xcc); // ��������ROM�������ȡ�ض������������к�
    Write_DS18B20(0xbe); // ���Ͷ�ȡ�¶������ʼ��ȡ�¶�����
    L=Read_DS18B20(); // ��ȡ�¶����ݵĵ��ֽ�
    H=Read_DS18B20(); // ��ȡ�¶����ݵĸ��ֽ�
    init_temp(); // �ٴγ�ʼ���¶ȴ�������׼����һ���¶�ת��
    tmp=(u32)H<<8|L; // �����ֽں͵��ֽ���ϳ��������¶�����
    return tmp*0.0625f; // ���¶�����ת��Ϊʵ���¶�ֵ������
}

void temp_proc()
{
    cj.t=read_temp(); // ����ȫ�ֱ���cj.tΪ��ǰ�¶�ֵ
}

