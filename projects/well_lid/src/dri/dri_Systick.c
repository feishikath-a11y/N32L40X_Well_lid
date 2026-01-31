#include "dri_Systick.h"
#include "com_config.h"
#include "dri_config.h"
#include "elog.h"



#define CAT "dri_Systick"



void dri_systick_1ms_IRQ_init(void)
{
    SystemError_t error = (SystemError_t)SysTick_Config(SystemCoreClock / (1000U / uwTickFrequency)); // 1ms中断
    if (error != ERR_SUCCESS)
    {
        elog_error(CAT, "SysTick_Config error: %d", error);
    } 

    NVIC_SetPriority(SysTick_IRQn, 0x0); // 设置SysTick中断优先级为最高
    NVIC_EnableIRQ(SysTick_IRQn);      // 使能SysTick中断

}




