#include "dri_ClockConfig.h"
#include "elog.h"
#include "n32l40x.h"
#include "n32l40x_flash.h"


#define CAT "dri_ClockConfig"

GPIO_InitType GPIO_InitStructure;
RCC_ClocksType RCC_ClockFreq;
ErrorStatus HSEStartUpStatus;
ErrorStatus HSIStartUpStatus;

enum
{
    SYSCLK_PLLSRC_HSI,
    SYSCLK_PLLSRC_HSIDIV2,
    SYSCLK_PLLSRC_HSI_PLLDIV2,
    SYSCLK_PLLSRC_HSIDIV2_PLLDIV2,
    SYSCLK_PLLSRC_HSE,
    SYSCLK_PLLSRC_HSEDIV2,
    SYSCLK_PLLSRC_HSE_PLLDIV2,
    SYSCLK_PLLSRC_HSEDIV2_PLLDIV2,
};

ErrorStatus SetSysClockToMSI(void);
ErrorStatus SetSysClockToHSI(void);
ErrorStatus SetSysClockToHSE(void);
ErrorStatus SetSysClockToPLL(uint32_t freq, uint8_t src);

// void NVIC_Configuration(void);

/**
 * @brief 时钟配置初始化函数
 * @note 此函数按顺序配置系统时钟源，从低频率逐步切换到高频率
 *       包括：复位状态 → MSI(4MHz) → HSE(8MHz) → PLL(64MHz)
 * @retval 无
 */
void dri_ClockConfig_Init(void)
{
    // 初始状态：显示复位后的时钟配置
    // ------------------------------------------------------------
    dri_ClockConfig_DumpClock("status after reset");

    // 第一步：配置系统时钟为内部MSI（4MHz）
    // ------------------------------------------------------------
    if (SetSysClockToMSI() == ERROR)
    {
        elog_error(CAT, "MSI clock configuration failed!");
        // 可以考虑添加错误恢复机制，如系统复位
    }
    else
    {
        elog_info(CAT, "MSI clock configuration succeeded");
    }
    dri_ClockConfig_DumpClock("MSI, 4MHz");

    // 第二步：切换到外部高速时钟HSE（8MHz）
    // ------------------------------------------------------------
    if (SetSysClockToHSE() == ERROR)
    {
        elog_error(CAT, "HSE clock configuration failed!");
        // 注意：HSE失败时通常保持MSI时钟继续运行
    }
    else
    {
        elog_info(CAT, "HSE clock configuration succeeded");
    }
    dri_ClockConfig_DumpClock("HSE, 8MHz");

    // 第三步：通过PLL倍频到64MHz
    // ------------------------------------------------------------
    if (SetSysClockToPLL(64000000, SYSCLK_PLLSRC_HSE) == ERROR)
    {
        elog_error(CAT, "PLL clock configuration failed!");
        // PLL配置失败时回退到HSE时钟
    }
    else
    {
        elog_info(CAT, "PLL clock configuration succeeded, system frequency: 64MHz");
    }
    dri_ClockConfig_DumpClock("HSE->PLL, 64MHz");

    // 第四步：启用时钟安全系统(CSS)
    // ------------------------------------------------------------
    // 当时钟安全系统启用后，如果HSE时钟失效，系统会自动切换到备用时钟源
    // 并触发NMI异常，防止系统因时钟失效而崩溃
    RCC_EnableClockSecuritySystem(ENABLE);
    elog_info(CAT, "Clock Security System enabled");

    // 第五步：配置中断控制器（注释状态，根据需要启用）
    // ------------------------------------------------------------
    // NVIC_Configuration();  // 中断优先级分组配置
    
    // 第六步：配置MCO引脚输出时钟信号（注释状态，用于调试）
    // ------------------------------------------------------------
    // RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    // 配置PA8为MCO输出HSE时钟，可用于示波器观察
}

void dri_ClockConfig_DumpClock(const char *msg)
{
    elog_info(CAT, "--------------------------------\n");
    elog_info(CAT, "%s:\n", msg);
    RCC_GetClocksFreqValue(&RCC_ClockFreq);
    elog_info(CAT, "SYSCLK: %u\n", (unsigned int)RCC_ClockFreq.SysclkFreq);
    elog_info(CAT, "HCLK: %u\n", (unsigned int)RCC_ClockFreq.HclkFreq);
    elog_info(CAT, "PCLK1: %u\n", (unsigned int)RCC_ClockFreq.Pclk1Freq);
    elog_info(CAT, "PCLK2: %u\n", (unsigned int)RCC_ClockFreq.Pclk2Freq);
}



void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable and configure RCC global IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel                   = RCC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



/**
 * @brief  Selects MSI as System clock source and configure HCLK, PCLK2
 *         and PCLK1 prescalers.
 */
ErrorStatus SetSysClockToMSI(void)
{
    uint32_t timeout_value = 0xFFFFFF; 
    
    if(RESET == RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_MSIRD))
    {
        /* Enable MSI and Config Clock */
        RCC_ConfigMsi(RCC_MSI_ENABLE, RCC_MSI_RANGE_4M);
        /* Waits for MSI start-up */
        if(SUCCESS != RCC_WaitMsiStable())
        {
            return ERROR;
        }
    }

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufSet(FLASH_PrefetchBuf_EN);

    /* Select MSI as system clock source */
    RCC_ConfigSysclk(RCC_SYSCLK_SRC_MSI);

    /* Wait till MSI is used as system clock source */
    while (RCC_GetSysclkSrc() != 0x00)
    {
        if ((timeout_value--) == 0)
        {
            return ERROR;
        }
    }

    /* Flash 0 wait state */
    FLASH_SetLatency(FLASH_LATENCY_0);

    /* HCLK = SYSCLK */
    RCC_ConfigHclk(RCC_SYSCLK_DIV1);

    /* PCLK2 = HCLK */
    RCC_ConfigPclk2(RCC_HCLK_DIV1);

    /* PCLK1 = HCLK */
    RCC_ConfigPclk1(RCC_HCLK_DIV1);

    return SUCCESS;
}

/**
 * @brief  Selects HSI as System clock source and configure HCLK, PCLK2
 *         and PCLK1 prescalers.
 */
ErrorStatus SetSysClockToHSI(void)
{
    uint32_t msi_ready_flag = RESET;
    uint32_t timeout_value = 0xFFFFFF; 

    RCC_EnableHsi(ENABLE);

    /* Wait till HSI is ready */
    HSIStartUpStatus = RCC_WaitHsiStable();

    if (HSIStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufSet(FLASH_PrefetchBuf_EN);

        if(((*(__IO uint8_t*)((UCID_BASE + 0x2))) == 0x01)
        || ((*(__IO uint8_t*)((UCID_BASE + 0x2))) == 0x11)
        || ((*(__IO uint8_t*)((UCID_BASE + 0x2))) == 0xFF))
        {
            /* Cheak if MSI is Ready */
            if(RESET == RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_MSIRD))
            {
                /* Enable MSI and Config Clock */
                RCC_ConfigMsi(RCC_MSI_ENABLE, RCC_MSI_RANGE_4M);
                /* Waits for MSI start-up */
                if(SUCCESS != RCC_WaitMsiStable())
                {
                    return ERROR;
                }

                msi_ready_flag = SET;
            }

            /* Select MSI as system clock source */
            RCC_ConfigSysclk(RCC_SYSCLK_SRC_MSI);

            /* Disable PLL */
            RCC_EnablePll(DISABLE);

            RCC_ConfigPll(RCC_PLL_HSI_PRE_DIV2, RCC_PLL_MUL_2, RCC_PLLDIVCLK_DISABLE);

            /* Enable PLL */
            RCC_EnablePll(ENABLE);

            /* Wait till PLL is ready */
            while (RCC_GetFlagStatus(RCC_CTRL_FLAG_PLLRDF) == RESET)
            {
                if ((timeout_value--) == 0)
                {
                    return ERROR;
                }
            }

            /* Select PLL as system clock source */
            RCC_ConfigSysclk(RCC_SYSCLK_SRC_PLLCLK);

            /* Wait till PLL is used as system clock source */
            timeout_value = 0xFFFFFF; 
            while (RCC_GetSysclkSrc() != 0x0C)
            {
                if ((timeout_value--) == 0)
                {
                    return ERROR;
                }
            }

            if(msi_ready_flag == SET)
            {
                /* MSI oscillator OFF */
                RCC_ConfigMsi(RCC_MSI_DISABLE, RCC_MSI_RANGE_4M);
            }
        }
        else
        {
            /* Select HSI as system clock source */
            RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSI);

            /* Wait till HSI is used as system clock source */
            timeout_value = 0xFFFFFF; 
            while (RCC_GetSysclkSrc() != 0x04)
            {
                if ((timeout_value--) == 0)
                {
                    return ERROR;
                }
            }
        }

        /* Flash 0 wait state */
        FLASH_SetLatency(FLASH_LATENCY_0);

        /* HCLK = SYSCLK */
        RCC_ConfigHclk(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_ConfigPclk2(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_ConfigPclk1(RCC_HCLK_DIV1);
    }
    else
    {
        /* If HSI fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief  Selects HSE as System clock source and configure HCLK, PCLK2
 *         and PCLK1 prescalers.
 */
ErrorStatus SetSysClockToHSE(void)
{
    uint32_t timeout_value = 0xFFFFFF; 
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/

    uint32_t msi_ready_flag = RESET;

    /* Enable HSE */
    RCC_ConfigHse(RCC_HSE_ENABLE);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitHseStable();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufSet(FLASH_PrefetchBuf_EN);
        
        if(((*(__IO uint8_t*)((UCID_BASE + 0x2))) == 0x01)
        || ((*(__IO uint8_t*)((UCID_BASE + 0x2))) == 0x11)
        || ((*(__IO uint8_t*)((UCID_BASE + 0x2))) == 0xFF))
        {
            /* Cheak if MSI is Ready */
            if(RESET == RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_MSIRD))
            {
                /* Enable MSI and Config Clock */
                RCC_ConfigMsi(RCC_MSI_ENABLE, RCC_MSI_RANGE_4M);
                /* Waits for MSI start-up */
                if(SUCCESS != RCC_WaitMsiStable())
                {
                    return ERROR;
                }

                msi_ready_flag = SET;
            }

            /* Select MSI as system clock source */
            RCC_ConfigSysclk(RCC_SYSCLK_SRC_MSI);

            /* Disable PLL */
            RCC_EnablePll(DISABLE);

            RCC_ConfigPll(RCC_PLL_SRC_HSE_DIV2, RCC_PLL_MUL_2, RCC_PLLDIVCLK_DISABLE);

            /* Enable PLL */
            RCC_EnablePll(ENABLE);

            /* Wait till PLL is ready */
            while (RCC_GetFlagStatus(RCC_CTRL_FLAG_PLLRDF) == RESET)
            {
                if ((timeout_value--) == 0)
                {
                    return ERROR;
                }
            }

            /* Select PLL as system clock source */
            RCC_ConfigSysclk(RCC_SYSCLK_SRC_PLLCLK);

            /* Wait till PLL is used as system clock source */
            timeout_value = 0xFFFFFF; 
            while (RCC_GetSysclkSrc() != 0x0C)
            {
                if ((timeout_value--) == 0)
                {
                    return ERROR;
                }
            }

            if(msi_ready_flag == SET)
            {
                /* MSI oscillator OFF */
                RCC_ConfigMsi(RCC_MSI_DISABLE, RCC_MSI_RANGE_4M);
            }
        }
        else
        {
            /* Select HSE as system clock source */
            RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSE);

            /* Wait till HSE is used as system clock source */
            timeout_value = 0xFFFFFF; 
            while (RCC_GetSysclkSrc() != 0x08)
            {
                if ((timeout_value--) == 0)
                {
                    return ERROR;
                }
            }
        }

        if (HSE_Value <= 32000000)
        {
            /* Flash 0 wait state */
            FLASH_SetLatency(FLASH_LATENCY_0);
        }
        else
        {
            /* Flash 1 wait state */
            FLASH_SetLatency(FLASH_LATENCY_1);
        }

        /* HCLK = SYSCLK */
        RCC_ConfigHclk(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_ConfigPclk2(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_ConfigPclk1(RCC_HCLK_DIV1);
    }
    else
    {
        /* If HSE fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */
        return ERROR;
    }
    return SUCCESS;
}

/**
*\*\name    SetSysClockToPLL.
*\*\fun     Selects PLL clock as System clock source and configure HCLK, PCLK2
*\*\         and PCLK1 prescalers.
*\*\param   none
*\*\note    PLL frequency must be greater than or equal to 32MHz.
*\*\return  none 
**/
ErrorStatus SetSysClockToPLL(uint32_t freq, uint8_t src)
{
    uint32_t pllsrcclk;
    uint32_t pllsrc;
    uint32_t pllmul;
    uint32_t plldiv = RCC_PLLDIVCLK_DISABLE;
    uint32_t latency;
    uint32_t pclk1div, pclk2div;
    uint32_t msi_ready_flag = RESET;
    uint32_t timeout_value = 0xFFFFFF; 

    if (HSE_VALUE != 8000000)
    {
        /* HSE_VALUE == 8000000 is needed in this project! */
        return ERROR;
    }

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/

    if ((src == SYSCLK_PLLSRC_HSI)         || (src == SYSCLK_PLLSRC_HSIDIV2) 
     || (src == SYSCLK_PLLSRC_HSI_PLLDIV2) || (src == SYSCLK_PLLSRC_HSIDIV2_PLLDIV2))
    {
        /* Enable HSI */
        RCC_ConfigHsi(RCC_HSI_ENABLE);

        /* Wait till HSI is ready */
        HSIStartUpStatus = RCC_WaitHsiStable();

        if (HSIStartUpStatus != SUCCESS)
        {
            /* If HSI fails to start-up, the application will have wrong clock
               configuration. User can add here some code to deal with this
               error */
            return ERROR;
        }

        if ((src == SYSCLK_PLLSRC_HSIDIV2) || (src == SYSCLK_PLLSRC_HSIDIV2_PLLDIV2))
        {
            pllsrc = RCC_PLL_HSI_PRE_DIV2;
            pllsrcclk = HSI_VALUE/2;

            if(src == SYSCLK_PLLSRC_HSIDIV2_PLLDIV2)
            {
                plldiv = RCC_PLLDIVCLK_ENABLE;
                pllsrcclk = HSI_VALUE/4;
            }
        } else if ((src == SYSCLK_PLLSRC_HSI) || (src == SYSCLK_PLLSRC_HSI_PLLDIV2))
        {
            pllsrc = RCC_PLL_HSI_PRE_DIV1;
            pllsrcclk = HSI_VALUE;

            if(src == SYSCLK_PLLSRC_HSI_PLLDIV2)
            {
                plldiv = RCC_PLLDIVCLK_ENABLE;
                pllsrcclk = HSI_VALUE/2;
            }
        }

    } else if ((src == SYSCLK_PLLSRC_HSE)         || (src == SYSCLK_PLLSRC_HSEDIV2) 
            || (src == SYSCLK_PLLSRC_HSE_PLLDIV2) || (src == SYSCLK_PLLSRC_HSEDIV2_PLLDIV2))
    {
        /* Enable HSE */
        RCC_ConfigHse(RCC_HSE_ENABLE);

        /* Wait till HSE is ready */
        HSEStartUpStatus = RCC_WaitHseStable();

        if (HSEStartUpStatus != SUCCESS)
        {
            /* If HSE fails to start-up, the application will have wrong clock
               configuration. User can add here some code to deal with this
               error */
            return ERROR;
        }

        if ((src == SYSCLK_PLLSRC_HSEDIV2) || (src == SYSCLK_PLLSRC_HSEDIV2_PLLDIV2))
        {
            pllsrc = RCC_PLL_SRC_HSE_DIV2;
            pllsrcclk = HSE_VALUE/2;

            if(src == SYSCLK_PLLSRC_HSEDIV2_PLLDIV2)
            {
                plldiv = RCC_PLLDIVCLK_ENABLE;
                pllsrcclk = HSE_VALUE/4;
            }
        } else if ((src == SYSCLK_PLLSRC_HSE) || (src == SYSCLK_PLLSRC_HSE_PLLDIV2))
        {
            pllsrc = RCC_PLL_SRC_HSE_DIV1;
            pllsrcclk = HSE_VALUE;

            if(src == SYSCLK_PLLSRC_HSE_PLLDIV2)
            {
                plldiv = RCC_PLLDIVCLK_ENABLE;
                pllsrcclk = HSE_VALUE/2;
            }
        }
    }

    latency = (freq/32000000);
    
    if(freq > 32000000)
    {
        pclk1div = RCC_HCLK_DIV4;
        pclk2div = RCC_HCLK_DIV2;
    }
    else if(freq > 16000000)
    {
        pclk1div = RCC_HCLK_DIV2;
        pclk2div = RCC_HCLK_DIV1;
    }
    else
    {
        pclk1div = RCC_HCLK_DIV1;
        pclk2div = RCC_HCLK_DIV1;
    }
    
    if(((freq % pllsrcclk) == 0) && ((freq / pllsrcclk) >= 2) && ((freq / pllsrcclk) <= 32))
    {
        pllmul = (freq / pllsrcclk);
        if(pllmul <= 16)
        {
            pllmul = ((pllmul - 2) << 18);
        }
        else
        {
            pllmul = (((pllmul - 17) << 18) | (1 << 27));
        }
    }
    else
    {
        /* Cannot make a PLL multiply factor to freq. */
        elog_info(CAT,"Cannot make a PLL multiply factor to freq..\n");
        return ERROR;
    }

    /* Cheak if MSI is Ready */
    if(RESET == RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_MSIRD))
    {
        /* Enable MSI and Config Clock */
        RCC_ConfigMsi(RCC_MSI_ENABLE, RCC_MSI_RANGE_4M);
        /* Waits for MSI start-up */
        if(SUCCESS != RCC_WaitMsiStable())
        {
            return ERROR;
        }

        msi_ready_flag = SET;
    }

    /* Select MSI as system clock source */
    RCC_ConfigSysclk(RCC_SYSCLK_SRC_MSI);

    FLASH_SetLatency(latency);

    /* HCLK = SYSCLK */
    RCC_ConfigHclk(RCC_SYSCLK_DIV1);

    /* PCLK2 = HCLK */
    RCC_ConfigPclk2(pclk2div);

    /* PCLK1 = HCLK */
    RCC_ConfigPclk1(pclk1div);

    /* Disable PLL */
    RCC_EnablePll(DISABLE);

    RCC_ConfigPll(pllsrc, pllmul, plldiv);

    /* Enable PLL */
    RCC_EnablePll(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_CTRL_FLAG_PLLRDF) == RESET)
    {
        if ((timeout_value--) == 0)
        {
            return ERROR;
        }
    }

    /* Select PLL as system clock source */
    RCC_ConfigSysclk(RCC_SYSCLK_SRC_PLLCLK);

    /* Wait till PLL is used as system clock source */
    timeout_value = 0xFFFFFF;
    while (RCC_GetSysclkSrc() != 0x0C)
    {
        if ((timeout_value--) == 0)
        {
            return ERROR;
        }
    }

    if(msi_ready_flag == SET)
    {
        /* MSI oscillator OFF */
        RCC_ConfigMsi(RCC_MSI_DISABLE, RCC_MSI_RANGE_4M);
    }
    return SUCCESS;
}

// void NVIC_Configuration(void)
// {
//     NVIC_InitType NVIC_InitStructure;

//     /* Enable and configure RCC global IRQ channel */
//     NVIC_InitStructure.NVIC_IRQChannel                   = RCC_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
//     NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
// }

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line assert_param error line source number
 */
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %u\r\n",
       file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}

#endif

/**
 * @}
 */

/**
 * @}
 */

