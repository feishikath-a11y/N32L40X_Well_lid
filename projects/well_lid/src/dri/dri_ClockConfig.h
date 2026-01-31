#ifndef __DRI_CLOCKCONFIG_H__
#define __DRI_CLOCKCONFIG_H__


//clock config 初始化
void dri_ClockConfig_Init(void);

/** 
 * @brief 时间戳转换函数
 * @param msg 调试信息字符串
 * @note 此函数被时间中断调用，需要重新初始化标志系统
 */
void dri_ClockConfig_DumpClock(const char* msg);

// NVIC 
// void NVIC_Configuration(void);

#endif /* __DRI_CLOCKCONFIG_H__ */
