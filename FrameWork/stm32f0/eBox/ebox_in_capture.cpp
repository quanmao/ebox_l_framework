/**
  ******************************************************************************
  * @file    incapture.cpp
  * @author  shentq
  * @version V2.1
  * @date    2017/07/23
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
#include "mcu_define.h"
#include "ebox_base.h"
#include "ebox_nvic.h"
#include "ebox_in_capture.h"
#include "ebox_timer_it.h"

typedef void (*_CCEnableIT)(TIM_TypeDef *);         //使能捕获中断
typedef uint32_t (*_getCapture)(TIM_TypeDef *TIMx); //捕获函数

typedef struct
{
    uint32_t ch;
    _CCEnableIT CCIT;
} TimCH_T;

//static const TimCH_T TimTable[] = {
//	{LL_TIM_CHANNEL_CH1,&LL_TIM_IC_GetCaptureCH1,&LL_TIM_EnableIT_CC1},
//	{LL_TIM_CHANNEL_CH2,&LL_TIM_IC_GetCaptureCH2,&LL_TIM_EnableIT_CC2},
//	{LL_TIM_CHANNEL_CH3,&LL_TIM_IC_GetCaptureCH3,&LL_TIM_EnableIT_CC3},
//	{LL_TIM_CHANNEL_CH4,&LL_TIM_IC_GetCaptureCH4,&LL_TIM_EnableIT_CC4}
//};

static const TimCH_T TimTable[] = {
    {LL_TIM_CHANNEL_CH1, &LL_TIM_EnableIT_CC1},
    {LL_TIM_CHANNEL_CH2, &LL_TIM_EnableIT_CC2},
    {LL_TIM_CHANNEL_CH3, &LL_TIM_EnableIT_CC3},
    {LL_TIM_CHANNEL_CH4, &LL_TIM_EnableIT_CC4}};
		
#if EBOX_DEBUG
#include "bsp_ebox.h"
#define  _DEBUG false
#endif

#if _DEBUG
#define  CAP_DEBUG(...) DBG("[CAP]  "),DBG(__VA_ARGS__)
#else
#define  CAP_DEBUG(...)
#endif

extern uint32_t t1_overflow_times;
extern uint32_t t2_overflow_times;
extern uint32_t t3_overflow_times;
extern uint32_t t4_overflow_times;

E_INCAPTURE::E_INCAPTURE(TIM_TypeDef *TIMx, E_GPIO *pin) : T_BASE(TIMx)
{
    _period = 0xffff;
    _prescaler = 1;
    _polarity = Falling;
    _highCapture = 0;
    _lowCapture = 0;
    _capture = 0;

    _chIndex = getIndex(pin->id, TIM_MAP, (uint32_t)TIMx);
    pin->mode(TIM_MAP[_chIndex]._pinMode,TIM_MAP[_chIndex]._pinAf);
    _chIndex = (uint8_t)(TIM_MAP[_chIndex]._periph);
}

void E_INCAPTURE::begin(uint16_t prescaler, ICMode_t mode)
{
    _prescaler = prescaler;
    _enableClock();
		LL_TIM_EnableARRPreload(_timx);
    _setPerPsc();
//		LL_TIM_ClearFlag_UPDATE(_timx);
    _updateClock();
    _timeClock = _clock / _prescaler;

    switch ((uint32_t)_timx)
    {
#if (USE_TIM1 && defined TIM1_BASE)
    case (uint32_t)TIM1_BASE:
        _overflow_times = &t1_overflow_times;
        _tIndex = tim1 * its;
        break;
#endif
#if (USE_TIM2 && defined TIM2_BASE)
    case (uint32_t)TIM2_BASE:
        _overflow_times = &t2_overflow_times;
        _tIndex = tim2 * its;
        break;
#endif
#if (USE_TIM3 && defined TIM3_BASE)
    case (uint32_t)TIM3_BASE:
        _overflow_times = &t3_overflow_times;
        _tIndex = tim3 * its;
        break;
#endif
#if (USE_TIM4 && defined TIM4_BASE)
    case (uint32_t)TIM4_BASE:
        _overflow_times = &t4_overflow_times;
        _tIndex = tim4 * its;
        break;
#endif
#if (USE_TIM5 && defined TIM5_BASE)
    case (uint32_t)TIM5_BASE:
        _tIndex = tim4 * its;
        break;
#endif
#if (USE_TIM6 && defined TIM6_BASE)
    case (uint32_t)TIM6_BASE:
        _tIndex = tim6 * its;
        break;
#endif
#if (USE_TIM7 && defined TIM7_BASE)
    case (uint32_t)TIM7_BASE:
        _tIndex = tim7 * its;
        break;
#endif
    }

    switch (_chIndex)
    {
#if (USE_CH1)
    case TIMxCH1:
        _tIndex += cc1;
        ccr = (uint16_t *)&_timx->CCR1;
        break;
#endif
#if (USE_CH2)
    case TIMxCH2:
        ccr = (uint16_t *)&_timx->CCR2;
        _tIndex += cc2;
        break;
#endif
#if (USE_CH3)
    case TIMxCH3:
        ccr = (uint16_t *)&_timx->CCR3;
        _tIndex += cc3;
        break;
#endif
#if (USE_CH4)
    case TIMxCH4:
        ccr = (uint16_t *)&_timx->CCR4;
        _tIndex += cc4;
        break;
#endif
    }
		if(_timx == TIM1){
			 _nvic(ENABLE, 0,1);
		}else{
    _nvic(ENABLE, 0);
		}

    timIrqAdd(_tIndex, (&E_INCAPTURE::_irqHandler), (uint32_t)this);
    if (mode == SIMPLE)
        attach(this, &E_INCAPTURE::simple_event);
    else
        attach(this, &E_INCAPTURE::complex_event);

    _setMode();
    _start();
    LL_TIM_EnableIT_UPDATE(_timx);
}

/**
 *@brief    设置为捕获模式
 *@param    none
 *@retval   频率
*/
void E_INCAPTURE::_setMode()
{
    /* Input capture mode configuration */
    /************************************/
    /* Select the active input: IC1 = TI1FP1 */
    LL_TIM_IC_SetActiveInput(_timx, TimTable[_chIndex].ch, LL_TIM_ACTIVEINPUT_DIRECTTI);
    /* Configure the input filter duration: no filter needed */
    LL_TIM_IC_SetFilter(_timx, TimTable[_chIndex].ch, LL_TIM_IC_FILTER_FDIV1);
    /* Set input prescaler: prescaler is disabled */
    LL_TIM_IC_SetPrescaler(_timx, TimTable[_chIndex].ch, LL_TIM_ICPSC_DIV1);
    /* Select the edge of the active transition on the TI1 channel: rising edge */
    LL_TIM_IC_SetPolarity(_timx, TimTable[_chIndex].ch, _polarity);

    /**************************/
    /* TIM1 interrupts set-up */
    /**************************/
    /* Enable the capture/compare interrupt for channel 1 */
    TimTable[_chIndex].CCIT(_timx);
    /***********************/
    /* Start input capture */
    /***********************/
    /* Enable output channel 1 */
    LL_TIM_CC_EnableChannel(_timx, TimTable[_chIndex].ch);
}

/**
 *@brief    设置采样极性
 *@param    none
 *@retval   频率
*/
void E_INCAPTURE::SetPolarity(ICPolarity_t polarity)
{
    _polarity = polarity;
    LL_TIM_IC_SetPolarity(_timx, TimTable[_chIndex].ch, _polarity);
}

void E_INCAPTURE::complex_event()
{
    uint32_t capture = 0;
    volatile uint32_t now = 0;
    now = *ccr + *_overflow_times; //get capture value
    capture = now - _lastValue;
    _lastValue = now;

    if (_polarity == Falling) //检测到下降沿，测量高电平时间完成
    {
        _highCapture = capture;
        SetPolarity(Rising); //切换检测上升沿
    }
    else
    {
        _lowCapture = capture;
        SetPolarity(Falling); //切换检测下降沿
    }
    if ((_highCapture != 0) && (_lowCapture != 0))
        _available = true;
}

void E_INCAPTURE::simple_event()
{
    volatile uint32_t now = *ccr + (*_overflow_times); //get capture value
    _capture = now - _lastValue;
    _lastValue = now;
    _available = true;
}

bool E_INCAPTURE::update_resault()
{
    if (_available == false)
        return false;
    else
    {
        if (_capture != 0) //简单模式
        {
            res.frq = (_timeClock * 100.0 / _capture);
            res.peroid = (_capture * 100000000.0 / (_timeClock));
        }
        else //高级模式
        {
            res.frq = (_timeClock * 100.0 / ((_highCapture + _lowCapture)));
            res.peroid = ((_highCapture + _lowCapture) * 100000000.0 / (_timeClock));
            res.high_duty = (_highCapture * 10000.0 / (_highCapture + _lowCapture));
            res.low_duty = (_lowCapture * 10000.0 / (_highCapture + _lowCapture));
            res.high_time = ((_highCapture)*100000000.0 / (_timeClock));
            res.low_time = ((_lowCapture)*100000000.0 / (_timeClock));
        }
        _available = false;
    }
		return true;
}

uint32_t E_INCAPTURE::get_timer_clock()
{
    return _clock / _prescaler;
}

void E_INCAPTURE::_irqHandler(uint32_t id)
{
    E_INCAPTURE *handler = (E_INCAPTURE *)id;
    handler->_pirq.call();
}
