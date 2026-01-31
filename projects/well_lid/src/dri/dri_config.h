/*
 * @Description: 
 * @Version: 
 * @Author: 
 * @Date: 2026-01-30 14:15:10
 * @LastEditors: Please set LastEditors
 * @LastEditorTime: 
 */
#ifndef __DRI_CONFIG_H__
#define __DRI_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32l40x.h"

#define _LPUART_COM_

#ifdef _LPUART_COM_
#define LPUART_GPIO       GPIOA
#define LPUART_GPIO_CLK   RCC_APB2_PERIPH_GPIOA
#define LPUART_RxPin      GPIO_PIN_0
#define LPUART_TxPin      GPIO_PIN_1
#define LPUART_Rx_GPIO_AF GPIO_AF6_LPUART
#define LPUART_Tx_GPIO_AF GPIO_AF6_LPUART
#endif

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;

#define CLKSTARTUP_TIMEOUT ((uint32_t)0x00020000)



#ifdef __cplusplus
}
#endif


//! 自定义宏定义
#define uwTickFrequency   1U // 1KHz

#endif /* __DRI_CONFIG_H__ */






