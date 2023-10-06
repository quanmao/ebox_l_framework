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
��ʱ���жϼ�飺
    ���ļ��ṩ�˶�ʱ�����ж���ڡ����������ж�ʱ������thisָ�����tim_irq_ids[TIM_IRQ_ID_NUM]��
�û�����ͨ��Tim_It_Index����ѯÿһ����ʱ������thisָ����ڵ�indexֵ����ͨ��tim_irq_handler���͵�
������ڰ���ľ�̬������ڡ�ʵ����Ѱ�Ҷ��󲢶�λ�������������衣
*/

__IO uint32_t t1_overflow_times = 0;
__IO uint32_t t2_overflow_times = 0;
__IO uint32_t t3_overflow_times = 0;
__IO uint32_t t4_overflow_times = 0;

static irqHandler pIrqHandler;
static uint32_t irqIds[tims][its]; //�������thisָ��

//index:��ʱ������thisָ�������б��е�����ֵ��ʵ�ֽ��ض��Ķ���󶨵��ض������
//handler����ľ�̬����
//id:�����thisָ��
void timIrqAdd(uint8_t index, irqHandler handler, uint32_t id)
{
    uint32_t *p = (uint32_t *)irqIds;
    p[index] = id;         //������thisָ�뱣�����б���
    pIrqHandler = handler; //��ľ�̬����
}

extern "C"
{
#if (USE_TIM1 && defined TIM1)
    void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
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
        //        if (LL_TIM_IsActiveFlag_CC1(TIM1) == 1)
        //        {
        //            pIrqHandler(irqIds[tim1][cc1]);
        //            LL_TIM_ClearFlag_CC1(TIM1);
        //        }
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
#endif

#if (USE_TIM2 && defined TIM2)
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
#if (USE_TIM3 && defined TIM3)
    void TIM3_IRQHandler(void)
    {
        /* Check whether update interrupt is pending */
        if (LL_TIM_IsActiveFlag_UPDATE(TIM3) && LL_TIM_IsEnabledIT_UPDATE(TIM3))
        {
            t3_overflow_times += 0x10000;
            pIrqHandler(irqIds[tim3][update]);
            LL_TIM_ClearFlag_UPDATE(TIM3);
        }
        /* CC interrupt */
        if (LL_TIM_IsActiveFlag_CC1(TIM3) && LL_TIM_IsEnabledIT_CC1(TIM3))
        {
            pIrqHandler(irqIds[tim3][cc1]);
            LL_TIM_ClearFlag_CC1(TIM3);
        }
        if (LL_TIM_IsActiveFlag_CC2(TIM3) && LL_TIM_IsEnabledIT_CC2(TIM3))
        {
            pIrqHandler(irqIds[tim3][cc2]);
            LL_TIM_ClearFlag_CC2(TIM3);
        }
    }
#endif

    void TIM14_IRQHandler()
    {
    }
#if (USE_TIM16 && defined TIM16)
    void TIM16_IRQHandler()
    {
        /* Check whether update interrupt is pending */
        //        if (LL_TIM_IsActiveFlag_UPDATE(TIM16) && LL_TIM_IsEnabledIT_UPDATE(TIM16))
        //        {
        //            t3_overflow_times += 0x10000;
        //            pIrqHandler(irqIds[tim3][update]);
        //            LL_TIM_ClearFlag_UPDATE(TIM3);
        //        }
        /* CC interrupt */
        if (LL_TIM_IsActiveFlag_CC1(TIM16) && LL_TIM_IsEnabledIT_CC1(TIM16))
        {
            pIrqHandler(irqIds[tim16][cc1]);
            LL_TIM_ClearFlag_CC1(TIM16);
        }
        if (LL_TIM_IsActiveFlag_CC2(TIM16) && LL_TIM_IsEnabledIT_CC2(TIM16))
        {
            pIrqHandler(irqIds[tim16][cc2]);
            LL_TIM_ClearFlag_CC2(TIM16);
        }
    }
#endif
#if (USE_TIM17 && defined TIM17)
    void TIM17_IRQHandler()
    {
        /* Check whether update interrupt is pending */
        //        if (LL_TIM_IsActiveFlag_UPDATE(TIM16) && LL_TIM_IsEnabledIT_UPDATE(TIM16))
        //        {
        //            t3_overflow_times += 0x10000;
        //            pIrqHandler(irqIds[tim3][update]);
        //            LL_TIM_ClearFlag_UPDATE(TIM3);
        //        }
        /* CC interrupt */
        if (LL_TIM_IsActiveFlag_CC1(TIM17) && LL_TIM_IsEnabledIT_CC1(TIM17))
        {
            pIrqHandler(irqIds[tim17][cc1]);
            LL_TIM_ClearFlag_CC1(TIM17);
        }
        if (LL_TIM_IsActiveFlag_CC2(TIM17) && LL_TIM_IsEnabledIT_CC2(TIM17))
        {
            pIrqHandler(irqIds[tim17][cc2]);
            LL_TIM_ClearFlag_CC2(TIM17);
        }
    }
#endif

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

    // void PendSV_Handler(void)
    //{
    // }
    /**
     * @brief  This function handles SVCall exception.
     * @param  None
     * @retval None
     */
    // void SVC_Handler(void)
    //{
    // }

    /**
     * @brief  This function handles Debug Monitor exception.
     * @param  None
     * @retval None
     */
    void DebugMon_Handler(void)
    {
    }

    void WWDG_IRQHandler(void) {}
    void FLASH_IRQHandler(void) {}
    //				void RTC_IRQHandler(void){}
    void RCC_IRQHandler(void) {}
    void DMA1_Channel1_IRQHandler(void) {}
    void DMA1_Channel2_3_IRQHandler(void) {}
    void DMA1_Channel4_5_IRQHandler(void) {}
    void ADC1_IRQHandler(void) {}
    }
