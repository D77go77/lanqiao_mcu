# 蓝桥杯单片机模板项目

本仓库为蓝桥杯单片机比赛提供的基础模板项目，包含多种功能模块和硬件接口的实现，适用于快速开发和扩展单片机应用。

---

## 项目功能模块

1. **蜂鸣器与继电器控制**:
   - 支持对蜂鸣器和继电器的开关控制

2. **实时时钟（RTC）与时间管理**:
   - 包含实时时钟模块的初始化与运行逻辑，支持时间读取和设置。

3. **传感器与 AD 转换**:
   - 支持温度传感器和 AD 模块的初始化与数据采集。

4. **数码管与 LED 显示**:
   - 包括数码管驱动 和 LED 的简单控制。

5. **按键处理**:
   - 按键状态检测与去抖动功能。

6. **串口通信与波形数据**:
   - 支持通过 UART 串口与外部设备通信。
   - 波形数据处理功能，可用于传感器信号的分析。

7. **调度器**:
   - 简单的任务调度器，用于管理多任务执行。

---

## 项目目录结构

```
lanqiao_mcu/
│
├── .vscode/                        # VS Code 配置
├── Listings/                       # 编译输出文件
├── Objects/                        # 中间文件
├── main.c                          # 主程序入口
├── all.h                           # 公共头文件
├── 功能模块:                       # 以下为各功能模块的源文件和头文件
│   ├── ctrl.c / ctrl.h             # 控制逻辑
│   ├── iic.c / iic.h               # I2C 通信
│   ├── key.c / key.h               # 按键处理
│   ├── led.c / led.h               # LED 控制
│   ├── onewire.c / onewire.h       # 单总线通信（如温湿度传感器）
│   ├── rtc.c / rtc.h               # 实时时钟
│   ├── smg.c / smg.h               # 数码管显示
│   ├── time.c / time.h             # 时间管理
│   ├── uart.c / uart.h             # 串口通信
│   ├── wave.c / wave.h             # 波形数据处理
├── test.uvproj / test.uvopt        # Keil 工程文件
```

---

## 快速开始

### 环境要求
- 硬件: 蓝桥杯单片机开发板
- 软件: Keil IDE 或 VS Code + GCC 工具链

### 编译与运行
1. 克隆仓库：
   ```bash
   git clone https://github.com/D77go77/lanqiao_mcu.git
   ```
2. 打开 `test.uvproj` 文件（Keil 工程）。
3. 根据目标硬件选择编译器和烧录工具。
4. 编译并烧录到开发板。

---

## 功能说明

### 1. 系统初始化
在 `main.c` 文件中，初始化包含以下内容：
- 蜂鸣器与继电器的初始状态设置。
- 实时时钟、AD 转换和传感器初始化。
- 调度器、串口和定时器配置。

### 2. 调度器
项目使用一个简单的任务调度器，以下是相关函数：
- `Scheduler_Init()`:
  - 初始化任务调度器。
- `Scheduler_Run()`:
  - 在主循环中运行，定期执行任务。

### 3. 功能模块
各功能模块文件（如 `led.c`、`rtc.c`）实现了具体的硬件接口和逻辑，调用方式可参考 `main.c`。

---

## 许可证

本仓库基于 [GPLv3许可证](LICENSE) 发布。

---

## 贡献

欢迎 Fork 本仓库并提交 Pull Request，贡献新的功能模块或改进代码结构。

---

本项目旨在为蓝桥杯单片机开发者提供一个完整的模板，帮助快速实现比赛需求。
```
