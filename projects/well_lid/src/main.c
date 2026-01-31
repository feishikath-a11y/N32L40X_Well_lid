/*
 * @Description: 
 * @Version: 
 * @Author: 
 * @Date: 2026-01-30 11:30:07
 * @LastEditors: Please set LastEditors
 * @LastEditorTime: 
 */
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "elog.h"
#include "dri_clockconfig.h"
#include "n32l40x.h"
#include "core_cm4.h"
#include "dri_systick.h"
#include "dri_usart.h"
#include "n32l40x_it.h"



/**
 * @brief  初始化LED
 * @note   配置LED对应的GPIO引脚为输出模式
 */
void Har_LED_Init(void)
{
    // 定义 GPIO 初始化结构体，用于存储 GPIO 引脚的配置信息
    GPIO_InitType GPIO_InitStructure;

    // 使能 GPIOA 端口的时钟，因为 LED 连接在该端口
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);

    // 初始化 GPIO 初始化结构体，将其成员变量设置为默认值
    GPIO_InitStruct(&GPIO_InitStructure);
    // 设置要初始化的 GPIO 引脚，LED_PIN 为 LED 对应的引脚宏定义
    GPIO_InitStructure.Pin = LED_PIN;
    // 设置 GPIO 引脚的驱动电流为 2mA
    GPIO_InitStructure.GPIO_Current = GPIO_DC_2mA;
    // 设置 GPIO 引脚的压摆率为低，即信号上升/下降时间较长
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_Low;
    // 设置 GPIO 引脚为下拉模式，即默认电平为低电平
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Down;
    // 设置 GPIO 引脚为推挽输出模式，可输出高、低电平
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // 根据上述配置信息初始化指定的 GPIO 端口（LED_PORT 为 LED 对应的端口宏定义）
    GPIO_InitPeripheral(LED_PORT, &GPIO_InitStructure);
}

void Har_LED_On(void)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);
}

void Har_LED_Off(void)
{
    GPIO_SetBits(LED_PORT, LED_PIN);
}

int main(void)
{
    //elog_init();
    
    //初始化SysTick
    //dri_systick_1ms_IRQ_init();

    //初始化时钟配置
    //dri_ClockConfig_Init();

    //初始化串口
    dri_usart_init();


   
    

    while (1)
    {
        printf("测试\n");
        delay_ms(3000);
        
    }
    


    //elog_deinit();

    //!LED_Init

    // Har_LED_Init();
    // while (1)
    // {
    //     Har_LED_On();
    //     Delay(0x28FFFF);
    //     Har_LED_Off();
    //     Delay(0x28FFFF);
    // }
}
