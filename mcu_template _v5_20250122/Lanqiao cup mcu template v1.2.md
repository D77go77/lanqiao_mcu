# 蓝桥杯单片机模版建立

## 前言

- **智能控制社团written by ws**

  ## 更新日志

  - 内存管理问题

  - 按键底层更新
  - 串口底层添加buf缓存清除

  

## 关于宏定义介绍 

使用宏定义，方便我们快速打字

```c
// 定义无符号字符类型别名u8
typedef unsigned char u8;
// 定义无符号整数类型别名u32
typedef unsigned int u32;
```

## 锁存器与三八译码器介绍

这里我们需要有一个基础知识，就是我们的锁存器和三八译码器介绍，这是一个比较关键的点。我们看到下面这两个图，可以看到我们的锁存器是用P25，P26，P27进行控制，即P2的高三位进行的控制，我们只需要按照如下的命令进行书写即可。

其中的P2&=0x1f，表示我们将高三位置为0，10x?0的操作表明我们对高三位进行书写，比如Y4C，那么就代表我们的高三位要表示为4，即0100，我们将其放在高位，即1000_0000，翻译过来就是0x80。

```c
P2 &= 0x1f;//CLOSE
P2 |= 0x?0;//OPEN
P2 &= 0x1f;//CLOSE
```



<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412170901257.png" alt="image-20241217090108151" style="zoom:67%;" />

## NOP延时函数

这里要注意一个问题，就是我们延时函数大部分都有NOP，包括Ds1302，IIC都使用了这个，这个函数存在于头文件intrins.h中，需要注意一下引用，一般来说，一个NOP在12MHz的单片机下是1us，其余的自己去换算一下。

## LED模版建立

------

### Led底层实现

我们可以看到代码中对LED的操作实现。值得注意的是，这里采用了 `volatile` 和 `bdata` 关键字，并使用了 `sbit` 对 `led_dat` 变量进行逐位定义。这种设计在C51单片机上非常有用，既能节约内存，又能方便逐个位操作。

------

### ****内存优化****

由于C51单片机的内存容量非常有限，我们在代码设计中尽可能减少不必要的内存占用。定义的 `led_dat` 使用了 `bdata` 存储器空间，这种特殊存储类型可以直接访问位，配合 `sbit` 可以对每个位进行独立操作。

示例代码：

```c
volatile  u8  bdata led_dat=0;
sbit L1 = led_dat^0;
sbit L2 = led_dat^1;
sbit L3 = led_dat^2;
sbit L4 = led_dat^3;
sbit L5 = led_dat^4;
sbit L6 = led_dat^5;
sbit L7 = led_dat^6;
sbit L8 = led_dat^7;
```

这里将 `led_dat` 的每个位绑定到了 `L1` 到 `L8`，它们分别代表8个LED灯的状态。

------

### LED扫描函数****

`led_scan` 函数的主要作用是将 `led_dat` 取反后输出到P0端口，实现LED硬件控制。因为硬件电路中，输出低电平时LED才会点亮，因此代码中通过 `~led_dat` 实现逻辑取反。

```c
void led_scan()
{
    P0 = ~led_dat;  // 将led_dat取反后输出到P0端口
    gate(LED);      // 控制LED点亮状态
}
```

1. **P0 = ~led_dat**：通过取反操作符合硬件原理

   **特别说明**：为了符合正常人的直觉，我们在逻辑层面用 **1表示亮**，硬件上取反输出实现 **低电平点亮**。

2. **gate(LED)**：调用外部函数，进一步控制LED状态。

------

## 蜂鸣器，继电器，MOTOR模版建立

------

### Ctrl底层实现

在 `Ctrl.c` 中，我们定义了一系列控制函数，主要包括：

1. **`gate`**：执行门控操作，先关闭门控、再打开指定门控，然后再次关闭，确保硬件控制的稳定性。
2. **`on` 和 `off`**：用于打开和关闭指定的控制线，并通过位操作修改 `ctrl_dat` 数据变量，最终写入到P0端口，控制具体硬件设备。

------

### 变量和宏定义

在代码中，使用了以下变量和宏来简化控制操作：

- **`u8 ctrl_dat`**：控制数据变量，初始化为 `0x70`，用于记录当前控制线的状态。
- **`CLOSE()` 和 `OPEN(a)`** 宏：分别用于关闭门控和打开指定的门控地址，操作P2端口的高三位。

示例代码：

```c
#define CLOSE() P2 &= 0x1f   // 清除P2高三位，关闭门控
#define OPEN(a) P2 |= a      // 设置P2高三位，打开指定门控
```

------

### ****门控操作函数****

`gate` 函数用于对门控进行控制，先关闭再打开指定门控，最后再次关闭。这种设计保证了控制的稳定性，避免误操作。

```c
void gate(u8 a)
{
    CLOSE();  // 关闭门控
    OPEN(a);  // 打开指定门控a
    CLOSE();  // 再次关闭门控
}
```

**执行过程：**

1. **CLOSE()**：清除P2的高三位，关闭所有门控。
2. **OPEN(a)**：打开指定的门控。
3. **CLOSE()**：确保执行后的稳定性，再次关闭门控。

------

### ****打开控制线函数****

`on` 函数用于打开指定的控制线。它会先检查目标控制线的状态，如果已打开，就不再执行操作，从而避免重复操作，提高运行效率。

```c
void on(u8 a)
{
    if ((a & ctrl_dat) != 0) return;  // 如果控制线a已经打开，直接返回
    ctrl_dat |= a;                  // 设置控制数据，打开控制线a
    P0 = ctrl_dat;                  // 将数据写入P0端口
    gate(CTRL);                     // 执行门控操作
}
```

**操作步骤：**

1. **状态检查**：通过 `a & ctrl_dat` 判断控制线是否已打开。
2. **修改数据**：通过 `ctrl_dat |= a` 将指定控制线置1（打开）。
3. **数据写入**：将修改后的数据输出到P0端口。
4. **门控执行**：调用 `gate` 执行门控操作，确保操作有效。

------

### ****关闭控制线函数****

`off` 函数用于关闭指定的控制线。逻辑与 `on` 类似，但这里是将目标控制线置0（关闭）。

```c
void off(u8 a)
{
    if ((a & ctrl_dat) == 0) return;  // 如果控制线a已经关闭，直接返回
    ctrl_dat &= ~a;                 // 清除控制数据，关闭控制线a
    P0 = ctrl_dat;                  // 将数据写入P0端口
    gate(CTRL);                     // 执行门控操作
}
```

**操作步骤：**

1. **状态检查**：通过 `a & ctrl_dat` 判断控制线是否已关闭。
2. **修改数据**：通过 `ctrl_dat &= ~a` 将指定控制线置0（关闭）。
3. **数据写入**：将修改后的数据输出到P0端口。
4. **门控执行**：调用 `gate` 执行门控操作。

------

### 头文件参数声明

`ALL.H` 是整个项目的头文件，包含了多个模块的头文件及宏定义，确保代码的可复用性和可维护性。

```c
#ifndef ALL_H
#define ALL_H
#define CTRL 0xa0       // 控制命令地址
#define CLOSE() P2 &= 0x1f  // 关闭门控
#define OPEN(a) P2 |= a     // 打开指定门控
#define JDQ 1<<4        // 继电器控制信号
#define MOTO 1<<5       // 电机控制信号
#define FMQ 1<<6        // 蜂鸣器控制信号
#define LED 1<<7		// LED控制信号 
#endif
```

## Key模版建立

------

### 按键扫描原理

我们采用 **行列扫描法** 来检测按键状态。具体过程如下：

1. **逐列扫描**：将一列设置为 **低电平**，其他列保持 **高电平**。

2. **判断行电平**：检测指定行是否为低电平。如果某行检测到低电平，则该行对应的按键被按下。

3. **按键状态机**：使用状态机处理按键的 **短按** 和 **长按** 等事件。

   <img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161455095.png" alt="image-20241216145526053" style="zoom: 80%;" />

------

### 基础代码结构

1. **`getkeybtn` 函数**：简单的按键读取函数，适合检测单个按键状态。
2. **`getkey22` 函数**：实现多行多列的按键扫描。
3. **`key_scan` 函数**：基于状态机，管理按键的按下、释放、短按、长按等状态。

------

### 代码详解

#### 独立按键扫描函数

```c
u8 getkeybtn()
{
	u8 tmp=0;
	if(!P30)tmp+=7;
	if(!P31)tmp+=6;
	if(!P32)tmp+=5;
	if(!P33)tmp+=4;
	return tmp;
}
```

#### 矩阵按键扫描函数****

`getkey22` 函数用于实现按键的行列扫描。通过设置列的电平，然后检查行的电平来判断按键是否被按下。

```c
u8 getkey22()
{
    u8 tmp = 0;
    // 扫描第一列
    P33 = 1; P32 = 0;   // P32设置为低电平，P33为高电平
    P44 = 1; P42 = 1;   // 行保持高电平
    if (!P44) tmp += 5; // 检测行P44是否为低电平
    if (!P42) tmp += 9; // 检测行P42是否为低电平
    // 扫描第二列
    P33 = 0; P32 = 1;   // P33设置为低电平，P32为高电平
    P44 = 1; P42 = 1;   // 行保持高电平
    if (!P44) tmp += 4; // 检测行P44是否为低电平
    if (!P42) tmp += 8; // 检测行P42是否为低电平
    return tmp;
}
```

**说明**：

- **逐列设置低电平**：通过 `P32` 和 `P33` 控制列。
- **判断行状态**：检测 `P44` 和 `P42` 是否为低电平。
- **返回按键值**：根据检测到的行电平，返回按键值。

------

#### 按键消抖函数

通过边沿检测，检测按键的上下沿，减少采样率来达到消抖的目的，其中getkey22再非按下的情况返回0，也就是说该按键Key_Val 是高电平按键。需要注意的是 下面的Key_Down，即下降沿键码。

```
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
}
```



#### 状态机扫描函数

`key_scan` 函数使用状态机管理按键的按下、长按、连击、释放等状态。

```c
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
```

**状态说明**：

1. **状态0**：确认按键按下，进一步检测按键类型。
2. **状态1**：长按检测，执行特定操作。
3. **状态2**：连按检测，执行特定操作。
4. **状态3**：长按释放，回到初始状态。

------

### 注意事项

1. **P30/P31 冲突**：由于P30/P31用于串口通信，当使用串口，且按键需要P30/P31行按键扫描时需关闭串口轮询。

   ```c
   ET1 = 0;  // 关闭定时器1轮询
   key_scan();
   ET1 = 1;  // 重新开启定时器1
   ```

2. **状态管理**：使用状态机可以有效处理短按、长按及多键功能，确保系统稳定。

在这里要注意一个问题，我们的串口进行发送的时候会占用P30和P31口，这个是硬件的冲突，所以我们在进入这个扫描函数的时候，需要对其关掉轮询（就是在main函数里面用的什么来进行各个函数定时的定时器，叫做轮询定时器）。这里我用的是定时器1，所以我进入后就要使用ET1=0关掉，结束后用P3=0xff；ET1=1开启。

## smg模板建立

本教程介绍 **`smg.c`** 文件中数码管显示的实现，包括数码管字符映射、数据处理和扫描显示的实现方法。数码管可以显示 **数字、字母及符号**，并支持小数点显示。代码设计合理且易于扩展。

------

### 数码管段码推导

这里我单独开一个标题来做数码管的编码推导，因为这个很重要，我们看到下方的截图：

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161506262.png" alt="image-20241216150611209" style="zoom:50%;" />

可以观察到每一段的数码管都被编号，这里由于我们用的是共阳极数码管，即0亮1灭，下面我给一个例子来进行说明

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161506164.png" alt="image-20241216150650120" style="zoom:50%;" />

我们标好后按照dp > g > … > a的顺序写出，即1010_0100 > 0xa4，这个值在数码管的显示中就代表着“2”。

所以我们就可以根据这个规律来编写一个段选表。

#### 字符映射表

这个方便我们打字，直接复制资源包里的段码表，对于没有的段码就自己推导添加。

```c
code unsigned char smg[] =
{
    0xc0, // 0
    0xf9, // 1
    0xa4, // 2
    0xb0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xf8, // 7
    0x80, // 8
    0x90, // 9
    0x88, // A
    0x83, // b
    0xc6, // C
    0xa1, // d
    0x86, // E
    0x8e  // F
};
```

- 说明：
  - `0xc0` 表示数字 **0** 的段选值，对应二进制 `1100 0000`（a, b, c, d, e, f 段点亮）。
  - 其他字符按照段选法的规则设置。
  - 可以根据项目需求扩展其他字符，如 **`-`**（0xbf）。

------

### 显示缓冲区

定义 **`dis_buf`** 和 **`dis_str`** 两个缓冲区：

```c
u8 dis_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 数码管显示数据缓存
u8 dis_str[11];                           // 显示字符串缓存
```

- **`dis_buf`**：存储每个数码管的显示数据。
- **`dis_str`**：格式化后的字符串，通过 `sprintf` 设置显示内容。

------

### 数码管扫描函数

`disp_scan` 函数实现数码管的 **动态扫描**。原理如下：

1. **逐位选中** 数码管，并输出对应的显示数据。
2. 使用静态变量 **`i`** 记忆当前扫描位置，每次调用时切换到下一个数码管。

**代码：**

```c
void disp_scan()
{
    static u8 i = 0;  // 记忆当前扫描位置
    P0 = 0;           // 关闭所有数码管
    gate(WEI);        // 位选信号
    P0 = dis_buf[i];  // 输出当前位的数据
    gate(DUAN);       // 段选信号
    P0 = 1 << i;      // 选中当前位数码管
    gate(WEI);        // 位选信号

    if (++i == 8) i = 0;  // 循环扫描，超过8位重置
}
```

- **`P0`**：用于输出数据。
- **`gate(WEI)` 和 `gate(DUAN)`**：分别设置位选和段选信号。
- **`1 << i`**：选中第 `i` 位数码管。

------

### 数据处理函数

`disp_proc` 函数将字符串转换为数码管的显示数据，并存入 **`dis_buf`**。

**实现过程：**

1. 遍历字符串 `dis_str`。
2. 根据字符映射表 `smg[]` 查找对应的显示值。
3. 支持 **小数点** 的处理，点亮小数点时将对应的段数据与 `0x7F` 按位与。

**代码：**

```c
void disp_proc()
{
    u8 i, j, tmp = 0;
    for (i = 0; i < 8; i++, j++)
    {
        switch (dis_str[j])
        {
            case '0': tmp = smg[0]; break;
            case '1': tmp = smg[1]; break;
            case '2': tmp = smg[2]; break;
            case '3': tmp = smg[3]; break;
            case '4': tmp = smg[4]; break;
            case '5': tmp = smg[5]; break;
            case '6': tmp = smg[6]; break;
            case '7': tmp = smg[7]; break;
            case '8': tmp = smg[8]; break;
            case '9': tmp = smg[9]; break;
            case 'A': tmp = smg[10]; break;
            case 'b': tmp = smg[11]; break;
            case 'C': tmp = smg[12]; break;
            case 'd': tmp = smg[13]; break;
            case 'E': tmp = smg[14]; break;
            case 'F': tmp = smg[15]; break;
            case '-': tmp = 0xbf; break;
            default: tmp = 0xff; break;
        }
        if (dis_str[j + 1] == '.')
        {
            tmp &= 0x7f;  // 点亮小数点
            j++;          // 跳过小数点字符
        }
        dis_buf[i] = tmp;  // 存入显示缓冲区
    }
}
```

------

### 数码管显示处理函数

`smg_proc` 根据系统状态 `cj.state` 格式化显示内容，并调用 `disp_proc` 进行处理。

```c
void smg_proc()
{
    switch (cj.state)
    {
        case 10:
            if (f_wei)
                sprintf(dis_str, "F%02bu-%02bubb", cj.m, cj.s);
            break;
    }
    disp_proc();  // 处理显示数据
}
```

------



## Ds1302时钟模版建立

这里有官方提供的参考代码，所以我们上面的时序可以不用书写，我们直接对下方的时间读取/写入进行操作。

我们如果需要读取时钟的话，只需要读取地址0x85->小时 0x83->分钟 0x81->秒钟。

我们如果需要写入时钟的话，需要先将0x8E位置的最高位WP置0，解除写保护后才能进行写入，写入的地址为0x84->小时 0x82->分钟 0x80->秒钟。

------

### RTC 设置函数

`rtc_run` 函数用于设置 DS1302 芯片的时间。具体实现步骤：

1. **停止 RTC**：通过写入控制寄存器 `0x8E` 关闭写保护。
2. **设置时间**：分别设置 **小时**、**分钟** 和 **秒钟**。
3. **启动 RTC**：重新启用写保护以启动时钟运行。

------

**代码解析：**

```c
void rtc_run(u8 h, u8 m, u8 s)
{
    // 停止RTC，解除写保护
    Write_Ds1302_Byte(0x8e, 0);
    // 设置时间：写入小时、分钟、秒钟
    Write_Ds1302_Byte(0x84, h);  // 设置小时
    Write_Ds1302_Byte(0x82, m);  // 设置分钟
    Write_Ds1302_Byte(0x80, s);  // 设置秒钟
    // 启动RTC，重新启用写保护
    Write_Ds1302_Byte(0x8e, 0x80);
}
```

------

**说明：**

- 寄存器地址:`0x80`：秒寄存器（写操作）`0x82`：分寄存器（写操作）`0x84`：时寄存器（写操作）`0x8E`：控制寄存器，用于启用/停止写保护。
- **写保护机制**：写入 `0` 停止 RTC，写入 `0x80` 启动 RTC。
- 参数格式
  - 传入的 `h`、`m`、`s` 需为 **BCD 格式**。例如 `0x12` 表示 12 小时。

------

### RTC 读取函数

`rtc_proc` 函数用于从 DS1302 芯片中读取当前时间，并将其转换为 **十进制格式** 以便显示和使用。

------

代码解析：

```c
void rtc_proc()
{
    u8 h, m, s;
    // 禁用中断，保证数据读取完整性
    EA = 0;
    // 读取时间寄存器数据
    h = Read_Ds1302_Byte(0x85);  // 读取小时
    m = Read_Ds1302_Byte(0x83);  // 读取分钟
    s = Read_Ds1302_Byte(0x81);  // 读取秒钟
    // 恢复中断
    EA = 1;
    // 将BCD格式的时间转换为十进制
    cj.h = (h >> 4) * 10 + (h & 0x0f);  // 高四位为十位，低四位为个位
    cj.m = (m >> 4) * 10 + (m & 0x0f);
    cj.s = (s >> 4) * 10 + (s & 0x0f);
}
```

------

**说明：**

1. **中断控制**：在读取 RTC 数据时，关闭中断（`EA = 0`），防止其他中断影响数据读取的完整性。

2. 寄存器地址`0x81`：秒寄存器（读操作）。`0x83`：分寄存器（读操作）。`0x85`：时寄存器（读操作）。

3. BCD 转十进制

   - **BCD 格式**：`0x12` 表示 12，`0x23` 表示 23。

   - 转换公式：

     ```
     (值 >> 4) * 10 + (值 & 0x0F)
     ```

     - `值 >> 4` 获取高四位（十位）。
     - `值 & 0x0F` 获取低四位（个位）。

## Onewire温度模版建立

### onewire.c

这里同样有官方提供的参考代码，所以我们上面的时序可以不用书写，我们直接对下方的进行温度读取操作。

首先是我们需要绑定DQ的引脚，在原理图中可以看到：

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161514337.png" alt="image-20241216151448423" style="zoom: 80%;" />

```c
sbit DQ = P1 ^ 4;
```

我们不需要管上方的时序的书写，只需要进行温度读取的代码书写就行，下方是温度芯片的使用顺序（官方手册），首先我们需要运行init，然后再检查rom（可跳过），最后执行我们的功能函数，注意，一次只能使用一个功能。

![image-20241216151555825](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161515891.png)

我们可以在官方手册上找到需要运行的指令

![image-20241216151633355](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161516409.png)

当我们发送0xcc的时候，就会跳过R0M检查，并且这里也说明了，我们发送0x44就可以开启温度转换，发送0xbe就可以开始读取温度，但是要注意一个事情，我们同样可以在我们的官方的手册上面找到一段读取的语句，这里表明我们先读取低位，再读取高位，最后将其拼接后再乘精度就是我们的数据了

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161517612.png" alt="image-20241216151713548" style="zoom:80%;" />

温度的精度在文档中对应如下

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161517076.png" alt="image-20241216151744012" style="zoom:80%;" />

精度为9→0.5；10→0.25；11→0.125；12（默认）→0.0625

我们就可以根据上面的描述来进行代码书写

```c
void init_temp()
{
    init_ds18b20(); // 初始化DS18B20温度传感器
    Write_DS18B20(0xcc); // 发送跳过ROM命令，不读取特定传感器的序列号
    Write_DS18B20(0x44); // 发送转换温度命令，开始温度转换
}
float read_temp()
{
    u8 H,L; // 声明变量H和L用于存储温度数据的高低字节
    float tmp; // 声明变量tmp用于存储计算得到的温度值
    init_ds18b20(); // 初始化DS18B20温度传感器
    Write_DS18B20(0xcc); // 发送跳过ROM命令，不读取特定传感器的序列号
    Write_DS18B20(0xbe); // 发送读取温度命令，开始读取温度数据
    L=Read_DS18B20(); // 读取温度数据的低字节
    H=Read_DS18B20(); // 读取温度数据的高字节
    init_temp(); // 再次初始化温度传感器，准备下一次温度转换
    tmp=(u32)H<<8|L; // 将高字节和低字节组合成完整的温度数据
    return tmp*0.0625f; // 将温度数据转换为实际温度值并返回
}
```

这里给一个调用示例

```c
 cj.t=read_temp(); // 更新全局变量cj.t为当前温度值
```

## I2C_PCF8591(AD/DA)与AT24C02(EEPROM)模版建立



这里有官方提供的参考代码，所以我们上面的时序可以不用书写，我们直接进行AD读取与DA输出还有AT24C02的存储操作。

首先是我们需要绑定sda和scl的引脚，在原理图中可以看到，并且要注意一个问题，我们用到了NOP函数，所以需要引入头文件intrins.h

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161525922.png" alt="image-20241216152521865" style="zoom:80%;" />

```c
#include "intrins.h"
sbit scl = P2 ^ 0;
sbit sda = P2 ^ 1;
```



我在这里需要说明一下IIC通信的方式，他是一个一个八位的数据，其中第一位是我们的发送地址，永远都是1，然后后面挂载了不同的外设，我们的蓝桥杯的板子挂了两个外设，一个是PCF8591，对应着AD/DA的功能：另一个是AT24C02，对应着EEPR0M的存储功能。在我们板子上就长我们下面这个样子，所以我们就可以知道，如果我们要对AT24C02芯片写入数据的话，我们就需要使用地址为（1010_0000一0xa0），要读取数据的化，我们就要使用地址为（1010_0001一0xa1）；要写入DA输出的数据的话，我们就需要使用地址为（1081_8008一→0x98），要读取AD采样的数据的话，我们就需要使用地址为（1001_0001->0x91)。

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161526203.png" alt="image-20241216152628151" style="zoom:80%;" />

在我们使用IIC进行通信的时候，我们要遵循下面的运行逻辑

```c
I2cStart();
I2CSendByte（地址);
I2CWaitAck();
I2CSendByte（功能);
I2CWaitAck(）;
```

### AD与DA

从上面说明的内容的我们可以知道，如果我们要对PCF8591进行操作，那么就需要对地址0x90和0x91进行操作我们现在先说一下AD的代码，通过观察原理图我们可以发现，AD有4个通道可以进行读取

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161527581.png" alt="image-20241216152740525" style="zoom:80%;" />

其中AINO通道是我们的外部测量通道（由于蓝桥杯现在还没有涉及到信号发生器，所以我们可以先暂时不用管这个通道）



![image-20241216152759522](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161527569.png)

AIN1通道很明显就是光敏电阻的分压读取（光照越强，分压越高，采集到的结果越大），

AIN2通道为差分输入（我们也是不常用到的输入，因为没有信号发生器）

AIN3通道为滑动变阻器输入（我们测量滑动变阻器的分压，这个你左右旋钮看一下就知道哪边变大了)

![image-20241216152842456](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161528522.png)

我们查看一下PCF8591的手册可以看到，我们读取的地址的书写方式，这里我统一给一个说明

![image-20241216152912714](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161529770.png)

我们那么这个时候常考的就是下面的形式，注意一下题目中是否需求DA输出，如果需要DA输出的话，这里使用地址的时候必须使用允许DA输出，

![image-20241216152930899](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161529957.png)

AD完整代码：

```c
void init_ad(u8 add)//add为选择设备
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CStop();

}
u8 read_ad()
{
	u8 tmp;
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	tmp=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	return tmp;
}
```

说完AD我们就要说到DA了

和AD一样，我们只需要传入我们需要写入的数据就行，但是要注意一下我们的数据为数字电压，即0-255，我们输出后的电压是模拟电压，即0-5V，所以我们是一一映射过去的

举个例子，我们如果想要输出电压为2V,那么我们就应该如果想要输出的电压为2.5V，那么我们就应该，

```c
Da_Write(2*51);
```

这里不用关心他会出现不匹配的情况，C语言底层会自动转换为unsigned char类型的,这类有啊注意

```c
Da_Write(2.5*51);
```

首先我们需要选中我们的PCF芯片并且给他我们的地址信息（前面提到过如果要开启DA输出，那么我们应该写入的地址），这里需要注意的就只有每次发送Byte后我们需要Wait一下，这里的0x41可以更换为0x40-0x43的任何一个，但是最好和题目中要求AD需要读取的通道一致（这里也要注意在你读取AD的时候加上0x4）

```c
void w_dac(u8 val)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x43);//add
	I2CWaitAck();
	I2CSendByte(val);
	I2CWaitAck();
	I2CStop();
}
```

### EEPROM

从上面说明的内容的我们可以知道，如果我们要对AT24C02进行操作，那么就需要对地址0xa0和0xa1进行操作

```c
void w_e2p(u8 add,u8 val)
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CSendByte(val);
	I2CWaitAck();
	I2CStop();
    //delay(10us);连续写最好延时一下
}

u8 r_e2p(u8 add)
{
	u8 tmp;
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CStop();

	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	tmp=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	return tmp;
}
```

## 超声波模板建立

### wave.c

在这里我们需要注意，超声波官方是没有给底层的，我们需要自己手搓。我们通过查询原理图可以看到，超声波的T为P10，R为P11，所以我们需要定义一下

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161626698.png" alt="image-20241216162658645" style="zoom:80%;" />

```c
sbit Tx = P1 ^ 0;
sbit Rx = P1 ^ 1;
```

现在我们来进行逻辑的书写，我们首先要知道超声波测距的原理，我发送数据后，等待接收数据，那么我通过计算发送和接收中的间隔时间，就可以得到我们的距离，这里用数学公式可以进行计算，一般来说声速都是340m/s（也不排除题目里面和你说要对其进行变换啥的【比如第十四届国赛】，这里就按照声速为340m/s来进行说明)

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161628259.png" alt="image-20241216162833214" style="zoom:67%;" />

我们这里有一个单位换算需要进行注意，t的单位为Us，也就是10-′s，我们进行完整的单位换算就可以知道d的距离了（单位为cm）

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161628101.png" alt="image-20241216162857056" style="zoom:50%;" />

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161629842.png" alt="image-20241216162943790" style="zoom:80%;" />

根据我们上方的分析，我们可以知道我们首先需要进行发送，我们在这里发送8个40kHz的方波行测量，那么周期就是1/40k=25us，也就是我们需要12us将发送的电平进行反转

```c
void de(u8 a)//a=14
{
    while (a--); // 消耗CPU周期以实现延时
}
```

然后就是我们的超声波发送函数，发送8个4kHz的方波

```c
u32 read_d()
{
    u32 d; // 定义变量d用于存储读取的数据
    u8 num = 16; // 定义变量num并初始化为16，用于计数

    TX = 0; // 将TX设置为低电平
    while (num--) // 执行一个循环，每次循环调用de函数进行延时，并切换TX电平
    {
        de(14); // 调用延时函数，延时14个周期
        TX = !TX; // 切换TX的电平状态
    }
}
```

最后是我们最重要的数据接收与转换函数的书写，在这里我们使用的pca定时器，可以节省定时器来进行其他的操作（因为ne555强制占用定时器0，串口考到就会占用一个定时器，还有一个定时器需要用来进行函数轮询，所以我们就必须用其他的定时器进行超声波的操作），其实PCA本质上还是一个定时器，所以他也存在一些定时器的参数，比如TLX→CL;THx→CH;TMOD->CMOD;TRx→>CR;TFx→→CF我们首先需要对定时器的CH和CL清零，为了我们计数做准备；然后将CMOD配置为0x00，十六位不重载定时器。

```c
CH = CL = 0;
CMOD = 0x00;
```

在我们发送超声波的时候，我们需要将总中断关闭，在我们发送完毕后就关掉，在我们发送完成后，就打开CR开始计时，等待我们收到回应（收到回应后Rx为0）或者计数溢出（溢出后CF为1），我们就关闭CR开始读取数据

```c
u32 read_d()
{
    u32 d; // 定义变量d用于存储读取的数据
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
    while (RX && CH < 0x20); // 等待RX变为0或CH达到0x20//设置脉冲上线
    CR = 0; // 设置CR为0，停止数据读取
    d = (u32)CH << 8 | CL | 156; // 计算最终的数据值//156为矫正值
    return d; // 返回读取的数据值
}
```

如果我们是因为收到回应而结束（那么就代表我们测量到了数据），那么我们就进行时间读取，如下所示，你们可能会好奇为什么+156，这是因为我们实际测量发现，+156后在远程数据（测量结果>10的时候）误差会比较小，而近程数据（测量数据<10）满足误差范围，我们也不可能测量得到0这个数据（题目中一般都不会测到）(所以我们才把他视为测量错误的返回结果)

## 串口模板建立

在这里我们需要注意，串口官方是没有给底层的，我们需要自己手搓，在这里我给的代码为重定向代码。首先是使用isp生成一个用定时器2计时的串口1的代码，波特率为4800（比赛的时候可能不是4800，要根据题目来看），定时器时钟选用12T，数据位为8位。

<img src="https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202412161645425.png" alt="image-20241216164523352" style="zoom:67%;" />

我们直接把他复制到我们代码里面就行，最后别忘记加一个EA=1来开启总中断，还有些isp版本可能比较老，没有使能中断的选项，所以需要自己加上ES=1。

```
void Uart1_Init(void) // 4800bps @ 12.000MHz
{
    SCON = 0x50;       // 串口模式1，8位数据，波特率可变
    AUXR |= 0x01;      // 串口1选择定时器2作为波特率发生器
    AUXR |= 0x04;      // 定时器2时钟工作在1T模式
    T2L = 0x8F;        // 定时初始值低字节
    T2H = 0xFD;        // 定时初始值高字节
    AUXR |= 0x10;      // 启动定时器2
    ES = 1;            // 使能串口1中断
}
```

------

### 串口中断服务函数：

`Uart1_Isr` 是串口1的中断服务函数，处理接收和发送中断。

**代码：**

```c
void Uart1_Isr(void) interrupt 4
{
    if (TI)  // 检测发送中断
    {
        TI = 0;  // 清除发送中断标志位
    }
    if (RI)  // 检测接收中断
    {
        RI = 0;  // 清除接收中断标志位
        uu.buf[uu.i++] = SBUF;  // 读取接收数据，存入缓冲区
        uu.f = 1;   // 标记接收完成
        uu.ton = 1; // 设置接收标志位
    }
}
```

**说明：**

- **`RI`**：接收中断，表示数据已接收完成，`SBUF` 保存接收到的数据。
- **`TI`**：发送中断，表示数据发送完成。
- **`uu.buf`**：存储接收到的数据。
- **`uu.i`**：接收缓冲区的索引。

------

### UART 状态初始化函数：

`uu_init` 用于初始化 UART 相关状态变量和缓冲区。添加缓存区初始化代码。

**代码：**

```c
void uu_init()
{
    memset(uu.buf, 0, uu. i); // 清空接收数据
    uu.f = 0;   // 清除接收标志
    uu.i = 0;   // 清除缓冲区索引
    uu.t = 0;   // 清除时间标志
    uu.ton = 0; // 清除接收标志位
    uu.cmd = 0; // 清除命令
}
```

## init初始化模版建立

我们在启动开发板的时候最好对其进行初始化，关闭蜂鸣器和继电器，MOTOR和全部LED灯，避免上电情况有误。

代码如下：

```c
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
```

## main主函数书写

### 内存优化

为什么要内存优化，为了区分程序之间的差距，代码运行效率也是需要做到的。

首先在Targe魔术棒中，绝对不能使用xdata，最基本的 data是效率最高的，

![image-20250122201837096](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202501222018175.png)

其次，声明普通变量最好都统一使用 ：**idata**  。

在小于128字节的范围内，idata就是data，也就是最高效率，在128~256字节内就会变成较高效率的idata。

```
例如：
idata struct _cj cj;
static idata u8 key_state = 0; // 状态机状态
idata u8 i,j,tmp=0; 
```

在bool类型，也就是变量只有0与1 的变化，统一使用 bit 作为关键字。

```
例如：
idata bit f_wei=0;//smg闪烁标志
```

**注意：**

1. 时刻关注程序编译之后 的大小。Program Size

   ![](https://z1r343l-001.obs.cn-north-4.myhuaweicloud.com/img/202501222024970.png)

2. Program Size 中的data 不能大于 230 !如果大于230，那么data区的空间将严重不足，会导致堆栈溢出，程序跑飞的情况，所以在data大概210的时候，可以采取将部分不需要频繁读取的变量 存放在xdata中。

   ```
   例如:
   static xdata u8 key_tick = 0;// 长按计时器
   ```

   

### main.c

现在我们来进行主函数的编写

### 全部变量的归纳

变量声明在all.h里面

#### 什么是结构体？

结构体是一种自定义的数据类型，用于将一组相关数据组合在一起。

在嵌入式系统中，使用结构体可以方便地管理传感器读数、系统状态以及其他需要集中存储的参数。

全局变量说明

```c
extern xdata struct _cj cj;
```

**`xdata`**：

- 表示结构体存储在 **外部RAM（XRAM）** 中。
- 适用于C51单片机，节省片内RAM资源。

**`extern`**：

- 声明变量为外部引用，即结构体 **`cj`** 在其他文件中定义，当前文件可以访问它。

```c
// 定义一个结构体用于存储测量数据
xdata struct _cj
{
    u8 state; // 总状态
    u8 h; // 时
    u8 m; // 分
    u8 s; // 秒
    float t; // 温度
    u8 d; // 距离
    u8 ad; // 月份
    u8 key; // 按键值
    u8 adc; // 模拟数字转换值
    u32 freq; //频率
};
extern xdata struct _cj cj;
```

### Key键盘模块

这个模块是重中之重，如果要在代码里面判断按键按下，通常这样写：

```c
void key_proc()
{
    key_scan();// 执行键盘扫描
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
    }
    key_code=0;// 重置按键代码以准备下一次键盘事件
}
```

### 数码管模块

使用sprintf处理字符串，然后提取字符串来显示smg

```c
void smg_proc()
{
    switch(cj.state)                                // 根据状态选择不同的显示格式
    {
        case 10:
                sprintf(dis_str,"F%02bu-%02bubb",cj.m,cj.s); // 格式化字符串，显示频率和时间
        break;
    }
    disp_proc();                                    // 调用 disp_proc 函数进行显示处理
}
```

### Led灯模块

```c
void led_proc()
{
    led_dat=0;
    switch(cj.state)
    {
        case 10:
            // 当状态为10时，设置L1和L3常亮，L2和L4闪烁
            L1=L3=1;
            L2=L4=f_on;
        break;
    }
}
```

### Uart串口模块

我们在这里使用了串口超时解析，即40ms内没有收集到串口的信息的时候我们才进入并进行解析，在我们解析结束后使用 uu.cmd = 2（为了防止后续解析被干扰），为下一次接收解析进行准备。

基础代码：
```c
// 如果uu.ton标志位为真，则执行定时关断逻辑
if (uu.ton) {
    if (++uu.t >= 20) {
        uu.cmd = 2; // 设置命令为2
    }
}
```

使用示例，这里发送小写的"A\r\n"返回hello，注意一下，判断的时候用的是单引号而不是双引号：

```c
void uart_proc()
{
    if(uu.f)// 检查是否有待处理的数据
    {
        uu.f=0;
        switch(uu.i)// 根据接收的数据长度决定如何处理
        {
            case 3:
                // 当接收的数据以'A'开头，后面紧跟回车和换行符时，设置命令为1
                if(uu.buf[0]=='A'&&uu.buf[1]=='\r'&&uu.buf[2]=='\n')
                    uu.cmd=1;
                else
                    uu.cmd=2; // 否则设置命令为2，表示接收到的数据不符合预期格式
                break;
        }
    }
    if(uu.cmd)// 如果有命令待执行
    {
        ES=0;TI=1;// 禁用串口中断，防止在发送过程中被中断
        switch(uu.cmd)// 根据命令执行相应的操作
        {
            case 1:printf("hello\r\n");break;
            case 2:printf("error\r\n");break;
        }
        ES=1;TI=0;// 重新启用串口中断，准备接收下一次数据
        uu_init();// 重置UART相关的状态和缓冲区
    }
}
```

### 定时器0初始化（NE555模块）

这个模块是最有意思的，首先我们需要注意一下，P34和singa的跳线帽要插在一起，并且按键方面需要删除P34，然后才能开始测。这里依然用isp生成一下初始化的定时器0，但是这里要注意，新版的不能生成0ms，我们就随便生成个时间，然后按照我这样勾选后复制粘贴就行。

我们需要将TL0和TH0的值改为0，并且在TMOD = 0xF0;下面对其进行配置 TMOD = 0x05;将其变为十六位不自动重载（这里具体可以参考手册里面对于定时器的配置，我之前的视频有提到蓝桥杯单片机ne555模块），这里别忘了EA开启总中断（这是我的习惯），频率的读取放在定时器1那里讲。

```c
void Timer0_Init(void)
{
  AUXR &= 0x7F; // 定时器时钟1ms 12T模式
  TMOD &= 0xF0; // 设置定时器模式
  TMOD |= 0x05; // 设置为模式5（定时器模式）//计数器模式
  TL0 = 0;      // 设置初始值
  TH0 = 0;      // 设置初始值
  TF0 = 0;      // 清除TF0标志
  TR0 = 1;      // 启动定时器0
}
```

### 定时器1初始化

这是我们轮询的定时器的初始化代码，我们使用isp生成2ms定时器1的代码（记得勾选上使能），老版本没有使能，所以记得加上ET1 = 1开启。

```c
void Timer1_Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x40;				//设置定时初始值
	TH1 = 0xA2;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;
}
```

### 定时器1中断（重点）

因为定时器1中断会打断其他模块时序，所以定时器1中断要快进快出。在不需要pwm进行led调光的时候需要把Led_scan()函数放到Led_proc()函数里执行，以降低中断执行时间，然后在中断函数里尽量避免乘法，除法，取余运算的耗时的操作。

我们在这里放了一堆东西，这里我做一下说明，我们每个中断都有对应的中断号（可以在官方的手册查到），这里定时器1的中断号是3。

然后是我们的一个全局定时器。在这个定时器里面我们对频率进行了读取，将TH0 << 8 | TL0的结果给了Freq（注意一下Freq的类型要是unsigned int，不然会溢出），在我们读取完数据后，将TH0和TL0进行了置0手动重载，方便下一次进行测量。

基础代码：
```c
void pit1() interrupt 3
{
    uwTick++;//系统总运行时长
    disp_scan(); // 调用数码管扫描函数
    tim_proc();  // 调用时间处理函数
}

tim_proc:
    // 每10次中断（即每20ms）执行一次LED扫描
    if (++cnt_20ms >= 10) {
        cnt_20ms = 0;
        led_scan(); // 调用LED扫描函数
    }

    // 每400次中断（即每800ms）切换f_wei和f_on的状态
    if (++cnt_800ms >= 400) {
        cnt_800ms = 0;
        f_wei = !f_wei; // 切换f_wei状态
        f_on = !f_on;   // 切换f_on状态
    }
    
    // 每500次中断（即每1s）更新频率计数并清空定时器0
    if (++cnt_1s >= 500) {
        cnt_1s = 0;
        cj.freq = (TH0 << 8) | TL0; // 计算频率
        TH0 = TL0 = 0;           // 清空定时器0
    }
```

### 串口中断（串口处理）

我们第一行就是判断是否RI为1，因为RI为1则代表有信息过来了，当我们收集到信息时，我们将uu.f置为1，uu.ton置为1，开始嘀嗒计时，当我们40ms没有接收到新的数据的时候，我们就认为数据接收完成，可以开始进行串口处理。

后面的接收就不需要多讲了，我们是一位一位进行接收的，而读写都是用的SBUF这个缓冲区（串口1，如果是串口2则是S2BUF，但是串口2不会考，所以这里不用管），我们接收完成后，将RI置为0，为下一次接收做准备。

```c
void Uart1_Isr(void) interrupt 4
{
	if (TI)				//检测串口1发送中断
	{TI = 0;}			//清除串口1发送中断请求位
	if (RI)				//检测串口1接收中断
	{
		RI = 0;			//清除串口1接收中断请求位
		uu.buf[uu.i++]=SBUF;
		uu.f=1;
		uu.ton=1;
	}
}
```

### 任务调度器的使用

这个方法主要是知道就行，对于比赛最好采取下面的中断标志位的方法

采用了全局变量uwTick来计时，在定时器中断里每2毫秒自增一，调度器通过检查每个任务的上次运行时间和当前时间，来决定是否执行任务，这种设计可以确保任务按照预定的周期执行。只要满足了运行的条件，该任务就可以被执行。

代码详细看注释：

```c
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
```

### 中断标志位的使用

蓝桥杯重要的还是简洁，尽快实现，节省下来时间解决其它问题，所以比赛来说推荐使用中断标志位的方法，

在2ms定时器中断中判断标志位。

```
void pit1() interrupt 3//2ms中断
{
	disp_scan();
	tim_proc();//定时器标志位处理
}

void tim_proc()
{
	static u8 cnt_20ms=0;
	static u8 cnt_100ms=0;
	static u16 cnt_800ms=0;
	
	if(++cnt_20ms>=10){
		cnt_20ms=0;
		f_20ms_key=1;f_20ms_rtc=1;
		led_proc();
	}
	if(++cnt_100ms>=50){
		cnt_100ms=0;
		f_100ms_state=1;f_100ms_led=1;
		f_100ms_wave=1;f_100ms_adc=1;
	}
}
```

运行函数例子：

其adc函数每100ms执行一次

```
void adc_mainloop()
{
	if(!f_100ms_adc)return;
	f_100ms_adc=0;
	cj.ad=read_ad();
}
```

### 内存分配问题

实际上iap15单片机的data只有128个字节，在引入串口数据解析，和格式化输出函数printf等以后是很容易内存不够导致编译不通过的。要解决这个问题就要了解iap15单片机的几种存储介质的大小和特性等，详细特性见下表：(访问速度以连续写入30个char数据的时间计算)

| 名称  | 大小    | 访问速度 | 备注                                 |
| ----- | ------- | -------- | ------------------------------------ |
| data  | 128字节 | 33微秒   | data为直接寻址                       |
| idata | 256字节 | 33微秒   | idata为间接寻址，使用不要超过128字节 |
| xdata | 64k字节 | 50微秒   | xdata为外部存储，访问速度较慢        |
| pdata | 128字节 | 38微秒   | pdata为xdata的当前页，是xdata的子集  |

分析上表可知，data和idata的读写速度最快，编写代码时推荐优先写满data的128个字节，再考虑idata的其中的128个字节，实在不够用再考虑pdata,外部存储的使用可能会降低系统运行速度，且pdata和xdata作为全局变量使用时不默认为0，需要初始化。

### main主函数书写

这是我们的一个最初的函数，没什么好说明的，就是需要进行一些初始化而已，但是顺序不要写错了，首先是系统初始化，然后是定时器0（ne555），最后是定时器1（轮询）的初始化。

你可以发现，我在这里进行了一次温度的空读取，因为温度上电后会显示85（温度转换的时间），如果在后面才给他开始温度转换的话，那么很有可能跳85，导致测量结果错误，这里的延时函数也是用isp直接生成的，注意系统时间别选错了就行。

```c
//运行调度器
void main()
{
    init();// 系统初始化
    EA=1;// 打开中断，开始正常运行
    while(1)
    {
        Scheduler_Run();// 运行调度器
    }
}
/*或者
使用中断标志位
*/
void main()
{
 	init();
	EA=1;
	while(1)
	{
		state_mainloop();
		key_mainloop();
		rtc_mainloop();
		wave_mainloop();
		adc_mainloop();
		temp_mainloop();
		uart_mainloop();
		led_mainloop();
	}
}
```

### all.h 

最后是all.h文件，它包含了所有需要的头文件：

```c
// 防止多次包含的头文件保护宏
#ifndef ALL_H
#define ALL_H

// 包含微控制器STC15F2K60S2的头文件，提供特殊功能寄存器的定义
#include <STC15F2K60S2.H>
// 包含标准输入输出库头文件
#include "stdio.h"
// 包含编译器提供的内建函数头文件
#include "intrins.h"

// 定义无符号字符类型别名u8
typedef unsigned char u8;
// 定义无符号整数类型别名u32
typedef unsigned int u32;

// 包含其他模块或功能的头文件
#include "time.h"
#include "ctrl.h"
#include "smg.h"
#include "iic.h"
#include "onewire.h"
#include "rtc.h"
#include "key.h"
#include "wave.h"
#include "uart.h"
#include "led.h"

// 定义控制命令的地址
#define CTRL 0xa0
// 定义LED显示的地址
#define LED 0x80
// 定义位选显示的地址
#define WEI 0xc0
// 定义段显示的地址
#define DUAN 0xe0
// 定义关闭门控的操作
#define CLOSE() P2&=0x1f
// 定义打开指定门控的操作
#define OPEN(a) P2|=a

// 定义继电器控制信号
#define JDQ 1<<4
// 定义电机控制信号
#define MOTO 1<<5
// 定义蜂鸣器控制信号
#define FMQ 1<<6

// 定义一个结构体用于存储测量数据
xdata struct _cj
{
    u8 state; // 总状态
    u8 h; // 时
    u8 m; // 分
    u8 s; // 秒
    float t; // 温度
    u8 d; // 距离
    u8 ad; // 月份
    u8 key; // 按键值
    u8 adc; // 模拟数字转换值
    u32 freq; //频率
};
extern xdata struct _cj cj;

#endif 
```

这就是全部的代码模板内容。每个部分都有其特定的功能和用途，通过合理的组织和调用，可以实现各种单片机的功能需求。

这些代码模板是为蓝桥杯比赛准备的，包含了大部分常用功能的实现。在实际使用时，可以根据具体需求进行修改和调整。
