#include "all.h"
xdata struct _cj cj;

// ϵͳ��ʼ������
void init()
{
    // ��ʼ���̵����ͷ�����
    off(JDQ);off(FMQ);
    // ��ʼ��PCA������������ģ��
    CMOD=CH=CL=0;
    // ��ʼ��ʵʱʱ�ӡ�ADģ����¶ȴ�����
    rtc_run(0x00,0x01,0x21);
    init_ad(0x43);
    init_temp();
    // ��ʼ�����ƽṹ��cj
    cj.state=10;
    cj.h=0;cj.m=1;cj.s=21;
    cj.key=0;cj.freq = 0;
    // ��ʼ��������
    Scheduler_Init();
    // ��ʼ�����ں��û��ӿ�
    Uart1_Init();
    uu_init();
    // ��ʼ����ʱ��0�Ͷ�ʱ��1
    Timer0_Init();
    Timer1_Init();//ϰ�߳�ʼ���ж϶�ʱ�������//
}

// ������
void main()
{
    // ϵͳ��ʼ��
    init();
    // ���жϣ���ʼ��������
    EA=1;
    while(1)
    {
        Scheduler_Run();// ���е�����
    }
}
