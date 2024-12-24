#include "all.h"
xdata struct _cj cj;
// ��������غ�������
void Scheduler_Init(void);
void Scheduler_Run(void);
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
    Timer1_Init();//ϰ�߳�ʼ����ʱ���жϷ����
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
// ����������ṹ�嶨��
typedef struct
{
    void (*task_func)(void);    // ������
    unsigned long int rate_ms;  // ����ִ�����ڣ����룩
    unsigned long int last_run; // �����ϴ�����ʱ��
} task_t;
// �����������б�
idata task_t Scheduler_Task[] = {
    {led_proc, 10, 0},        // LED����ÿ20����ִ��һ��
    {key_proc, 10, 0},        // ��������ÿ20����ִ��һ��
    {smg_proc, 50, 0},        // ���������ÿ100����ִ��һ��
    {uart_proc, 5, 0},     	  // ��������ÿ10����ִ��һ��
    {temp_proc, 100, 0},      // �¶�����ÿ200����ִ��һ��
    {wave_proc, 75, 0},       // ��������ÿ150����ִ��һ��
    {rtc_proc, 250, 0},       // ʱ������ÿ500����ִ��һ��
    {adc_proc, 80, 0},        // AD/DA����ÿ160����ִ��һ��
};

idata u8 task_num;//��������
//��������ʼ��
void Scheduler_Init(void)
{
    task_num = sizeof(Scheduler_Task) / sizeof(task_t); // ������������
}
//����������
void Scheduler_Run(void)
{
    u8 i;
    for (i = 0; i < task_num; i++)
    {
        unsigned long int now_time = uwTick;// ��ȡ��ǰʱ�� 
        if (now_time >= (Scheduler_Task[i].last_run + Scheduler_Task[i].rate_ms))// ��������Ƿ���Ҫִ��
        {
            Scheduler_Task[i].last_run = now_time; // ���������ϴ�����ʱ��
            Scheduler_Task[i].task_func();// ִ������         
        }
    }
}