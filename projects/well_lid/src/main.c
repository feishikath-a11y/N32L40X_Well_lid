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

#define CAT "MAIN"

int main(void)
{
    //elog_init();
    // 注册计时器
    systick_register_get_tick_callback(get_tick);

    // 初始化SysTick
    dri_systick_1ms_IRQ_init();

    // 初始化时钟配置
    //dri_ClockConfig_Init();

    // 初始化串口
    dri_usart_init();

    printf("register\n");


    // elog_info(CAT, "hello world!");

    while (1)
    {
        printf("test\n");
        dri_systick_delay_ms(3000);
    }

    //! LED_Init

    // Har_LED_Init();
    // while (1)
    // {
    //     Har_LED_On();
    //     Delay(0x28FFFF);
    //     Har_LED_Off();
    //     Delay(0x28FFFF);
    // }
}
