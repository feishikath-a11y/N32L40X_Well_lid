/*
 * @Description:
 * @Version:
 * @Author:
 * @Date: 2026-01-30 14:10:05
 * @LastEditors: Please set LastEditors
 * @LastEditorTime:
 */
#include "dri_usart.h"
#include "dri_config.h"
#include <stdio.h>
#include "n32l40x.h"
#include "elog.h"

//!宏定义
#define CAT "DRI_USART"


//!变量定义

/** @addtogroup N32L40x_StdPeriph_Examples
 * @{
 */

/** @addtogroup LPUART_Printf
 * @{
 */

LPUART_InitType LPUART_InitStructure;


//! 函数声明
ErrorStatus RCC_Configuration(uint32_t LPUART_CLK_SRC);
void GPIO_Configuration(void);



//!函数定义
SystemError_t dri_usart_init(void)
{


    /* System Clocks Configuration */
    if(RCC_Configuration(RCC_LPUARTCLK_SRC_APB1)  == SUCCESS) //包含enable LPUART时钟
    {
        /* System Clocks Configuration Success */
        //elog_info(CAT,"RCC_Configuration SUCCESS");
    }
    else
    {
        /* System Clocks Configuration Error, User-added processing required */
        //elog_error(CAT,"RCC_Configuration ERROR");
        return ERR_UART_FAILURE;
    }

    /* Enable GPIO clock */
    RCC_EnableAPB2PeriphClk(LPUART_GPIO_CLK, ENABLE); //使能GPIOA时钟

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE); //使能AFIO时钟


    /* Configure the GPIO ports */
    GPIO_Configuration();

    LPUART_DeInit();
    /* LPUART configuration ------------------------------------------------------*/
    LPUART_StructInit(&LPUART_InitStructure);
    LPUART_InitStructure.BaudRate            = 115200;
    // LPUART_InitStructure.Parity              = LPUART_PE_NO;
    // LPUART_InitStructure.RtsThreshold        = LPUART_RTSTH_FIFOFU;
    // LPUART_InitStructure.HardwareFlowControl = LPUART_HFCTRL_NONE;
    // LPUART_InitStructure.Mode                = LPUART_MODE_RX | LPUART_MODE_TX;
    /* Configure LPUART */
    LPUART_Init(&LPUART_InitStructure);

    /* Output a message on Hyperterminal using printf function */
    printf("\n\rLPUART Printf Example: retarget the C library printf function to the LPUART\n\r");



    return ERR_SUCCESS;
}


/**
 * @brief  Configures the different system clocks.
 * @param  LPUART_CLK_SRC: specifies the LPUART clock source.
 */
ErrorStatus RCC_Configuration(uint32_t LPUART_CLK_SRC)
{
    ErrorStatus Status_value = SUCCESS;
    uint32_t StartUpCounter = 0;
    
    switch(LPUART_CLK_SRC)
    {
        case RCC_LPUARTCLK_SRC_LSE:
        {            
            RCC_EnableAPB1PeriphClk (RCC_APB1_PERIPH_PWR, ENABLE); 
            PWR->CTRL1 |= PWR_CTRL1_DRBP;
            /* Configures the External Low Speed oscillator (LSE) */
            RCC_ConfigLse(RCC_LSE_ENABLE,0x1FF);
            while ((RCC_GetFlagStatus(RCC_LDCTRL_FLAG_LSERD) == RESET) && (StartUpCounter <= CLKSTARTUP_TIMEOUT))
            {
                StartUpCounter++;
            }
            
            if(RCC_GetFlagStatus(RCC_LDCTRL_FLAG_LSERD) == RESET)
            {
                Status_value = ERROR;
            }
            else
            {
                /* Specifies the LPUART clock source, LSE selected as LPUART clock */
                RCC_ConfigLPUARTClk(RCC_LPUARTCLK_SRC_LSE);
            }
        }
        break;
        case RCC_LPUARTCLK_SRC_HSI:
        {
            /* Configures the High Speed Internal RC clock (HSI) */
            RCC_ConfigHsi(RCC_HSI_ENABLE);
            while ((RCC_GetFlagStatus(RCC_CTRL_FLAG_HSIRDF) == RESET) && (StartUpCounter <= CLKSTARTUP_TIMEOUT))
            {
                StartUpCounter++;
            }
            
            if(RCC_GetFlagStatus(RCC_CTRL_FLAG_HSIRDF) == RESET)
            {
                Status_value = ERROR;
            }
            else
            {
                /* Specifies the LPUART clock source, HSI selected as LPUART clock */
                RCC_ConfigLPUARTClk(RCC_LPUARTCLK_SRC_HSI);
            }
            
        }
        break;
        case RCC_LPUARTCLK_SRC_SYSCLK:
        {
            /* Specifies the LPUART clock source, SYSCLK selected as LPUART clock */
            RCC_ConfigLPUARTClk(RCC_LPUARTCLK_SRC_SYSCLK);
        }
        break;
        default:
        {
            /* Specifies the LPUART clock source, APB1 selected as LPUART clock */
            RCC_ConfigLPUARTClk(RCC_LPUARTCLK_SRC_APB1);
        }
        break;
    }    
    
    /* Enable LPUART Clock */    
    RCC_EnableRETPeriphClk(RCC_RET_PERIPH_LPUART, ENABLE);
    
    return Status_value;
}  


void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;   

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);

    /* Configure LPUART Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = LPUART_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
    GPIO_InitStructure.GPIO_Alternate = LPUART_Tx_GPIO_AF;
    GPIO_InitPeripheral(LPUART_GPIO, &GPIO_InitStructure);

    /* Configure LPAURT Rx as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = LPUART_RxPin;
    GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF_PP;    
    GPIO_InitStructure.GPIO_Alternate = LPUART_Rx_GPIO_AF;
    GPIO_InitPeripheral(LPUART_GPIO, &GPIO_InitStructure);
}


/* retarget the C library printf function to the LPUART */
int fputc(int ch, FILE* f)
{
    LPUART_SendData((uint8_t)ch);
    while (LPUART_GetFlagStatus(LPUART_FLAG_TXC) == RESET)
        ;
    LPUART_ClrFlag(LPUART_FLAG_TXC);      

    return (ch);
}
