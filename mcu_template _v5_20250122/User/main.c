#include "all.h"
// 变量尽量使用idata,加快程序运行速度,优化内存
// 魔术棒里面的MemoryModel:必须使用small 来加快程序效率
// 注意图下 Program Siz :
//	data_ 不能大于230  防止data区堆栈空间不足 约232以上程序就会堆栈溢出
// 	如果大于就只能将部分不是高频率的变量使用xdata ，
//	code自然越低越好
idata struct _cj cj;

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
//    // 初始化串口和用户接口
//    Uart1_Init();
//    uu_init();
    // 初始化定时器0和定时器1
    Timer0_Init();
    Timer1_Init();//习惯初始化中断定时器放最后//uwTick定时器
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
