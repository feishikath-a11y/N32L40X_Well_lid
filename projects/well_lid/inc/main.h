#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32l40x.h"

/**
 * @def LED_PORT
 * @brief 定义 LED 所连接的 GPIO 端口，此处为 GPIOA。
 */
#define LED_PORT GPIOA

/**
 * @def LED_PIN
 * @brief 定义 LED 所连接的 GPIO 引脚，此处为 GPIO_PIN_10。
 */
#define LED_PIN GPIO_PIN_9


/**
 * @brief  初始化LED
 * @note   配置LED对应的GPIO引脚为输出模式
 */
void Har_LED_Init(void);

/**
 * @brief  打开LED
 */
void Har_LED_On(void);

/**
 * @brief  关闭LED
 */
void Har_LED_Off(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
