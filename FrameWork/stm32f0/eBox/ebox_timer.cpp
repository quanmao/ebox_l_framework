/**
  ******************************************************************************
  * @file    timer.cpp
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
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_rcc.h"
#include "ebox_base.h"
#include "ebox_nvic.h"
#include "ebox_timer.h"
#include "ebox_timer_it.h"

#if EBOX_DEBUG
#include "bsp_ebox.h"
#define  _DEBUG false
#endif

#if _DEBUG
#define  TIMER_DEBUG(...) DBG("[TIMER]  "),DBG(__VA_ARGS__)
#else
#define  TIMER_DEBUG(...)
#endif

/**
 *@brief    获取时钟频率
 *@param    NONE
 *@retval   当前TIM频率
*/
void TIM::_updateClock(void)
{
    Clock_t clock;
    eboxGetClockInfo(&clock);
    // 计算TIM时钟频率
    _clock = LL_RCC_GetAPB1Prescaler() == 0 ? clock.pclk1 : (clock.pclk1 * 2);
}

void TIM::_nvic(FunctionalState enable, uint8_t preemption_priority,uint8_t index)
{
    IRQn_Type irq = eboxDevToIrq(IrqTableTIM, (uint32_t)_timx, index);
    eboxNvicIrqSetPriority(irq, preemption_priority);
    (enable != DISABLE) ? (eboxNvicIrqEnable(irq)) : eboxNvicIrqDisable(irq);
}

/**
 *@brief    设置TIM Prescaler & Period
 *@param    NONE
 *@retval   none
*/
void TIM::_setPerPsc(void)
{
    /* Set the pre-scaler value */
    LL_TIM_SetPrescaler(_timx, _prescaler - 1);
    /* Set the auto-reload value*/
    LL_TIM_SetAutoReload(_timx, _period);
}

/**
 *@name     T_BASE::calculate(uint32_t frq)
 *@brief    根据给定频率，计算period,prescaler参数
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *@retval   None
*/
void T_BASE::_calculate(uint32_t frq)
{
    uint32_t period = 0;    // 周期
    uint32_t prescaler = 1; // 预分频

    for (; prescaler <= 0xffff; prescaler++)
    {
        period = ((_clock * 100.0) / prescaler / frq);
        if (0xffff >= period)
        {
            TIMER_DEBUG("TIM: %d ,Timer: %d period = %d , prescaler = %d  \r\n", _clock, frq, period, prescaler);
            break;
        }
    }
    _period = period;
    _prescaler = prescaler;
}

/*********************************  E_TIME  *****************************************/
/**
 *@brief    设置定时器时间
 *@param    uint32_t us
 *@retval   NONE
*/
void E_TIMER::setUs(uint16_t us)
{
    uint32_t frq = 100000000 / us;
    setFrequency(frq);
}

void E_TIMER::setMs(uint16_t ms)
{
    uint32_t frq = 100000 / ms;
    setFrequency(frq);
}

E_TIMER::E_TIMER(TIM_TypeDef *TIMx) : T_BASE(TIMx)
{
    switch ((uint32_t)_timx)
    {
#if (USE_TIM1 && defined TIM1_BASE)
    case (uint32_t)TIM1_BASE:
        _tIndex = tim1 * its + update;
        break;
#endif
#if (USE_TIM2 && defined TIM2_BASE)
    case (uint32_t)TIM2_BASE:
        _tIndex = tim2 * its + update;
        break;
#endif
#if (USE_TIM3 && defined TIM3_BASE)
    case (uint32_t)TIM3_BASE:
        _tIndex = tim3 * its + update;
        break;
#endif
#if (USE_TIM4 && defined TIM4_BASE)
    case (uint32_t)TIM4_BASE:
        _tIndex = tim4 * its + update;
        break;
#endif
#if (USE_TIM5 && defined TIM5_BASE)
    case (uint32_t)TIM5_BASE:
        _tIndex = tim5 * its + update;
        break;
#endif
#if (USE_TIM6 && defined TIM6_BASE)
    case (uint32_t)TIM6_BASE:
        _tIndex = tim6 * its + update;
        break;
#endif
#if (USE_TIM7 && defined TIM7_BASE)
    case (uint32_t)TIM7_BASE:
        _tIndex = TIM7_IT_Update;
        break;
#endif
    }
    timIrqAdd(_tIndex, (&E_TIMER::_irqHandler), (uint32_t)this);
}

/**
 *@brief    设置定时器频率
 *@param    uint32_t frq  0-TIM时钟频率
 *@retval   NONE
*/
void E_TIMER::setFrequency(uint32_t frq)
{
    _updateClock();
    if (frq >= _clock) //控制频率，保证其有1%精度
        frq = _clock;
    _calculate(frq);
    _enableClock();
    _setPerPsc();
}

/**
 *@brief    启动定时器
 *@param    NONE
 *@retval   NONE
*/
void E_TIMER::start(void)
{ 
	_nvic(ENABLE,0);
	LL_TIM_EnableIT_UPDATE(_timx);
	_start();  
	/* Force update generation 强制更新 */
	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    关闭定时器
 *@param    NONE
 *@retval   NONE
*/
void E_TIMER::stop(void)
{
    LL_TIM_DisableCounter(_timx);
}

void E_TIMER::_irqHandler(uint32_t id)
{
    E_TIMER *hander = (E_TIMER *)id;
    hander->_pirq.call();
}

void E_TIMER::attach(void (*fptr)(void))
{
    _pirq.attach(fptr);
}
