/**
  ******************************************************************************
  * @file    pwm.h
  * @author  shentq
  * @version V2.1
  * @date    2016/11/04
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

#ifndef __PWM_H
#define __PWM_H
#include "ebox_timer.h"

/*
	1.֧��TIM1,2,3,4�µ�CH1-4ͨ��������16��ͨ��
	2.������ʱ����pwm�Ĳ����޸ĺ���������setfrq��������ı�ռ�ձ�,���ǻ�Ӱ��ͬ��ʱ��������ͨ���Ĺ�����
    3.pwm��ռ�õĶ�ʱ����ʱ��ԴƵ�ʿ���ͨ��get_timer_clock()��ã�
    4.PWM���Ƶ���������ͨ��get_max_frq()��ã�
    5.PWM��ǰ����ľ��ȿ���ͨ��get_accuracy()��ã�
	ע�⣺  1.��ʱû������֧��remap���š�
                ���ʹ��remap������Ҫ�Լ���ִ�����ʼ����������ִ����Ӧ��remap������Ȼ����Ҫremap�����ų�ʼ��ΪAF_PPģʽ
                ��ϸ������ο�pwm&remap.cpp
            2.ʹ������Ӧ��timxΪpwm����󣬸�tim�Ͳ����ٳ�ʼ��������Ĺ���
            3.��ʼ��pwm��ֻ�е���setDuty������Ż����pwm
            4.���duty���ڵ���1000����������ߵ�ƽ

    ��ʱ�����Ŷ�Ӧ�б�
    ͨ�� CH1  CH2  CH3  CH4
    TIM1 PA8  PA9  PA10 PA11
    TIM2 PA0  PA1  PA2  PA3
    TIM3 PA6  PA7  PA0  PA1
    TIM4 PB6  PB7  PB8  PB9
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *�޸���set_dutyʱ���²����������
 *      *�޸���оƬ��Ƶ�䶯���¶�ʱ��PWMƵ�ʱ仯��bug��֧��������Ƶ����������
 *      *������get_timer_clock(),get_max_frq(),get_accuracy()���������ӿ�
 * -shentq                  -version 2.0(2016/11/4)
 *      *���ӶԶ�ʱ��1��֧��
 */
// ����ָ��,ָ��LL_TIM_OC_SetCompareCH4(TIM_TypeDef *TIMx, uint32_t CompareValue) ����
typedef void (*pfun)(TIM_TypeDef *, uint32_t);

class E_PWM : public T_BASE
{
public:
	E_PWM(TIM_TypeDef *TIMx, E_GPIO *pin);

	void begin(uint32_t frq, uint16_t duty);
	//�����������1���Ƚ�ƥ�������͵�ƽ��Ĭ�����ã���0���Ƚ�ƥ�������ߵ�ƽ
	void setPolarity(uint8_t porlicy);
	void setFrequency(uint32_t frq);		// ��СƵ��0.02Hz,����ֵfrq��ʵ�ʵ�100�������磬����5����ʵ��Ƶ��λ0.05hz
	void setDutyCycle(uint16_t duty);
	void setDuty(float duty);
	//��ȡ��ǰPWM�����Ƶ��
	uint32_t getMaxFrequency(void) { return _clock / 100; };
	uint8_t  getAccuracy(){return _accuracy;};

private:
	uint8_t _index;
	uint16_t _duty;	// ռ�ձ�
	uint8_t _accuracy; // ����
	void _setMode(void);
};

#endif