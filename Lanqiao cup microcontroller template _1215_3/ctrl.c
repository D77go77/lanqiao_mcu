#include "ctrl.h"

// 定义控制数据，初始化为0x70
u8 ctrl_dat = 0x70;
/**
 * 执行门控操作
 * 先关闭，再打开指定的门控，然后再次关闭
 * @param a 门控参数
 */
void gate(u8 a)
{
    // 关闭门控
    CLOSE();
    // 打开门控a
    OPEN(a);
    // 再次关闭门控
    CLOSE();
}
/**
 * 打开指定的控制线
 * 如果指定的控制线已经打开，则不执行任何操作
 * @param a 控制线参数
 */
void on(u8 a)
{
    // 如果控制线a已经打开，则返回
    if((a&ctrl_dat)!=0)return;
    // 设置控制数据，打开控制线a
    ctrl_dat |= a;
    // 将控制数据写入P0端口
    P0=ctrl_dat;
    // 执行门控操作
    gate(CTRL);
}
/**
 * 关闭指定的控制线
 * 如果指定的控制线已经关闭，则不执行任何操作
 * @param a 控制线参数
 */
void off(u8 a)
{
    // 如果控制线a已经关闭，则返回
    if((a&ctrl_dat)==0)return;
    // 清除控制数据，关闭控制线a
    ctrl_dat &=~a;
    // 将控制数据写入P0端口
    P0=ctrl_dat;
    // 执行门控操作
    gate(CTRL);
}