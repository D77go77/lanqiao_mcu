/*	#   I2C代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/
#include "iic.h"

#define DELAY_TIME	10
void Delay5ms();
//
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();		
    }
    while(n--);      	
}

//
void I2CStart(void)
{
    sda = 1;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 0;
	I2C_Delay(DELAY_TIME);
    scl = 0;    
}

//
void I2CStop(void)
{
    sda = 0;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 1;
	I2C_Delay(DELAY_TIME);
}

//
void I2CSendByte(unsigned char byt)
{
    unsigned char i;
	
    for(i=0; i<8; i++){
        scl = 0;
		I2C_Delay(DELAY_TIME);
        if(byt & 0x80){
            sda = 1;
        }
        else{
            sda = 0;
        }
		I2C_Delay(DELAY_TIME);
        scl = 1;
        byt <<= 1;
		I2C_Delay(DELAY_TIME);
    }
	
    scl = 0;  
}

//
unsigned char I2CReceiveByte(void)
{
	unsigned char da;
	unsigned char i;
	for(i=0;i<8;i++){   
		scl = 1;
		I2C_Delay(DELAY_TIME);
		da <<= 1;
		if(sda) 
			da |= 0x01;
		scl = 0;
		I2C_Delay(DELAY_TIME);
	}
	return da;    
}

//
unsigned char I2CWaitAck(void)
{
	unsigned char ackbit;
	
    scl = 1;
	I2C_Delay(DELAY_TIME);
    ackbit = sda; 
    scl = 0;
	I2C_Delay(DELAY_TIME);
	
	return ackbit;
}

//
void I2CSendAck(unsigned char ackbit)
{
    scl = 0;
    sda = ackbit; 
	I2C_Delay(DELAY_TIME);
    scl = 1;
	I2C_Delay(DELAY_TIME);
    scl = 0; 
	sda = 1;
	I2C_Delay(DELAY_TIME);
}
//**************************************
/**
 * adc初始化
 * @param a 地址参数 0x40 0x03 0x01 或者加起来 具体看PCF8591芯片
 */
void init_ad(u8 add)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CStop();
}
/**
 * adc读取
 * @note 取决于ad_init的变量
 */
u8 read_ad()
{
	idata u8 tmp;
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	tmp = I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	return tmp;
}
/**
 * adc读取
 * @param dac电压输出值 0~255对应 0~5v
 */
void w_dac(u8 val)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x43);//单DAC 0x40  光敏+0x01  adc+0x03 如果光敏 电位器 dac都要的话，需要分别读取(思路:先读取0x41，再读取0x43)
	I2CWaitAck();
	I2CSendByte(val);
	I2CWaitAck();
	I2CStop();
}
/**
 * e2prom单读取
 * @param  地址位 数据位(写入要8个8个的写)
 */
void w_e2p(u8 add,u8 val)
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CSendByte(val);
	I2CWaitAck();
	I2CStop();
}
/**
 * e2prom单写入
 * @param  地址位 (读取要8个8个的读)
 */
u8 r_e2p(u8 add)
{
	idata u8 tmp;
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CStop();

	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	tmp = I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	return tmp;
}

//***************
void adc_proc()
{
	cj.ad=read_ad();
	
	if(cj.ad>150)	
		on(JDQ);
	else
		off(JDQ);
	
	w_dac(cj.ad);
}














