#ifndef __DRI_SYSTICK_H__
#define __DRI_SYSTICK_H__

#include "n32l40x.h"
#include "n32l40x_it.h"

typedef uint32_t (*GetTick_CallbackFunc)(void);


// 1ms中断初始化
void dri_systick_1ms_IRQ_init(void);

// 注册获取滴答计时回调函数
void systick_register_get_tick_callback(GetTick_CallbackFunc callback);

// 获取当前tick - dri_systick内部使用回调获取值
uint32_t dri_systick_get_tick(void);

// 毫秒级延时函数
void dri_systick_delay_ms(uint32_t ms);



#endif /* __DRI_SYSTICK_H__ */

