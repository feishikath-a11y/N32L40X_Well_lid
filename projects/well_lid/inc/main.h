#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32l40x.h"


/**
 * @brief  ��ʼ��LED
 * @note   ����LED��Ӧ��GPIO����Ϊ���ģʽ
 */
void Har_LED_Init(void);

/**
 * @brief  ��LED
 */
void Har_LED_On(void);

/**
 * @brief  �ر�LED
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
