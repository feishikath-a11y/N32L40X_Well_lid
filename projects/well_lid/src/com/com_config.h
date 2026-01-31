#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

/**
 * @brief 系统错误代码枚举
 * @note 采用分层编码结构：模块(4bit) + 错误类型(4bit) + 具体错误(8bit)
 */
typedef enum {
    /*----------- 系统通用错误 (0x00xx) -----------*/
    ERR_SUCCESS                 = 0x0000,   // 操作成功
    ERR_FAILURE                 = 0x0001,   // 操作失败
    
    /* 内存相关错误 */
    ERR_MEM_ALLOC_FAILED        = 0x0101,   // 内存分配失败
    ERR_MEM_FREE_FAILED         = 0x0102,   // 内存释放失败
    ERR_MEM_OVERFLOW            = 0x0103,   // 内存溢出
    ERR_MEM_CORRUPT             = 0x0104,   // 内存损坏
    ERR_STACK_OVERFLOW          = 0x0105,   // 栈溢出
    ERR_HEAP_OVERFLOW           = 0x0106,   // 堆溢出
    
    /* 参数相关错误 */
    ERR_INVALID_PARAM           = 0x0201,   // 无效参数
    ERR_PARAM_OUT_OF_RANGE      = 0x0202,   // 参数超出范围
    ERR_PARAM_NULL_PTR          = 0x0203,   // 参数空指针
    ERR_PARAM_INVALID_SIZE      = 0x0204,   // 参数大小无效
    
    /* 状态相关错误 */
    ERR_INVALID_STATE           = 0x0301,   // 无效状态
    ERR_NOT_INITIALIZED         = 0x0302,   // 未初始化
    ERR_ALREADY_INITIALIZED     = 0x0303,   // 已初始化
    ERR_NOT_READY               = 0x0304,   // 未就绪
    ERR_BUSY                    = 0x0305,   // 忙碌状态
    ERR_TIMEOUT                 = 0x0306,   // 超时
    ERR_DEADLOCK                = 0x0307,   // 死锁
    
    /*----------- 硬件错误 (0x10xx) -----------*/
    /* CPU/内核错误 */
    ERR_HARD_FAULT              = 0x1001,   // 硬件错误
    ERR_MEM_FAULT               = 0x1002,   // 内存访问错误
    ERR_BUS_FAULT               = 0x1003,   // 总线错误
    ERR_USAGE_FAULT             = 0x1004,   // 用法错误
    ERR_NMI_HANDLER             = 0x1005,   // NMI不可屏蔽中断
    
    /* 外设错误 */
    ERR_PERIPH_FAILURE          = 0x1101,   // 外设故障
    ERR_PERIPH_TIMEOUT          = 0x1102,   // 外设超时
    ERR_PERIPH_BUSY             = 0x1103,   // 外设忙
    ERR_PERIPH_NOT_RESPONDING   = 0x1104,   // 外设无响应
    
    /* 通信接口错误 */
    ERR_UART_FAILURE            = 0x1201,   // UART故障
    ERR_UART_OVERRUN            = 0x1202,   // UART溢出
    ERR_UART_FRAMING_ERROR      = 0x1203,   // UART帧错误
    ERR_UART_PARITY_ERROR       = 0x1204,   // UART奇偶校验错误
    
    ERR_SPI_FAILURE             = 0x1211,   // SPI故障
    ERR_SPI_CRC_ERROR           = 0x1212,   // SPI CRC错误
    ERR_SPI_OVERRUN             = 0x1213,   // SPI溢出
    
    ERR_I2C_FAILURE             = 0x1221,   // I2C故障
    ERR_I2C_ARBITRATION_LOST    = 0x1222,   // I2C仲裁丢失
    ERR_I2C_BUS_ERROR           = 0x1223,   // I2C总线错误
    ERR_I2C_ACK_FAILURE         = 0x1224,   // I2C应答失败
    
    /* 存储错误 */
    ERR_FLASH_WRITE_FAILED      = 0x1301,   // Flash写入失败
    ERR_FLASH_ERASE_FAILED      = 0x1302,   // Flash擦除失败
    ERR_FLASH_READ_FAILED       = 0x1303,   // Flash读取失败
    ERR_FLASH_PROTECTED         = 0x1304,   // Flash写保护
    ERR_FLASH_ECC_ERROR         = 0x1305,   // Flash ECC错误
    
    ERR_EEPROM_FAILED           = 0x1311,   // EEPROM操作失败
    
    /* 电源/时钟错误 */
    ERR_POWER_FAILURE           = 0x1401,   // 电源故障
    ERR_BROWNOUT_RESET          = 0x1402,   // 欠压复位
    ERR_CLOCK_FAILURE           = 0x1403,   // 时钟故障
    ERR_PLL_UNLOCK              = 0x1404,   // PLL失锁
    
    /*----------- 通信协议错误 (0x20xx) -----------*/
    ERR_PROTOCOL_INVALID        = 0x2001,   // 协议无效
    ERR_PROTOCOL_CRC_ERROR      = 0x2002,   // 协议CRC错误
    ERR_PROTOCOL_TIMEOUT        = 0x2003,   // 协议超时
    ERR_PROTOCOL_SEQ_ERROR      = 0x2004,   // 协议序列错误
    ERR_PROTOCOL_BUFFER_FULL    = 0x2005,   // 协议缓冲区满
    
    /*----------- 应用层错误 (0x30xx) -----------*/
    ERR_TASK_CREATE_FAILED      = 0x3001,   // 任务创建失败
    ERR_TASK_DELETE_FAILED      = 0x3002,   // 任务删除失败
    ERR_QUEUE_FULL              = 0x3003,   // 队列满
    ERR_QUEUE_EMPTY             = 0x3004,   // 队列空
    ERR_SEMAPHORE_FAILED        = 0x3005,   // 信号量操作失败
    ERR_MUTEX_FAILED            = 0x3006,   // 互斥锁操作失败
    
    /*----------- 安全相关错误 (0x40xx) -----------*/
    ERR_SECURITY_VIOLATION      = 0x4001,   // 安全违规
    ERR_AUTHENTICATION_FAILED   = 0x4002,   // 认证失败
    ERR_ENCRYPTION_FAILED       = 0x4003,   // 加密失败
    ERR_DECRYPTION_FAILED       = 0x4004,   // 解密失败
    ERR_SIGNATURE_INVALID       = 0x4005,   // 签名无效
    
    /*----------- 自定义/保留错误 (0xF0xx) -----------*/
    ERR_CUSTOM_BASE             = 0xF000,   // 自定义错误起始
    ERR_UNKNOWN                 = 0xFFFF,   // 未知错误
    
} SystemError_t;


#endif /* __COM_CONFIG_H__ */

