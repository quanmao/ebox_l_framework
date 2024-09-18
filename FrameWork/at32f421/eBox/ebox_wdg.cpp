/**
  ******************************************************************************
  * @file    wdg.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_ll_iwdg.h"
#include "stm32f0xx_ll_rcc.h"
#include "ebox_wdg.h"
#include "ebox_base.h"

void E_IWWDG::begin(uint16_t ms)
{
    uint8_t pr = 0;   // 分频数 0-6
    uint16_t rlr = 0; // 重载寄存器值，12位有效位，最大值4095
    // 计算重载值
    for (pr = 0; pr < 7; pr++)
    {
        rlr = ms * 40 / (4 << pr);
        if (rlr <= 0x0fff)
            break;
    }
    // 当计算出的重载值大于0xfff时，取最大值rlr = 4095, pr = 6;
    // 此时延时Tout=(256*4095)/40 (ms)
    if (rlr > 0x0fff || pr >= 7)
    {
        pr = 6;
        rlr = 0xfff;
    }

    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1)
    {
    }

    /* Configure the IWDG with window option disabled */
    /* ------------------------------------------------------- */
    /* (1) Enable the IWDG by writing 0x0000 CCCC in the IWDG_KR register */
    /* (2) Enable register access by writing 0x0000 5555 in the IWDG_KR register */
    /* (3) Write the IWDG prescaler by programming IWDG_PR from 0 to 7 - LL_IWDG_PRESCALER_4 (0) is lowest divider*/
    /* (4) Write the reload register (IWDG_RLR) */
    LL_IWDG_Enable(IWDG);                /* (1) */
    LL_IWDG_EnableWriteAccess(IWDG);     /* (2) */
    LL_IWDG_SetPrescaler(IWDG, pr);      /* (3) */
    LL_IWDG_SetReloadCounter(IWDG, rlr); /* (4) */
}

#include "stm32f0xx_ll_wwdg.h"
void E_IWWDG::feed()
{
    LL_IWDG_ReloadCounter(IWDG);        /*reload*/
}

void E_WWDG::begin(uint16_t wr_us, uint16_t tr_us)
{
    Clock_t clock;
    eboxGetClockInfo(&clock);
    uint8_t pr, wr,clk = clock.pclk1/1000000; // 分频系数,窗口值，计数值。
    // 计算分频系数，窗口值
    for (pr = 0; pr < 4; pr++)
    {
        tr = (clk * tr_us) / (4096 * (1 << pr))-1;
        if (tr <= 0x3f)
            break;
    }
    // 约束tr,pr
    if (tr > 0x3f || pr == 4)
    {
        tr = 0x3f;
        pr = 3;
    }
    // 约束wr_us
    if(wr_us > tr_us) wr_us = tr_us;

    wr = (clk* wr_us) / (4096 * (1 << pr))-1;

    tr+=0x40;
    wr+=0x40;

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG); // 使能时钟
    LL_WWDG_SetPrescaler(WWDG, pr);    // 设置分频系数
    LL_WWDG_SetWindow(WWDG, wr);                        // 设置窗口值
    LL_WWDG_SetCounter(WWDG, tr);                       // 设置计数器值
    LL_WWDG_Enable(WWDG);                               // 启动WWDG
}

void E_WWDG::feed(){
    LL_WWDG_SetCounter(WWDG, tr);
}

void E_WWDG::getMaxMin(uint8_t pr,uint16_t &max_us, uint16_t &min_us)
{
    Clock_t clock;
    eboxGetClockInfo(&clock);
    uint16_t tmp = (clock.pclk1 / 1000000);

    max_us = 4096 * (1 << pr) * 64 / tmp;
    min_us = 4096 * (1 << pr) / tmp;
}

uint8_t E_WWDG::isWWDGRst()
{
    if (LL_RCC_IsActiveFlag_WWDGRST())
    {
        /* clear WWDG reset flag */
        LL_RCC_ClearResetFlags();
        return 1;
    }
    else
        return 0;
}

#ifdef __cplusplus
extern "C" {
#endif 



#ifdef __cplusplus
}
#endif
