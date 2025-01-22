#include "key.h"

idata u8 key_code=0;

u8 getkeybtn()
{
	u8 tmp=0;
	if(!P30)tmp+=7;
	if(!P31)tmp+=6;
	if(!P32)tmp+=5;
	if(!P33)tmp+=4;
	return tmp;
}

u8 getkey22()
{
	u8 tmp=0;
	P33=1;P32=0;
	P44=1;P42=1;
	if(!P44)tmp+=5;
	if(!P42)tmp+=9;
	P33=0;P32=1;
	P44=1;P42=1;
	if(!P44)tmp+=4;
	if(!P42)tmp+=8;
	
	return tmp;
}

xdata u8 Key_Val, Key_Down, Key_Up, Key_Old;
//按键扫描函数，该函数10ms扫描一次，并通过边沿检测消抖
void key_scan()
{
	static xdata u8 key_tick = 0;// 长按计时器
	static idata u8 key_state = 0; // 状态机状态
	Key_Val = getkey22();
	Key_Down =  Key_Val & (Key_Old ^ Key_Val); // 按键按下检测 高电平按键
	Key_Up 	 = 	~Key_Val & (Key_Old ^ Key_Val);// 按键抬起检测 高电平按键
	Key_Old  = 	Key_Val;
 
	switch(key_state)
	{
		case 0:// 短按，按下触发
			if(Key_Val == Key_Old && Key_Down)// 如果当前按键和上次一样 并且 已经有按下的键码
			{
				key_tick = 0;
				key_code = Key_Down;
				// 可在此检测Down键码 添加特殊状态，比如连击
				if(Key_Down == 5 || Key_Down == 8)
					key_state = 2;// 连击
				else if(Key_Down == 9)
					key_state = 1;// 长按
			}
			break;
		case 1:// 长按，按下触发 并等待释放
			if(Key_Val)
			{
				if(++key_tick >= 100)
				{
					key_state = 3;
					key_code = Key_Val + 10;
					
				}
			}else key_state = 0;
			break;
		case 2:// 连按，按下触发
			if(Key_Val)
			{
				if(++key_tick >= 100)
				{
					key_tick -= 10;//重置累加器
					key_code = Key_Val;
				}
			}else key_state = 0;
			break;
		case 3:// 等待释放
			if(!Key_Val) key_state=0;
			break;
	}
}


// 处理键盘事件的函数
void key_proc()
{
    // 执行键盘扫描
    key_scan();
    // 根据按键代码处理不同的逻辑
    switch(key_code)
    {
        case 4:
            switch(cj.state)// 根据当前状态改变状态
            {
                case 10:cj.state=20;break;
                case 20:cj.state=30;break;
                case 30:cj.state=40;break;
                case 40:cj.state=10;break;
            }
        break;
        case 5:
            // 在特定状态下增加键值
            switch(cj.state)
            {
                case 20:if(++cj.key>=100)cj.key=0;break;
            }
        break;
        case 8:
            // 在特定状态下减少键值
            switch(cj.state)
            {
                case 20:if(--cj.key>=100)cj.key=100;break;
            }
        break;
        case 19:
            cj.key=0;// 重置键值
        break;
        case 14:
            cj.key=0;// 重置键值
        break;
    }
    key_code=0;// 重置按键代码以准备下一次键盘事件
}











