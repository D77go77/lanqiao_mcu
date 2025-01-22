/*	#   I2C����Ƭ��˵��
	1. 	���ļ������ṩ���������빩����ѡ����ɳ�����Ʋο���
	2. 	����ѡ�ֿ������б�д��ش�����Ըô���Ϊ������������ѡ��Ƭ�����͡������ٶȺ�����
		�жԵ�Ƭ��ʱ��Ƶ�ʵ�Ҫ�󣬽��д�����Ժ��޸ġ�
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
 * adc��ʼ��
 * @param a ��ַ���� 0x40 0x03 0x01 ���߼����� ���忴PCF8591оƬ
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
 * adc��ȡ
 * @note ȡ����ad_init�ı���
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
 * adc��ȡ
 * @param dac��ѹ���ֵ 0~255��Ӧ 0~5v
 */
void w_dac(u8 val)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x43);//��DAC 0x40  ����+0x01  adc+0x03 ������� ��λ�� dac��Ҫ�Ļ�����Ҫ�ֱ��ȡ(˼·:�ȶ�ȡ0x41���ٶ�ȡ0x43)
	I2CWaitAck();
	I2CSendByte(val);
	I2CWaitAck();
	I2CStop();
}
/**
 * e2prom����ȡ
 * @param  ��ַλ ����λ(д��Ҫ8��8����д)
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
 * e2prom��д��
 * @param  ��ַλ (��ȡҪ8��8���Ķ�)
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














