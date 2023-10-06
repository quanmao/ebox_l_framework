/**
  ******************************************************************************
  * @file    ebox_it.cpp
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

//#include "ebox_core.h"
#include "mcu_define.h"
#include "ebox_timer_it.h"
/*
定时器中断简介：
    此文件提供了定时器的中断入口。定义了所有定时器对象this指针入口irqIds[TIM_IRQ_ID_NUM]。
用户可以通过Tim_It_Index来查询每一个定时器所有this指针入口的index值。并通过tim_irq_handler类型的
方法入口绑定类的静态方法入口。实现了寻找对象并定位方法的两个步骤。
*/


__IO uint32_t t1_overflow_times = 0;
__IO uint32_t t2_overflow_times = 0;
__IO uint32_t t3_overflow_times = 0;
__IO uint32_t t4_overflow_times = 0;


static irqHandler pIrqHandler;
static uint32_t irqIds[tims][its];//保存对象this指针

//index:定时器保存this指针数组列表中的索引值，实现将特定的对象绑定到特定的入口
//handler：类的静态方法
//id:对象的this指针
void timIrqAdd(uint8_t index, irqHandler handler, uint32_t id)
{
		uint32_t *p = (uint32_t *)irqIds;
    p[index] = id;//将对象this指针保存至列表中
    pIrqHandler =  handler;//类的静态方法
}

extern "C" {

    void TIM1_UP_IRQHandler(void)
    {
        if (LL_TIM_IsActiveFlag_UPDATE(TIM1))
        {
            t1_overflow_times++;
            pIrqHandler(irqIds[tim1][update]);
            LL_TIM_ClearFlag_UPDATE(TIM1);
        }
    }

    void TIM1_CC_IRQHandler(void)
    {
        /* Check whether CC1 interrupt is pending */
        if (LL_TIM_IsActiveFlag_CC1(TIM1) == 1)
        {
            pIrqHandler(irqIds[tim1][cc1]);
            LL_TIM_ClearFlag_CC1(TIM1);
        }
        if (LL_TIM_IsActiveFlag_CC2(TIM1) == 1)
        {
            pIrqHandler(irqIds[tim1][cc2]);
            LL_TIM_ClearFlag_CC2(TIM1);
        }
        if (LL_TIM_IsActiveFlag_CC3(TIM1) == 1)
        {
            pIrqHandler(irqIds[tim1][cc3]);
            LL_TIM_ClearFlag_CC3(TIM1);
        }
    }

#ifdef TIM2
    void TIM2_IRQHandler(void)
    {
        /* Check whether update interrupt is pending */
        if (LL_TIM_IsActiveFlag_UPDATE(TIM2) && LL_TIM_IsEnabledIT_UPDATE(TIM2))
        {
            t2_overflow_times++;
            pIrqHandler(irqIds[tim2][update]);
            LL_TIM_ClearFlag_UPDATE(TIM2);
        }
        /* CC interrupt */
        if (LL_TIM_IsActiveFlag_CC1(TIM2) && LL_TIM_IsEnabledIT_CC1(TIM2))
        {
            pIrqHandler(irqIds[tim2][cc1]);
            LL_TIM_ClearFlag_CC1(TIM2);
        }
//        if (LL_TIM_IsActiveFlag_CC2(TIM2) == 1)
//        {
//            pIrqHandler(irqIds[TIM2_IT_CC2]);
//            LL_TIM_ClearFlag_CC2(TIM2);
//        }
//        if (LL_TIM_IsActiveFlag_CC3(TIM2) == 1)
//        {
//            pIrqHandler(irqIds[TIM2_IT_CC3]);
//            LL_TIM_ClearFlag_CC3(TIM2);
//        }
//        if (LL_TIM_IsActiveFlag_CC4(TIM2) == 1)
//        {
//            pIrqHandler(irqIds[TIM2_IT_CC4]);
//            LL_TIM_ClearFlag_CC3(TIM2);
//        }
    }
#endif
#ifdef TIM3
    void TIM3_IRQHandler(void)
    {
        /* Check whether update interrupt is pending */
        if (LL_TIM_IsActiveFlag_UPDATE(TIM3) && LL_TIM_IsEnabledIT_UPDATE(TIM3) )
        {
            t3_overflow_times+= 0x10000;
            pIrqHandler(irqIds[tim3][update]);
            LL_TIM_ClearFlag_UPDATE(TIM3);
        }
        /* CC interrupt */
        if ((TIM3->SR & TIM_SR_CC1IF ) && (TIM3->DIER & TIM_DIER_CC1IE))
        {
            pIrqHandler(irqIds[tim3][cc1]);
            LL_TIM_ClearFlag_CC1(TIM3);
        }
        if ((TIM3->SR & TIM_SR_CC2IF ) && (TIM3->DIER & TIM_DIER_CC2IE))
        {
            pIrqHandler(irqIds[tim3][cc2]);
            LL_TIM_ClearFlag_CC2(TIM3);
        }
//        if (LL_TIM_IsActiveFlag_CC3(TIM3) == 1)
//        {
//            pIrqHandler(irqIds[TIM3_IT_CC3]);
//            LL_TIM_ClearFlag_CC3(TIM3);
//        }
//        if (LL_TIM_IsActiveFlag_CC4(TIM3) == 1)
//        {
//            pIrqHandler(irqIds[TIM3_IT_CC4]);
//            LL_TIM_ClearFlag_CC3(TIM3);
//        }
    }
#endif

    void TIM4_IRQHandler(void)
    {
        /* Check whether update interrupt is pending */
        if (LL_TIM_IsActiveFlag_UPDATE(TIM4) == 1)
        {
            t2_overflow_times++;
            pIrqHandler(irqIds[tim4][update]);
            LL_TIM_ClearFlag_UPDATE(TIM4);
        }
        /* CC interrupt */
        if (LL_TIM_IsActiveFlag_CC1(TIM4) && LL_TIM_IsEnabledIT_CC1(TIM4))
        {
            pIrqHandler(irqIds[tim4][cc1]);
            LL_TIM_ClearFlag_CC1(TIM4);
        }
//        if (LL_TIM_IsActiveFlag_CC2(TIM2) == 1)
//        {
//            pIrqHandler(irqIds[TIM2_IT_CC2]);
//            LL_TIM_ClearFlag_CC2(TIM2);
//        }
//        if (LL_TIM_IsActiveFlag_CC3(TIM2) == 1)
//        {
//            pIrqHandler(irqIds[TIM2_IT_CC3]);
//            LL_TIM_ClearFlag_CC3(TIM2);
//        }
//        if (LL_TIM_IsActiveFlag_CC4(TIM2) == 1)
//        {
//            pIrqHandler(irqIds[TIM2_IT_CC4]);
//            LL_TIM_ClearFlag_CC3(TIM2);
//        }
    }

    //    void TIM5_IRQHandler(void)
    //    {
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_Update) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_Update);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC1) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC1);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC1);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC2) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC2);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC2);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC3) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC3);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC3);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC4) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC4);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC4);
    //        }
    //    }
    //    void TIM6_IRQHandler(void)
    //    {
    //        if(TIM_GetITStatus(TIM6 , TIM_IT_Update) == SET)
    //        {
    //            tim_irq_callback(TIM6_IT_Update);
    //            TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);
    //        }
    //    }
    //    void TIM7_IRQHandler(void)
    //    {
    //        if(TIM_GetITStatus(TIM7 , TIM_IT_Update) == SET)
    //        {
    //            tim_irq_callback(TIM7_IT_Update);
    //            TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);
    //        }
    //    }

    void NMI_Handler(void)
    {
    }

    /**
      * @brief  This function handles Hard Fault exception.
      * @param  None
      * @retval None
      */
    void HardFault_Handler(void)
    {
        /* Go to infinite loop when Hard Fault exception occurs */
        while (1)
        {
        }
    }

    /**
      * @brief  This function handles Memory Manage exception.
      * @param  None
      * @retval None
      */
    void MemManage_Handler(void)
    {
        /* Go to infinite loop when Memory Manage exception occurs */
        while (1)
        {
        }
    }

    /**
      * @brief  This function handles Bus Fault exception.
      * @param  None
      * @retval None
      */
    void BusFault_Handler(void)
    {
        /* Go to infinite loop when Bus Fault exception occurs */
        while (1)
        {
        }
    }

    /**
      * @brief  This function handles Usage Fault exception.
      * @param  None
      * @retval None
      */
    void UsageFault_Handler(void)
    {
        /* Go to infinite loop when Usage Fault exception occurs */
        while (1)
        {
        }
    }

    //void PendSV_Handler(void)
    //{
    //}
    /**
      * @brief  This function handles SVCall exception.
      * @param  None
      * @retval None
      */
    //void SVC_Handler(void)
    //{
    //}

    /**
      * @brief  This function handles Debug Monitor exception.
      * @param  None
      * @retval None
      */
    void DebugMon_Handler(void)
    {
    }

    /**
      * @brief  This function handles PendSVC exception.
      * @param  None
      * @retval None
      */
}
