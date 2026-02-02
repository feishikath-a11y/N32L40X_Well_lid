/*
 * @Description: 
 * @Version: 
 * @Author: 
 * @Date: 2026-01-30 14:05:41
 * @LastEditors: Please set LastEditors
 * @LastEditorTime: 
 */
#include "int_LED.h"
#include "n32l40x.h"
#include "com_config.h"
#include "elog.h"
#include "int_config.h"


//!函数声明
// LED_VDD初始化
SystemError_t int_LED_VDD_config(void);



// LED初始化
SystemError_t int_LED_config(void);







SystemError_t int_LED_Init(void)
{
    
    int_LED_VDD_config();// 初始化VDD_EN引脚
    int_LED_VDD_ON();// 使能VDD_EN

    int_LED_config();// 初始化LED引脚

    return ERR_SUCCESS;

}


SystemError_t int_LED_VDD_config(void)
{
    RCC_EnableAHBPeriphClk(VDD_EN_RCC_PERIPH, ENABLE);
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStruct(&GPIO_InitStructure);

    GPIO_InitStructure.Pin        = VDD_EN_PIN;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_2mA;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_Low;
    GPIO_InitStructure.GPIO_Pull = GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(VDD_EN_PORT, &GPIO_InitStructure);
    return ERR_SUCCESS;
}

void int_LED_VDD_ON (void)
{
    GPIO_SetBits(VDD_EN_PORT, VDD_EN_PIN);
}

void int_LED_VDD_OFF (void)
{
    GPIO_ResetBits(VDD_EN_PORT, VDD_EN_PIN);
}

SystemError_t int_LED_config(void)
{
       // 定义 GPIO 初始化结构体，用于存储 GPIO 引脚的配置信息
    GPIO_InitType GPIO_InitStructure;

   
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE); // 使能 GPIOA 端口的时钟，因为 LED 连接在该端口

   
    GPIO_InitStruct(&GPIO_InitStructure); // 初始化 GPIO 初始化结构体，将其成员变量设置为默认值
   
    GPIO_InitStructure.Pin = LED_PIN; // 设置要初始化的 GPIO 引脚，LED_PIN 为 LED 对应的引脚宏定义
    GPIO_InitStructure.GPIO_Current = GPIO_DC_2mA;    // 设置 GPIO 引脚的驱动电流为 2mA
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_Low; // 设置 GPIO 引脚的压摆率为低，即信号上升/下降时间较长
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Down;// 设置 GPIO 引脚为下拉模式，即默认电平为低电平
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// 设置 GPIO 引脚为推挽输出模式，可输出高、低电平
    GPIO_InitPeripheral(LED_PORT, &GPIO_InitStructure);// 根据上述配置信息初始化指定的 GPIO 端口（LED_PORT 为 LED 对应的端口宏定义）
    
    return ERR_SUCCESS;
}

void int_LED_ON(void)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);
}

void int_LED_OFF(void)
{
    GPIO_SetBits(LED_PORT, LED_PIN);
}

void int_LED_Blink(void)
{

}
