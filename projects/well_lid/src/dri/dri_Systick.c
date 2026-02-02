/*
 * @Description: 
 * @Version: 
 * @Author: 
 * @Date: 2026-01-30 17:09:28
 * @LastEditors: Please set LastEditors
 * @LastEditorTime: 
 */

#include "dri_Systick.h"
#include <stdio.h>
#include "com_config.h"
#include "dri_config.h"
#include "elog.h"



#define CAT "dri_Systick"

static GetTick_CallbackFunc tick_callback = NULL;


/**
 * @brief 初始化SysTick为1ms中断
 * @note 配置SysTick定时器产生1ms中断，用于系统节拍计时
 */
void dri_systick_1ms_IRQ_init(void)
{
    // 配置SysTick定时器，计算重装载值实现1ms中断
    SysTick_Config(SystemCoreClock / (1000U / uwTickFrequency)); // 1ms中断
    
    // 检查配置是否成功
    // if (error != ERR_SUCCESS)
    // {
    //     // 输出错误日志
    //     elog_error(CAT, "SysTick_Config error: %d", error);
    // } 

    // 设置SysTick中断优先级为最高
    NVIC_SetPriority(SysTick_IRQn, 0); // 设置SysTick中断优先级为最高
    
    // // 使能SysTick中断
    // NVIC_EnableIRQ(SysTick_IRQn);      // 使能SysTick中断

    
}

void systick_register_get_tick_callback(GetTick_CallbackFunc callback)
{
    tick_callback = callback;
}

uint32_t dri_systick_get_tick(void)
{
    return tick_callback();
}

void dri_systick_delay_ms(uint32_t ms)
{
    printf("start delay\n");
    uint32_t tickstart = dri_systick_get_tick();
    printf("get tick %lu\n",tickstart);

    uint32_t wait = ms;

    if (wait < UINT32_MAX)
    {
        wait +=  (uint32_t)(uwTickFrequency);
    }

   
    while (dri_systick_get_tick() - tickstart < wait)
    {
       
    }
    
    
}

