#include "time.h"
idata unsigned long int uwTick = 0;   //ϵͳ��ʱ����
u8 f_on=0;//led��˸��־
u8 f_wei=0;//smg��˸��־

void tim_proc();
//**************************
void Timer0_Init(void)
{
  AUXR &= 0x7F; // ��ʱ��ʱ��1ms 12Tģʽ
  TMOD &= 0xF0; // ���ö�ʱ��ģʽ
  TMOD |= 0x05; // ����Ϊģʽ5����ʱ��ģʽ��//������ģʽ
  TL0 = 0;      // ���ó�ʼֵ
  TH0 = 0;      // ���ó�ʼֵ
  TF0 = 0;      // ���TF0��־
  TR0 = 1;      // ������ʱ��0
}
//**************************
void Timer1_Init(void)		//2����@12.000MHz
{
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x40;				//���ö�ʱ��ʼֵ
	TH1 = 0xA2;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

// ��ʱ��1�жϷ������ÿ�����ж�һ�Σ�uwTick����1
void pit1() interrupt 3
{
    uwTick++;//ϵͳ������ʱ��
    disp_scan(); // ���������ɨ�躯��
    tim_proc();  // ����ʱ�䴦����
}

// ʱ�䴦���������ڴ���ͬ��ʱ�����¼�
void tim_proc()
{
    static u8 cnt_20ms = 0;
    static u32 cnt_800ms = 0;
    static u32 cnt_1s = 0;
    
    // ÿ10���жϣ���ÿ20ms��ִ��һ��LEDɨ��
    if (++cnt_20ms >= 10) {
        cnt_20ms = 0;
        led_scan(); // ����LEDɨ�躯��
    }

    // ÿ400���жϣ���ÿ800ms���л�f_wei��f_on��״̬
    if (++cnt_800ms >= 400) {
        cnt_800ms = 0;
        f_wei = !f_wei; // �л�f_wei״̬
        f_on = !f_on;   // �л�f_on״̬
    }
    
    // ÿ500���жϣ���ÿ1s������Ƶ�ʼ�������ն�ʱ��0
    if (++cnt_1s >= 500) {
        cnt_1s = 0;
        cj.freq = (TH0 << 8) | TL0; // ����Ƶ��
        TH0 = TL0 = 0;           // ��ն�ʱ��0
    }
    
    // ���uu.ton��־λΪ�棬��ִ�ж�ʱ�ض��߼�
    if (uu.ton) {
        if (++uu.t >= 20) {
            uu.cmd = 2; // ��������Ϊ2
        }
    }
}