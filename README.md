# 嵌入式日常实验作业
## 项目简介
本项目基于 **STM32F103ZET6** 开发板，使用 **STM32CubeMX** 生成 HAL 库初始化代码，并在 **FreeRTOS** 环境下，集成了嵌入式课程日常实验作业的 **11 个基础实验**。项目采用 **CMake + GCC (arm-none-eabi)** 构建系统，配合 **VSCode** 编辑器进行开发、编译与调试。

**特别说明**：FreeRTOS 通过 CubeMX 内置的 **CMSIS-RTOS v2** 封装层进行调用，相关操作均使用 CMSIS-RTOS v2 标准 API。

## 注意事项
CH1116屏幕接I2C1(PA6, PA7)，I2C地址为0x78，在<kbd>oled.c</kbd>中可进行I2C地址和句柄的修改

串口使用USART1(PA9, PA10)，波特率9600

ADC提供参考电压(REF+接3V3)

HAL时基：TIM7

硬件延时：TIM6

## 任务与队列

| 任务        | 主要功能               |
| ----------- | ---------------------- |
| BtnTask     | 按键消抖与按键事件检测 |
| LEDTask     | 控制LED显示            |
| UITask      | 控制OLED屏幕显示       |
| SettingTask | 切换实验状态           |
| UARTTask    | 串口发送数据           |
| ADTask      | AD转换光照与温度       |
| SwitchTask  | 切换显示状态           |
| TickTask    | 滴答定时器计数         |

| 队列         | 主要功能             |
| ------------ | -------------------- |
| LightQueue   | 存放AD光照转换平均值 |
| TempQueue    | 存放AD温度转换平均值 |
| SwitchQueue  | 存放串口数据结构体   |
| LightSwQueue | 存放AD光照转换平均值 |

## 项目目录结构
```text
├───CMakeLists.txt
├───DailyEXP.ioc
│
├───cmake
│   ├───gcc-arm-none-eabi.cmake
│   ├───starm-clang.cmake
│   │
│   └───stm32cubemx
│           CMakeLists.txt
│
├───Core
│   ├───Inc
│   │       adc.h
│   │       btn.h
│   │       display_msg.h
│   │       dma.h
│   │       font.h
│   │       FreeRTOSConfig.h
│   │       gpio.h
│   │       i2c.h
│   │       led.h
│   │       main.h
│   │       oled.h
│   │       stm32f1xx_hal_conf.h
│   │       stm32f1xx_it.h
│   │       tim.h
│   │       usart.h
│   │
│   └───Src
│           adc.c
│           btn.c
│           dma.c
│           font.c
│           freertos.c
│           gpio.c
│           i2c.c
│           led.c
│           main.c
│           oled.c
│           stm32f1xx_hal_msp.c
│           stm32f1xx_hal_timebase_tim.c
│           stm32f1xx_it.c
│           syscalls.c
│           sysmem.c
│           system_stm32f1xx.c
│           tim.c
│           usart.c
│
├───Drivers
│   ├───CMSIS
│   └───STM32F1xx_HAL_Driver
│
└───...
```
