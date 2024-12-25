#include "all.h"
xdata struct _cj cj;

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
    Timer1_Init();//习惯初始化中断定时器放最后//
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
