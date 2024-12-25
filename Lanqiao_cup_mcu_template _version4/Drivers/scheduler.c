#include "scheduler.h"

// 调度器任务结构体定义
typedef struct
{
    void (*task_func)(void);    // 任务函数
    unsigned long int rate_ms;  // 任务执行周期（毫秒）
    unsigned long int last_run; // 任务上次运行时间
} task_t;
// 调度器任务列表
idata task_t Scheduler_Task[] = {
	{led_scan,10,0},		  // LED端口扫描，每20毫秒执行一次
    {led_proc, 10, 0},        // LED任务，每20毫秒执行一次
    {smg_proc, 50, 0},        // 数码管任务，每100毫秒执行一次
	{led_smg_blink,250,0},	  //led & smg 闪烁函数，每1ms执行一次
	{key_proc, 10, 0},        // 键盘任务，每20毫秒执行一次
    {uart_proc, 5, 0},     	  // 串口任务，每10毫秒执行一次
	{uart_timeout,5,0},	      // 串口超时处理，每10毫秒执行一次
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