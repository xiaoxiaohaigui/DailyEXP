/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "font.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "btn.h"
#include "display_msg.h"
#include "led.h"
#include "oled.h"
#include "usart.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/_intsup.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BTN_DEBOUNCE_DELAY 40    // 消抖延时，单位为ms
#define BTN_LONG_PRESS_DELAY 500 // 长按判定时间，单位为ms
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for BtnTask */
osThreadId_t BtnTaskHandle;
const osThreadAttr_t BtnTask_attributes = {
    .name = "BtnTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
    .name = "LEDTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for UITask */
osThreadId_t UITaskHandle;
const osThreadAttr_t UITask_attributes = {
    .name = "UITask",
    .stack_size = 384 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for SettingTask */
osThreadId_t SettingTaskHandle;
const osThreadAttr_t SettingTask_attributes = {
    .name = "SettingTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for UARTTask */
osThreadId_t UARTTaskHandle;
const osThreadAttr_t UARTTask_attributes = {
    .name = "UARTTask",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for ADTask */
osThreadId_t ADTaskHandle;
const osThreadAttr_t ADTask_attributes = {
    .name = "ADTask",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for SwitchTask */
osThreadId_t SwitchTaskHandle;
const osThreadAttr_t SwitchTask_attributes = {
    .name = "SwitchTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for TickTask */
osThreadId_t TickTaskHandle;
const osThreadAttr_t TickTask_attributes = {
    .name = "TickTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for LightQueue */
osMessageQueueId_t LightQueueHandle;
const osMessageQueueAttr_t LightQueue_attributes = { .name = "LightQueue" };
/* Definitions for TempQueue */
osMessageQueueId_t TempQueueHandle;
const osMessageQueueAttr_t TempQueue_attributes = { .name = "TempQueue" };
/* Definitions for SwitchQueue */
osMessageQueueId_t SwitchQueueHandle;
const osMessageQueueAttr_t SwitchQueue_attributes = { .name = "SwitchQueue" };
/* Definitions for LightSwQueue */
osMessageQueueId_t LightSwQueueHandle;
const osMessageQueueAttr_t LightSwQueue_attributes = { .name = "LightSwQueue" };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBtnTask(void *argument);
void StartLEDTask(void *argument);
void StartUITask(void *argument);
void StartSettingTask(void *argument);
void StartUARTTask(void *argument);
void StartADTask(void *argument);
void StartSwitchTask(void *argument);
void StartTickTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void
MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the queue(s) */
    /* creation of LightQueue */
    LightQueueHandle = osMessageQueueNew(2, sizeof(uint16_t), &LightQueue_attributes);

    /* creation of TempQueue */
    TempQueueHandle = osMessageQueueNew(2, sizeof(uint16_t), &TempQueue_attributes);

    /* creation of SwitchQueue */
    SwitchQueueHandle = osMessageQueueNew(2, sizeof(UartTxMsg_t), &SwitchQueue_attributes);

    /* creation of LightSwQueue */
    LightSwQueueHandle = osMessageQueueNew(2, sizeof(uint16_t), &LightSwQueue_attributes);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of BtnTask */
    BtnTaskHandle = osThreadNew(StartBtnTask, NULL, &BtnTask_attributes);

    /* creation of LEDTask */
    LEDTaskHandle = osThreadNew(StartLEDTask, NULL, &LEDTask_attributes);

    /* creation of UITask */
    UITaskHandle = osThreadNew(StartUITask, NULL, &UITask_attributes);

    /* creation of SettingTask */
    SettingTaskHandle = osThreadNew(StartSettingTask, NULL, &SettingTask_attributes);

    /* creation of UARTTask */
    UARTTaskHandle = osThreadNew(StartUARTTask, NULL, &UARTTask_attributes);

    /* creation of ADTask */
    ADTaskHandle = osThreadNew(StartADTask, NULL, &ADTask_attributes);

    /* creation of SwitchTask */
    SwitchTaskHandle = osThreadNew(StartSwitchTask, NULL, &SwitchTask_attributes);

    /* creation of TickTask */
    TickTaskHandle = osThreadNew(StartTickTask, NULL, &TickTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartBtnTask */
/**
 * @brief  Function implementing the BtnTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartBtnTask */
void
StartBtnTask(void *argument)
{
    /* USER CODE BEGIN StartBtnTask */

    uint32_t btnPressCounter; // 按键按下计数器
    uint8_t debounceCounter;  // 消抖计数器s

    /* Infinite loop */
    for(;;)
    {
        // 读取按键状态
        uint8_t btnPressed = HAL_GPIO_ReadPin(WKUP_GPIO_Port, WKUP_Pin) == GPIO_PIN_RESET;
        switch(btnState)
        {
            case BTN_STATE_IDLE:
                if(btnPressed)
                {
                    btnState = BTN_STATE_DEBOUNCE;
                    debounceCounter = 0;
                }
                break;

            case BTN_STATE_DEBOUNCE:
                if(btnPressed)
                {
                    debounceCounter++;
                    if(debounceCounter >= BTN_DEBOUNCE_DELAY / 20)
                    { // 每20ms检查一次
                        btnState = BTN_STATE_PRESSED;
                        btnPressCounter = 0;
                    }
                }
                else
                {
                    btnState = BTN_STATE_IDLE; // 按键抖动，回到空闲状态
                }
                break;

            case BTN_STATE_PRESSED:
                if(btnPressed)
                {
                    btnPressCounter++;
                    if(btnPressCounter >= BTN_LONG_PRESS_DELAY / 20)
                    {
                        btnEvent = BTN_EVENT_LONG_PRESS;   // 长按事件
                        btnState = BTN_STATE_WAIT_RELEASE; // 等待释放
                    }
                }
                else
                {
                    btnEvent = BTN_EVENT_CLICK; // 点击事件
                    btnState = BTN_STATE_IDLE;  // 回到空闲状态
                }
                break;

            case BTN_STATE_WAIT_RELEASE:
                if(!btnPressed)
                {
                    btnState = BTN_STATE_IDLE; // 按键释放，回到空闲状态
                }
                break;
        }
        osDelay(20);
    }
    /* USER CODE END StartBtnTask */
}

/* USER CODE BEGIN Header_StartLEDTask */
/**
 * @brief Function implementing the LEDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLEDTask */
void
StartLEDTask(void *argument)
{
    /* USER CODE BEGIN StartLEDTask */
    /* Infinite loop */
    for(;;)
    {
        // 实验一，软件流水灯
        if(mode == NORMAL_MODE && expState == EXP1)
        {
            LED_Flow(led_flow_counter % 16);
            led_flow_counter++;

            // 软件延时
            for(volatile uint32_t i = 0; i < 500000; i++)
                ;
        }
        else if(mode == NORMAL_MODE && expState == EXP2)
        // 实验二，定时器流水灯，流水灯由定时器中断控制，主循环中无需执行点亮函数，只需根据计数器更新LED状态
        {
            LED_Flow(led_flow_counter);
        }
        else if(expState == EXP3) // 实验三，按键加法器，结果以二进制形式显示在LED上
        {
            static uint16_t BtnBinaryCounter = 0;
            if(mode == NORMAL_MODE)
            {
                LED_Binary_Display(BtnBinaryCounter);
                // 按键实现二进制计数器加1
                if(btnEvent == BTN_EVENT_CLICK)
                {
                    BtnBinaryCounter++;
                    btnEvent = BTN_EVENT_NONE; // 清除事件
                }
            }
            else
            {
                // 清空计数器
                BtnBinaryCounter = 0;
            }
        }
        else if(expState == EXP4) // 实验四，定时器半秒计数，结果以二进制形式显示在LED上
        {
            if(mode == NORMAL_MODE)
            {
                LED_Binary_Display(TimerBinaryCounter /
                                   5); // 定时器每100ms产生一次中断，计数值除以5实现半秒计数
            }
            else
            {
                // 清空计数器
                TimerBinaryCounter = 0;
            }
        }

        // 进入设置模式时，关闭所有LED，准备下一次实验
        if(mode == SETTING_MODE)
        {
            LED_Turn_off_All();
        }

        osDelay(10);
    }
    /* USER CODE END StartLEDTask */
}

/* USER CODE BEGIN Header_StartUITask */
/**
 * @brief Function implementing the UITask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUITask */
void
StartUITask(void *argument)
{
    /* USER CODE BEGIN StartUITask */
    // 初始化OLED显示
    osDelay(20);
    OLED_Init();

    // 光标闪烁计数
    uint8_t cursorBlinkCounter = 0;

    /* Infinite loop */
    for(;;)
    {
        // 显示
        OLED_NewFrame();

        char str_exp[16];
        snprintf(str_exp, sizeof(str_exp), "Exp: %2d", expState + 1);

        OLED_PrintASCIIString(0, 0, str_exp, &afont16x8, OLED_COLOR_NORMAL);

        // 若处于设置模式，则显示闪烁的光标
        if(mode == SETTING_MODE)
        {
            cursorBlinkCounter = (cursorBlinkCounter + 1) % 8; // 每8次循环切换一次状态
            if(cursorBlinkCounter < 4)
            {
                OLED_DrawLine(0, 18, 56, 18, OLED_COLOR_NORMAL); // 绘制光标
            }
        }
        else
        {
            cursorBlinkCounter =
              0; // 非设置模式时重置计数器，确保进入设置模式时光标从可见状态开始闪烁
        }

        // 若当前实验需要切换显示，则根据实验状态显示对应信息
        if(mode == NORMAL_MODE && (expState >= EXP8 && expState <= EXP10))
        {
            // 显示串口发送类型
            char str_uart[16];
            char str_count_light[8];
            if(DisplayType == DISPLAY_COUNT)
            {
                if(expState == EXP8 || expState == EXP10)
                {
                    snprintf(str_uart, sizeof(str_uart), "HostDisplay:");
                    OLED_PrintASCIIString(0, 26, str_uart, &afont16x8, OLED_COLOR_NORMAL);
                    snprintf(str_count_light, sizeof(str_count_light), "Count");
                    OLED_PrintASCIIString(0, 42, str_count_light, &afont16x8, OLED_COLOR_NORMAL);
                }
                else if(expState == EXP9)
                {
                    // 实验九为LED和上位机切换显示计数值
                    snprintf(str_uart, sizeof(str_uart), "LedDisplay:");
                    OLED_PrintASCIIString(0, 26, str_uart, &afont16x8, OLED_COLOR_NORMAL);
                    snprintf(str_count_light, sizeof(str_count_light), "Count");
                    OLED_PrintASCIIString(0, 42, str_count_light, &afont16x8, OLED_COLOR_NORMAL);
                }
            }
            else if(DisplayType == DISPLAY_LIGHT)
            {
                if(expState == EXP10)
                {
                    snprintf(str_uart, sizeof(str_uart), "HostDisplay:");
                    OLED_PrintASCIIString(0, 26, str_uart, &afont16x8, OLED_COLOR_NORMAL);
                    snprintf(str_count_light, sizeof(str_count_light), "Light");
                    OLED_PrintASCIIString(0, 42, str_count_light, &afont16x8, OLED_COLOR_NORMAL);
                }
                else if(expState == EXP8)
                {
                    // 实验八为LED和上位机切换显示计数值，此处借用DISPLAY_LIGHT状态
                    snprintf(str_uart, sizeof(str_uart), "LedDisplay:");
                    OLED_PrintASCIIString(0, 26, str_uart, &afont16x8, OLED_COLOR_NORMAL);
                    snprintf(str_count_light, sizeof(str_count_light), "Count");
                    OLED_PrintASCIIString(0, 42, str_count_light, &afont16x8, OLED_COLOR_NORMAL);
                }
                else if(expState == EXP9)
                {
                    // 实验九为LED和上位机切换显示计数值
                    snprintf(str_uart, sizeof(str_uart), "LedDisplay:");
                    OLED_PrintASCIIString(0, 26, str_uart, &afont16x8, OLED_COLOR_NORMAL);
                    snprintf(str_count_light, sizeof(str_count_light), "Light");
                    OLED_PrintASCIIString(0, 42, str_count_light, &afont16x8, OLED_COLOR_NORMAL);
                }
            }
        }
        else if(mode == NORMAL_MODE && (expState >= EXP1 && expState <= EXP4))
        {
            char str_led[16];
            snprintf(str_led, sizeof(str_led), "LedDisplay");
            OLED_PrintASCIIString(0, 26, str_led, &afont16x8, OLED_COLOR_NORMAL);
        }
        else if(mode == NORMAL_MODE && (expState >= EXP5 && expState <= EXP7))
        {
            char str_uart[16];
            snprintf(str_uart, sizeof(str_uart), "HostDisplay");
            OLED_PrintASCIIString(0, 26, str_uart, &afont16x8, OLED_COLOR_NORMAL);
        }
        else if(mode == NORMAL_MODE && expState == EXP11)
        {
            char str_both[16];
            snprintf(str_both, sizeof(str_both), "BothDisplay");
            OLED_PrintASCIIString(0, 26, str_both, &afont16x8, OLED_COLOR_NORMAL);
        }

        OLED_ShowFrame();

        osDelay(100);
    }
    /* USER CODE END StartUITask */
}

/* USER CODE BEGIN Header_StartSettingTask */
/**
 * @brief Function implementing the SettingTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSettingTask */
void
StartSettingTask(void *argument)
{
    /* USER CODE BEGIN StartSettingTask */
    /* Infinite loop */
    for(;;)
    {
        // 根据按键事件切换实验状态，点击切换到下一个实验，长按进入设置模式
        if(btnEvent == BTN_EVENT_CLICK && mode == SETTING_MODE)
        {
            expState = (expState + 1) % 11; // 循环切换实验状态
            btnEvent = BTN_EVENT_NONE;      // 清除事件
        }
        else if(btnEvent == BTN_EVENT_LONG_PRESS)
        {
            mode = (mode == NORMAL_MODE) ? SETTING_MODE : NORMAL_MODE; // 切换模式
            btnEvent = BTN_EVENT_NONE;                                 // 清除事件
        }
        else
        {
            // 正常模式下根据实验状态执行对应功能，若当前实验不需要按键操作，则清除按键事件，避免干扰其他实验
            if(mode == NORMAL_MODE)
            {
                switch(expState)
                {
                    case EXP1:
                        // 实验1功能
                        btnEvent = BTN_EVENT_NONE;
                        break;
                    case EXP2:
                        // 实验2功能
                        btnEvent = BTN_EVENT_NONE;
                        break;
                    case EXP3:
                        // 实验3功能
                        break;
                    case EXP4:
                        // 实验4功能
                        btnEvent = BTN_EVENT_NONE;
                        break;
                    case EXP5:
                        // 实验5功能
                        btnEvent = BTN_EVENT_NONE;
                        break;
                    case EXP6:
                        // 实验6功能
                        btnEvent = BTN_EVENT_NONE;
                        break;
                    case EXP7:
                        // 实验7功能
                        btnEvent = BTN_EVENT_NONE;
                        break;
                    case EXP8:
                        // 实验8功能
                        break;
                    case EXP9:
                        // 实验9功能
                        break;
                    case EXP10:
                        // 实验10功能
                        break;
                    case EXP11:
                        // 实验11功能
                        btnEvent = BTN_EVENT_NONE;
                        break;
                }
            }
        }
        osDelay(10);
    }
    /* USER CODE END StartSettingTask */
}

/* USER CODE BEGIN Header_StartUARTTask */
/**
 * @brief Function implementing the UARTTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUARTTask */
void
StartUARTTask(void *argument)
{
    /* USER CODE BEGIN StartUARTTask */
    char countBuffer[16];        // 计数值字符串缓冲区
    char lightBuffer[16];        // 光照平均值字符串缓冲区
    uint16_t lightFilteredValue; // 从消息队列LightQueue接收光照平均值
    char tempBuffer[16];         // 温度滤波值字符串缓冲区
    uint16_t tempFilteredValue;  // 从消息队列TempQueue接收温度平均值
    uint16_t currentCounter = 0; // 定时器当前计数值，用于判断是否需要发送更新

    UartTxMsg_t switchMsg; // 从消息队列SwitchQueue接收的消息结构体
    /* Infinite loop */
    for(;;)
    {
        if(expState == EXP5) // 实验五，定时器半秒计数，通过UART发送计数值
        {
            if(mode == NORMAL_MODE)
            {
                // 只有当计数值发生变化时才发送，避免重复发送相同的值
                if(TimerBinaryCounter / 5 != currentCounter)
                {
                    currentCounter = TimerBinaryCounter / 5;
                    snprintf(countBuffer, sizeof(countBuffer), "Count: %d", TimerBinaryCounter / 5);

                    if(HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
                    {
                        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)countBuffer, strlen(countBuffer));
                    }
                }
            }
            else
            {
                // 设置模式下清空计数值，准备下一次实验
                TimerBinaryCounter = 0;
                currentCounter = 0;
            }
        }

        // 实验六，UART接收光照平均值，计算电压并通过UART发送
        // 接收消息队列中的光照平均值，无需等待，如果没有新数据则继续循环
        if(osMessageQueueGet(LightQueueHandle, &lightFilteredValue, 0, 0) == osOK)
        {
            // 将12位ADC值转换为电压值并发送
            snprintf(
              lightBuffer, sizeof(lightBuffer), "Light: %3.2fV", lightFilteredValue * 3.3 / 4095);

            if(HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
            {
                HAL_UART_Transmit_DMA(&huart1, (uint8_t *)lightBuffer, strlen(lightBuffer));
                osDelay(500); // 发送后短暂延时，避免发送过快导致数据混乱
            }
        }

        // 实验七，UART接收温度滤波值，计算电压并通过UART发送
        // 接收消息队列中的温度滤波值，无需等待，如果没有新数据则继续循环
        if(osMessageQueueGet(TempQueueHandle, &tempFilteredValue, 0, 0) == osOK)
        {
            // 计算电压并发送
            snprintf(
              tempBuffer, sizeof(tempBuffer), "Temp: %3.2fV", tempFilteredValue * 3.3 / 4095);
            if(HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
            {
                HAL_UART_Transmit_DMA(&huart1, (uint8_t *)tempBuffer, strlen(tempBuffer));
                osDelay(500); // 发送后短暂延时，避免发送过快导致数据混乱
            }
        }

        // 实验八，UART接收计数值，通过UART发送
        if(osMessageQueueGet(SwitchQueueHandle, &switchMsg, 0, 0) == osOK)
        {
            if(DisplayType == DISPLAY_COUNT)
            {
                snprintf(countBuffer, sizeof(countBuffer), "Count: %d", switchMsg.countValue);
                if(HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
                {
                    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)countBuffer, strlen(countBuffer));
                }
            }
            else if(DisplayType == DISPLAY_LIGHT)
            {
                snprintf(lightBuffer,
                         sizeof(lightBuffer),
                         "Light: %3.2fV",
                         switchMsg.lightValue * 3.3 / 4095);
                if(HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
                {
                    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)lightBuffer, strlen(lightBuffer));
                }
            }
        }

        osDelay(50);
    }
    /* USER CODE END StartUARTTask */
}

/* USER CODE BEGIN Header_StartADTask */
/**
 * @brief Function implementing the ADTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartADTask */
void
StartADTask(void *argument)
{
    /* USER CODE BEGIN StartADTask */
    uint16_t lightValue[10];       // 存储光敏传感器10次的采样值
    uint16_t lightValueSorted[10]; // 存储排序后的光敏传感器采样值
    uint32_t lightSum = 0;         // 光敏传感器采样值的累加和，用于计算平均值
    uint16_t lightFilteredValue;   // 光敏传感器滤波后的值
    uint16_t tempValue[10];        // 存储温度传感器10次的采样值
    uint16_t tempValueSorted[10];  // 存储排序后的温度传感器采样值
    uint32_t tempSum = 0;          // 温度传感器采样值的累加和，用于计算平均值
    uint16_t tempFilteredValue;    // 温度传感器滤波后的值

    /* Infinite loop */
    for(;;)
    {
        if(expState == EXP6 || expState == EXP9 || expState == EXP10 || expState == EXP11)
        // 实验六，ADC采集光敏传感器的模拟值，转换结果采用二级数字滤波法（10次采样的结果排序、取中间的6个数的平均值），并将滤波后的转换结果通过消息队列发送给UARTTask
        {
            if(mode == NORMAL_MODE)
            {
                // 采集值已经通过DMA存储在adcValue数组中，adcValue[1]为光敏传感器的采样值，adcValue[0]为温度传感器的采样值

                // 将之前的采样值向后移动
                memmove(&lightValue[1], &lightValue[0], sizeof(uint16_t) * 9);

                // 将采样值放入lightValue数组的第一个位置
                lightValue[0] = adcValue[1];

                // 将采样值放入lightValueSorted数组，准备排序
                memcpy(lightValueSorted, lightValue, sizeof(lightValue));

                // 冒泡排序
                for(int i = 0; i < 9; i++)
                {
                    for(int j = 0; j < 9 - i; j++)
                    {
                        if(lightValueSorted[j] > lightValueSorted[j + 1])
                        {
                            uint16_t temp = lightValueSorted[j];
                            lightValueSorted[j] = lightValueSorted[j + 1];
                            lightValueSorted[j + 1] = temp;
                        }
                    }
                }

                // 取排序后中间的6个数的平均值，并通过消息队列发送给UARTTask
                lightSum = 0;
                for(int i = 2; i < 8; i++)
                {
                    lightSum += lightValueSorted[i];
                }

                lightFilteredValue = lightSum / 6; // 计算平均值
                if(expState == EXP6)
                {
                    // 发送平均值到LightQueue
                    osMessageQueuePut(LightQueueHandle, &lightFilteredValue, 0, 0);
                }
                else if(expState == EXP9 || expState == EXP10 || expState == EXP11)
                {
                    // 发送平均值到LightSwQueue
                    osMessageQueuePut(LightSwQueueHandle, &lightFilteredValue, 0, 0);
                }
            }
            else
            {
                // 实验六设置模式，清空采样值、累加和和消息队列，准备下一次实验
                memset(lightValue, 0, sizeof(lightValue));
                memset(lightValueSorted, 0, sizeof(lightValueSorted));
                osMessageQueueReset(LightQueueHandle);
            }
        }

        if(expState == EXP7)
        // 实验七，ADC采集温度传感器的模拟值，将滤波后的转换结果通过消息队列发送给UARTTask
        {
            if(mode == NORMAL_MODE)
            {
                // 处理温度传感器的采样值，方法同光敏传感器
                memmove(&tempValue[1], &tempValue[0], sizeof(uint16_t) * 9);
                tempValue[0] = adcValue[0];
                memcpy(tempValueSorted, tempValue, sizeof(tempValue));
                for(int i = 0; i < 9; i++)
                {
                    for(int j = 0; j < 9 - i; j++)
                    {
                        if(tempValueSorted[j] > tempValueSorted[j + 1])
                        {
                            uint16_t temp = tempValueSorted[j];
                            tempValueSorted[j] = tempValueSorted[j + 1];
                            tempValueSorted[j + 1] = temp;
                        }
                    }
                }
                tempSum = 0;
                for(int i = 2; i < 8; i++)
                {
                    tempSum += tempValueSorted[i];
                }
                tempFilteredValue = tempSum / 6;
                osMessageQueuePut(TempQueueHandle, &tempFilteredValue, 0, 0);
            }
            else
            {
                memset(tempValue, 0, sizeof(tempValue));
                memset(tempValueSorted, 0, sizeof(tempValueSorted));
                osMessageQueueReset(TempQueueHandle);
            }
        }

        osDelay(100);
    }
    /* USER CODE END StartADTask */
}

/* USER CODE BEGIN Header_StartSwitchTask */
/**
 * @brief Function implementing the SwitchTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSwitchTask */
void
StartSwitchTask(void *argument)
{
    /* USER CODE BEGIN StartSwitchTask */
    UartTxMsg_t uartTxMsg;       // UART发送消息结构体
    uint16_t currentCounter = 0; // 定时器当前计数值，用于判断是否需要发送更新
    uint16_t lightSwValue;       // 从消息队列LightSwQueue接收的光照平均值

    /* Infinite loop */
    for(;;)
    {
        if(expState == EXP8)
        // 实验八，用定时器实现秒计数，通过按键在LED和上位机上切换显示计数值
        {
            if(mode == NORMAL_MODE)
            {
                // 按键切换显示内容，LED显示计数值的二进制形式，上位机通过UART显示计数值的十进制形式
                if(btnEvent == BTN_EVENT_CLICK)
                {
                    DisplayType = (DisplayType == DISPLAY_COUNT) ? DISPLAY_LIGHT
                                                                 : DISPLAY_COUNT; // 切换显示类型
                    btnEvent = BTN_EVENT_NONE;                                    // 清除事件
                }

                if(DisplayType == DISPLAY_COUNT)
                {
                    if(TimerBinaryCounter / 10 != currentCounter)
                    {
                        currentCounter = TimerBinaryCounter / 10;

                        // 定时器每100ms产生一次中断，计数值除以10实现秒计数
                        uartTxMsg.countValue = TimerBinaryCounter / 10;
                        uartTxMsg.lightValue = 0; // 光照值不使用

                        // 发送消息到SwitchQueue
                        osMessageQueuePut(SwitchQueueHandle, &uartTxMsg, 0, 0);
                    }
                }
                else
                {
                    // LED显示计数值的二进制形式
                    LED_Binary_Display(TimerBinaryCounter / 10);
                }
            }
            else
            {
                // 设置模式下清空计数值，准备下一次实验
                TimerBinaryCounter = 0;
                currentCounter = 0;
            }
        }

        if(expState == EXP9 || expState == EXP10)
        // 实验九，用滴答定时器实现半秒计数，通过按键在LED上切换显示计数值和光照转换值
        // 实验十，用滴答定时器实现半秒计数，通过按键在上位机上切换显示计数值和光照转换值
        {
            if(mode == NORMAL_MODE)
            {
                // 从消息队列LightSwQueue接收光照平均值
                osMessageQueueGet(LightSwQueueHandle, &lightSwValue, 0, 0);

                if(btnEvent == BTN_EVENT_CLICK)
                {
                    DisplayType = (DisplayType == DISPLAY_COUNT) ? DISPLAY_LIGHT
                                                                 : DISPLAY_COUNT; // 切换显示类型
                    btnEvent = BTN_EVENT_NONE;                                    // 清除事件
                }
                if(expState == EXP9)
                {
                    if(DisplayType == DISPLAY_COUNT)
                    {
                        // LED显示半秒计数的二进制形式
                        LED_Binary_Display(tickCounter);
                    }

                    else
                    {
                        // LED显示光照转换值的二进制形式
                        LED_Binary_Display(lightSwValue);
                    }
                }
                else if(expState == EXP10)
                {
                    // 上位机通过UART显示半秒计数值
                    uartTxMsg.countValue = tickCounter;
                    // 上位机通过UART显示光照转换值
                    uartTxMsg.lightValue = lightSwValue;

                    osMessageQueuePut(SwitchQueueHandle, &uartTxMsg, 0, 0);
                }
            }
            else
            {
                // 清除消息队列和计数值，准备下一次实验
                osMessageQueueReset(LightSwQueueHandle);

                tickCounter = 0;
            }
        }

        osDelay(500);
    }
    /* USER CODE END StartSwitchTask */
}

/* USER CODE BEGIN Header_StartTickTask */
/**
 * @brief Function implementing the TickTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTickTask */
void
StartTickTask(void *argument)
{
    /* USER CODE BEGIN StartTickTask */
    UartTxMsg_t uartTxMsg; // UART发送消息结构体
    uint16_t lightSwValue; // 从消息队列LightSwQueue接收的光照平均值
    /* Infinite loop */
    for(;;)
    {
        // 滴答定时器实现半秒计数
        if(expState == EXP9 || expState == EXP10 || expState == EXP11)
        {
            if(mode == NORMAL_MODE)
            {
                tickCounter++; // 每次滴答定时器中断递增计数值
            }
            else
            {
                // 设置模式下清空计数值，准备下一次实验
                tickCounter = 0;
            }
        }

        if(expState == EXP11)
        // 实验十一，任务一：用滴答定时器实现半秒计数，结果在上位机显示；任务二：将光照转换值显示在LED上
        {
            if(mode == NORMAL_MODE)
            {
                // 将显示模式设置为计数
                DisplayType = DISPLAY_COUNT;

                uartTxMsg.countValue = tickCounter;
                uartTxMsg.lightValue = 0; // 光照值不使用

                // 此处借用Switch任务的消息队列发送计数值
                osMessageQueuePut(SwitchQueueHandle, &uartTxMsg, 0, 0);

                osMessageQueueGet(LightSwQueueHandle, &lightSwValue, 0, 0);
                LED_Binary_Display(lightSwValue);
            }
            else
            {
                // 设置模式下清空计数值，准备下一次实验
                tickCounter = 0;
                osMessageQueueReset(LightSwQueueHandle);
            }
        }
        osDelay(500);
    }
    /* USER CODE END StartTickTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
