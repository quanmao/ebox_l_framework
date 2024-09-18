/**
  ******************************************************************************
  * @file    gtimer.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __GTIMER_H
#define __GTIMER_H


#include "FunctionPointer.h"
/*
	1.支持tim1,2,3,4,5,6,7
    2.定时器的时钟源频率可以通过get_timer_clock()获得；
    3.定时器最大中断频率可由get_max_frq()获得；
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *增加了TIM1的支持；
 *      *增加了get_timer_clock(),get_max_frq()两个函数接口
 *      *修复了芯片主频变动导致定时器中断频率变化的bug，支持任意主频下正常工作
 */

class TIM
{
public:
  TIM(TIM_TypeDef *TIMx) { _timx = TIMx; };
  void _updateClock();
  void _enableClock() { eboxRccCmd(RccTableTIM, (uint32_t)_timx, ENABLE); }; //使能TIM时钟
  void _start() { LL_TIM_EnableCounter(_timx); };
  void _setPerPsc();
  void _nvic(FunctionalState enable, uint8_t preemption_priority,uint8_t index = 0);

public:
  TIM_TypeDef *_timx;  // TIM外设
  uint8_t _tIndex;     // TIM索引
  uint32_t _period;    // 周期 0-65535
  uint32_t _prescaler; // TIM预分频值,赋值范围1-65536,程序最终会转为0-65535
  uint32_t _clock;
};

class T_BASE : protected TIM
{
public:
  T_BASE(TIM_TypeDef *TIMx) : TIM(TIMx){};
  uint32_t getSourceClock(void) { return _clock;}; // 获取TIM时钟频率
protected:
  void _calculate(uint32_t frq);
};

class E_TIMER : public T_BASE
{
public:
  E_TIMER(TIM_TypeDef *TIMx);
  // 设置time周期,us
  void setUs(uint16_t us);
  void setMs(uint16_t ms);         // 最大约53s
    // 设置time频率,hz
  void setFrequency(uint32_t frq); // 最小频率0.02Hz,输入值frq是实际的100倍，比如，输入5，则实际频率位0.05hz
  void start();
  void stop();

    uint32_t getMaxFrequency(void){return _clock;};

  // 绑定中断回调函数
  void attach(void (*fptr)(void));
  template <typename T>
  // 绑定类成员函数作为回调函数，类名::函数名
  void attach(T *tptr, void (T::*mptr)(void))
  {
    _pirq.attach(tptr, mptr);
  }

private:
  FunctionPointer _pirq;
  static void _irqHandler(uint32_t id);
};

#endif
