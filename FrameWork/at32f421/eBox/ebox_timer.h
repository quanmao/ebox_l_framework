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
	1.֧��tim1,2,3,4,5,6,7
    2.��ʱ����ʱ��ԴƵ�ʿ���ͨ��get_timer_clock()��ã�
    3.��ʱ������ж�Ƶ�ʿ���get_max_frq()��ã�
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *������TIM1��֧�֣�
 *      *������get_timer_clock(),get_max_frq()���������ӿ�
 *      *�޸���оƬ��Ƶ�䶯���¶�ʱ���ж�Ƶ�ʱ仯��bug��֧��������Ƶ����������
 */

class TIM
{
public:
  TIM(TIM_TypeDef *TIMx) { _timx = TIMx; };
  void _updateClock();
  void _enableClock() { eboxRccCmd(RccTableTIM, (uint32_t)_timx, ENABLE); }; //ʹ��TIMʱ��
  void _start() { LL_TIM_EnableCounter(_timx); };
  void _setPerPsc();
  void _nvic(FunctionalState enable, uint8_t preemption_priority,uint8_t index = 0);

public:
  TIM_TypeDef *_timx;  // TIM����
  uint8_t _tIndex;     // TIM����
  uint32_t _period;    // ���� 0-65535
  uint32_t _prescaler; // TIMԤ��Ƶֵ,��ֵ��Χ1-65536,�������ջ�תΪ0-65535
  uint32_t _clock;
};

class T_BASE : protected TIM
{
public:
  T_BASE(TIM_TypeDef *TIMx) : TIM(TIMx){};
  uint32_t getSourceClock(void) { return _clock;}; // ��ȡTIMʱ��Ƶ��
protected:
  void _calculate(uint32_t frq);
};

class E_TIMER : public T_BASE
{
public:
  E_TIMER(TIM_TypeDef *TIMx);
  // ����time����,us
  void setUs(uint16_t us);
  void setMs(uint16_t ms);         // ���Լ53s
    // ����timeƵ��,hz
  void setFrequency(uint32_t frq); // ��СƵ��0.02Hz,����ֵfrq��ʵ�ʵ�100�������磬����5����ʵ��Ƶ��λ0.05hz
  void start();
  void stop();

    uint32_t getMaxFrequency(void){return _clock;};

  // ���жϻص�����
  void attach(void (*fptr)(void));
  template <typename T>
  // �����Ա������Ϊ�ص�����������::������
  void attach(T *tptr, void (T::*mptr)(void))
  {
    _pirq.attach(tptr, mptr);
  }

private:
  FunctionPointer _pirq;
  static void _irqHandler(uint32_t id);
};

#endif
