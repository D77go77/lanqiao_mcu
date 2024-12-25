#include "wave.h"

void de(u8 a); // 延时函数，通过消耗CPU周期实现延时效果

u16 read_d()
{
    u16 d; // 定义变量d用于存储读取的数据
    u8 num = 16; // 定义变量num并初始化为16，用于计数

    TX = 0; // 将TX设置为低电平
	EA=0;
    while (num--) // 执行一个循环，每次循环调用de函数进行延时，并切换TX电平
    {
        de(14); // 调用延时函数，延时14个周期
        TX = !TX; // 切换TX的电平状态
    }
	EA=1;

    CH = CL = 0; // 初始化CH和CL为0
    CR = 1; // 设置CR为1，启动数据读取
    while (RX && CH < 0x20); // 等待RX变为0或CH达到0x20
    CR = 0; // 设置CR为0，停止数据读取
    d = (u16)CH << 8 | CL | 156; // 计算最终的数据值
    return d; // 返回读取的数据值
}

void de(u8 a)
{
    while (a--); // 消耗CPU周期以实现延时
}

//***************
void wave_proc()
{
    cj.d = read_d() * 0.017f; // 读取数据并乘以0.017后赋值给cj.d
}