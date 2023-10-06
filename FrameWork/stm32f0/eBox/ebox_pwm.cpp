/**
  ******************************************************************************
  * @file    pwm.cpp
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
#include "ebox_gpio.h"
#include "ebox_timer.h"
#include "ebox_timer_it.h"
#include "ebox_pwm.h"

typedef struct
{
    uint32_t ch;
    pfun OCsetCompare;
}TimCH_T;

static const TimCH_T TimTable[] = {
	{LL_TIM_CHANNEL_CH1, &LL_TIM_OC_SetCompareCH1},
	{LL_TIM_CHANNEL_CH2, &LL_TIM_OC_SetCompareCH2},
	{LL_TIM_CHANNEL_CH3, &LL_TIM_OC_SetCompareCH3},
	{LL_TIM_CHANNEL_CH4, &LL_TIM_OC_SetCompareCH4}};
	
#if EBOX_DEBUG
#include "bsp_ebox.h"
#define  _DEBUG false
#endif

#if _DEBUG
#define  PWM_DEBUG(...) DBG("[PWM]  "),DBG(__VA_ARGS__)
#else
#define  PWM_DEBUG(...)
#endif

E_PWM::E_PWM(TIM_TypeDef *TIMx, E_GPIO *pin) : T_BASE(TIMx)
{
	_timx = TIMx;
	_index = getIndex(pin->id, TIM_MAP, (uint32_t)TIMx);
	pin->mode(TIM_MAP[_index]._pinMode, TIM_MAP[_index]._pinAf);
	_index = (uint8_t)(TIM_MAP[_index]._periph);
}

/**
 *@brief    ����TIMΪPWMģʽ
 *@param    NONE
 *@retval   NONE
*/
void E_PWM::_setMode(void)
{
	LL_TIM_EnableARRPreload(_timx);
	/*********************************/
	/* Output waveform configuration */
	/*********************************/
	/* Set output mode */
	LL_TIM_OC_SetMode(_timx, TimTable[_index].ch, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_EnablePreload(_timx, TimTable[_index].ch);
	/* Enable output channel */
	LL_TIM_CC_EnableChannel(_timx, TimTable[_index].ch);
}

/**
 *@brief    ����PWM���
 *@param    frq  Ҫ�����Ƶ�ʣ���λhz. 0-���Ƶ�ʡ�
 *		   duty ռ�ձ� 0-1000
 *@retval   NONE
*/
void E_PWM::begin(uint32_t frq, uint16_t duty)
{
	_enableClock();
	_setMode();
	setFrequency(frq);
	setDutyCycle(duty);
	if (IS_TIM_BREAK_INSTANCE(_timx))
	{
		LL_TIM_EnableAllOutputs(_timx);
	}
	_start();
	/* Force update generation ǿ�Ƹ��� */
	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    ����Ƶ��,��ͬ���������ռ�ձ�;���Ƶ�ʿ�ͨ��GetMaxFrequency()���
 *@param    frq  Ҫ�����Ƶ�ʣ���λhz. 0-���Ƶ�ʡ�
 *@retval   None
*/
void E_PWM::setFrequency(uint32_t frq)
{
	_updateClock();
	PWM_DEBUG("CPU_CLOCK: %d Mhz",_clock/1000000);
	if (frq >= _clock) //����Ƶ�ʣ���֤����1%����
		frq = _clock;

	_calculate(frq);
	_accuracy = ((_period >= 100) && (_period < 1000)) ? 1 : 0;
	_setPerPsc();
	setDutyCycle(_duty);
}

/**
 *@brief    ����PWMռ�ձ�.Ƶ��С�ڵ���ʮ��֮һ���Ƶ��ʱ���ֱ���Ϊǧ��֮һ
 *		   Ƶ����ʮ��֮һ���Ƶ�ʺ����Ƶ��֮�䣬�ֱ���Ϊ�ٷ�֮һ
 *@param    duty 0-1000 ��Ӧ0%-100%
 *@retval   none
*/
void E_PWM::setDutyCycle(uint16_t duty)
{
	uint16_t pulse = 0;
	float percent; // �ٷֱ�

	_duty = duty > 1000 ? 1000 : duty;
	// �ٷ�֮һ����
	if (_accuracy)
	{
		_duty = (_duty < 10 && _duty != 0) ? 10 : duty;
	}

	percent = (_duty / 1000.0);
	pulse = (uint16_t)((percent * _period));
	//    _OCsetCompare(_timx, pulse);
	TimTable[_index].OCsetCompare(_timx, pulse);
	//	/* Force update generation ǿ�Ƹ��� */
	//	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    ����PWMռ�ձ�.Ƶ��С�ڵ���ʮ��֮һ���Ƶ��ʱ���ֱ���Ϊǧ��֮һ
 *		   Ƶ����ʮ��֮һ���Ƶ�ʺ����Ƶ��֮�䣬�ֱ���Ϊ�ٷ�֮һ
 *@param    duty 0-1000 ��Ӧ0%-100%
 *@retval   none
*/
void E_PWM::setDuty(float duty)
{
	uint16_t pulse = 0;
	pulse = (uint16_t)((duty * _period));
	TimTable[_index].OCsetCompare(_timx, pulse);
	//	/* Force update generation ǿ�Ƹ��� */
	//	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    ���ü���
 *@param    flag:  1 HIGH  0 LOW
 *@retval   None
*/
void E_PWM::setPolarity(uint8_t Polarity)
{
	LL_TIM_OC_SetPolarity(_timx, TimTable[_index].ch, (Polarity == 1) ? (LL_TIM_OCPOLARITY_HIGH) : (LL_TIM_OCPOLARITY_LOW));
	/* Force update generation ǿ�Ƹ��� */
	LL_TIM_GenerateEvent_UPDATE(_timx);
}
