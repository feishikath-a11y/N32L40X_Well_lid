/*
 * @Description: 
 * @Version: 
 * @Author: 
 * @Date: 2026-01-30 14:05:20
 * @LastEditors: Please set LastEditors
 * @LastEditorTime: 
 */
#ifndef __INT_LED_H__
#define __INT_LED_H__

#include "com_config.h"

// VDD控制
void int_LED_VDD_ON (void);// LED_VDD使能

void int_LED_VDD_OFF (void);// LED_VDD关闭

// LED 初始化
SystemError_t int_LED_Init(void);

void int_LED_ON (void);// LED点亮

void int_LED_OFF (void);// LED熄灭

//void int_LED_Toggle(void);// LED翻转

void int_LED_Blink(void);// LED闪烁


#endif /* __INT_LED_H__ */
