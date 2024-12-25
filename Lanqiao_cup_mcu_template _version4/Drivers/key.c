#include "key.h"

u8 key_code=0;

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

void key_scan()
{
	static u8 k_state = 0;
	static u8 _key=0;
	static u8 _cnt=0;
	u8 k_tmp;
	
	k_tmp=getkey22();
	switch(k_state)
	{
		case 0:
			if(k_tmp)k_state=1; // 如果检测到有键按下，则进入状态1
		break;
		case 1:
			if(k_tmp)// 如果持续检测到键按下，则进一步判断是哪个键
			{
				_key=k_tmp;
				if(_key==17)// 如果是特定的键（例如数字17），则进入状态3
				{
					k_state=3;
					_cnt=0;
				}
				else if(_key==8 || _key== 5)// 如果是其他特定的键（例如数字8或5），则进入状态4
				{
					k_state=4;
					_cnt=0;
				}else
				k_state=2; // 对于其他键，进入状态2
			}else k_state=0; // 如果键释放，则回到初始状态0
			break;
		case 2:
			if(!k_tmp)// 如果键释放，则记录键值并回到初始状态0
			{
				key_code=_key;
				k_state=0;
			}
		break;
		case 3:
			if(k_tmp) // 如果是长按特定键（例如数字17），在达到一定时间长按后进入状态30
			{
				if(++_cnt>=50)
				{
					key_code=17;//累加
					k_state=30;
				}
			}else k_state=0;
		break;
		case 30:
			if(!k_tmp)
			{
				k_state=0;
			}
		break;
			
		case 4:
			if(k_tmp)// 对于其他特定键（例如数字8或5），在长按一段时间后发送键值
			{
				if(++_cnt>=50)
				{
					_cnt-=5;//反复触发-长按累加
					key_code=_key;
				}
			}else // 如果键释放，则发送键值并回到初始状态0
			{
				key_code=_key;
				k_state=0;
			}
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
        case 9:
            // 保留为空，以便未来扩展
        break;
        case 17:
            cj.key=0;// 重置键值
        break;
    }
    key_code=0;// 重置按键代码以准备下一次键盘事件
}











