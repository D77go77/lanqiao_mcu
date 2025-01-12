
# 蓝桥杯单片机设计与开发模板

本仓库旨在为蓝桥杯单片机开发者提供一个完整的模板，帮助快速实现比赛需求。

## 功能模块

- **蜂鸣器与继电器控制**: 实现对蜂鸣器和继电器的开关控制。
- **实时时钟（RTC）与时间管理**: 包含实时时钟模块的初始化与运行逻辑，支持时间读取和设置。
- **传感器与 AD 转换**: 支持温度传感器和 AD 模块的初始化与数据采集。
- **数码管与 LED 显示**: 包括数码管驱动和 LED 的简单控制。
- **按键处理**: 按键状态检测与去抖动功能。
- **串口通信与波形数据**: 支持通过 UART 串口与外部设备通信，包含波形数据处理功能，可用于传感器信号的分析。
- **调度器**: 简单的任务调度器，用于管理多任务执行。

## 目录结构

```plaintext
lanqiao_mcu/
├── .vscode/                        # VS Code 配置
├── Listings/                       # 编译输出文件
├── Objects/                        # 中间文件
├── main.c                          # 主程序入口
├── all.h                           # 公共头文件
├── ctrl.c / ctrl.h                 # 控制模块
├── rtc.c / rtc.h                   # 实时时钟模块
├── sensor.c / sensor.h             # 传感器模块
├── display.c / display.h           # 显示模块
├── key.c / key.h                   # 按键处理模块
├── uart.c / uart.h                 # 串口通信模块
├── waveform.c / waveform.h         # 波形数据处理模块
├── scheduler.c / scheduler.h       # 调度器模块
└── keilclean.bat                   # Keil 清理脚本
```

## 快速开始

1. **环境准备**:
   - 安装 [Keil uVision](https://www.keil.com/download/product/) 开发环境。
   - 确保已配置好相应的编译器和调试器。

2. **克隆仓库**:
   ```bash
   git clone https://github.com/D77go77/lanqiao_mcu.git
   ```

3. **打开项目**:
   - 在 Keil uVision 中，打开 `lanqiao_mcu.uvproj` 项目文件。

4. **编译和下载**:
   - 在 Keil 中编译项目，确保无错误。
   - 使用调试器将程序下载到目标单片机中。

5. **运行和测试**:
   - 根据需要，修改 `main.c` 或各模块的代码，以实现特定功能。
   - 运行程序，测试各模块功能是否符合预期。

## 许可证

本项目基于 GPL-3.0 许可证进行分发。有关详细信息，请参阅 [LICENSE](./LICENSE) 文件。

## 贡献

欢迎 Fork 本仓库并提交 Pull Request，贡献新的功能模块或改进代码结构。

