/**
  ******************************************************************************
  * @file    ebox_exti.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/16
  * @brief   EXTI����,Ĭ��֧�������أ��½��ش�����16��ͨ����
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EXTI_H
#define __EXTI_H
#include "ebox_base.h"
#include "FunctionPointer.h"
#include "ebox_config.h"

// �������Ƿ�ʹ���½��أ��������жϣ�1 ʹ�ã�0 ��ʹ��
#if USE_EXTI
#define EXTI_FALL 1
#define EXTI_RISE 1
#endif
// ����ʹ�õ�����жϺ�,����ʹ��PA15��Ϊ�ⲿ�жϣ���max_exti = 16,��ʹ���˶����ж��޹أ����������ڽ����ڴ�
#define MAX_EXTI  16
/*
	1.�ṩһ��io�ж�
	*ע�⣺stm32һ���ж���EXTI_Linexֻ�����ӵ�һ��port��GPIO_Pin_x��������PA0Ϊ�ж�Դ֮��Ͳ�������PB0��PC0��Ϊ�ж�Դ
    trigger: ���Ŵ����е�������������һ������ģʽ�е�һ�֣�
        - RISING: �����ش����ж�
        - FALLING: �½��ش����ж�
        - CHANGE: �����غ��½��ؾ������ж�
 */

// ��������
enum TrigType
{
    FALL = 0,	// �½��ش���
    RISE,			// �����ش���
    FALL_RISING		// �������½���
};

class E_EXTI
{
public:
    E_EXTI(E_GPIO *exti_pin);
    void begin(PinMode_t mode = INPUT );

		// ����/�ر��ж�
    void enable(TrigType trig, uint32_t priority = 0);
    void disable(TrigType trig);
#if (EXTI_FALL | EXTI_RISE)
		// ���жϻص�����
    void attach(void (*fptr)(void), TrigType type);
		// �����Ա������Ϊ�ص�����������::������
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), TrigType type);
#endif
private:

    E_GPIO	*_pin;
    uint16_t	_extiLine;	//�ⲿ�ж�0-15
#if (EXTI_FALL | EXTI_RISE)
		static void _irqHandler(uint32_t pObj);
		FunctionPointer _pirq[EXTI_FALL + EXTI_RISE];
#endif
};

#endif
