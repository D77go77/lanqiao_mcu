#include "all.h"
xdata struct _cj cj;
// 调度器相关函数声明
void Scheduler_Init(void);
void Scheduler_Run(void);
// 系统初始化函数
void init()
{
    // 初始化继电器和蜂鸣器
    off(JDQ);off(FMQ);
    // 初始化PCA计数器超声波模块
    CMOD=CH=CL=0;
    // 初始化实时时钟、AD模块和温度传感器
    rtc_run(0x00,0x01,0x21);
    init_ad(0x43);
    init_temp();
    // 初始化控制结构体cj
    cj.state=10;
    cj.h=0;cj.m=1;cj.s=21;
    cj.key=0;cj.freq = 0;
    // 初始化调度器
    Scheduler_Init();
    // 初始化串口和用户接口
    Uart1_Init();
    uu_init();
    // 初始化定时器0和定时器1
    Timer0_Init();
    Timer1_Init();//习惯初始化定时器中断放最后
}

// 主函数
void main()
{
    // 系统初始化
    init();
    // 打开中断，开始正常运行
    EA=1;
    while(1)
    {
        Scheduler_Run();// 运行调度器
    }
}
// 调度器任务结构体定义
typedef struct
{
    void (*task_func)(void);    // 任务函数
    unsigned long int rate_ms;  // 任务执行周期（毫秒）
    unsigned long int last_run; // 任务上次运行时间
} task_t;
// 调度器任务列表
idata task_t Scheduler_Task[] = {
    {led_proc, 10, 0},        // LED任务，每20毫秒执行一次
    {key_proc, 10, 0},        // 键盘任务，每20毫秒执行一次
    {smg_proc, 50, 0},        // 数码管任务，每100毫秒执行一次
    {uart_proc, 5, 0},     	  // 串口任务，每10毫秒执行一次
    {temp_proc, 100, 0},      // 温度任务，每200毫秒执行一次
    {wave_proc, 75, 0},       // 距离任务，每150毫秒执行一次
    {rtc_proc, 250, 0},       // 时间任务，每500毫秒执行一次
    {adc_proc, 80, 0},        // AD/DA任务，每160毫秒执行一次
};

idata u8 task_num;//任务数量
//调度器初始化
void Scheduler_Init(void)
{
    task_num = sizeof(Scheduler_Task) / sizeof(task_t); // 计算任务数量
}
//调度器运行
void Scheduler_Run(void)
{
    u8 i;
    for (i = 0; i < task_num; i++)
    {
        unsigned long int now_time = uwTick;// 获取当前时间 
        if (now_time >= (Scheduler_Task[i].last_run + Scheduler_Task[i].rate_ms))// 检查任务是否需要执行
        {
            Scheduler_Task[i].last_run = now_time; // 更新任务上次运行时间
            Scheduler_Task[i].task_func();// 执行任务         
        }
    }
}