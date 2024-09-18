/**
  ******************************************************************************
  * @file    incapture.h
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
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *�Ż��ڲ��ṹ������Ƶ���жϴ����Ķ�����㿪��
 *      *���������˲��λ����������ߣ����û��Ĳ��β������
 */

/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __IN_CAPTURE_H
#define __IN_CAPTURE_H

//#include "FunctionPointer.h"
#include "ebox_timer.h"
#include "ebox_gpio.h"

#define TIM_ICPOLARITY_FALLING LL_TIM_IC_POLARITY_FALLING
#define TIM_ICPOLARITY_RISING  LL_TIM_IC_POLARITY_RISING
/*
1.֧��TIM2��3��4��ch1,2,3,4.����12��ͨ��
2.֧�ֲ������ڡ�Ƶ�ʡ��߼��÷�֧�ֲ���ռ�ձ�
3.Ĭ�ϲ����½��أ���ͨ���������ò�������ģʽ
4.��ʱ�����������ֵΪ0xffff,Ϊ��ͻ��������ƣ�
    �ڱ������У�ʹ����update����жϵ���tx_overflow_times���Խ�������
    ��չ��2^32�������߲�����Χ������ʵ�����Ƶ�ʣ�1��Ƶ���������ڵ���120s���źš�
    ���ʹ��2��Ƶ���ɲ������ڵ���240s���źš��Դ����ơ�
5.���ڷ�Ƶϵ���������Ȳ����ļ����ϵ��Ҫ��ѭһ��ԭ���ڲ����������¾���ʹ�õͷ�Ƶϵ������TIMʱ�ӣ�ȥ������
6.������խ����Ĳ���
   ����STM32������ٶ����ƣ��޷������ǳ�խ���������ʹ��Ĭ�Ϸ�Ƶϵ����1������խ����Ϊ4us��
   ��խ������PWM��ռ�ձȺ�Ƶ�������������ơ�


7.���ʹ��ĳ����ʱ��ͨ���������벶����ö�ʱ������ͨ�������������벶��ģʽ������������Ϊ��������ģʽ
�ص㣺
    �ڲ��õͷ�Ƶϵ����ʱ�򣬿��Ա�֤�������ȣ����ǻ�����ʱ�����Ƶ�ʣ���������cpu������
    �ڲ��ø߷�Ƶϵ����ʱ�򣬲������Ƚϵͣ����ǻή�Ͷ�ʱ�����Ƶ�ʣ���������cpu������
    stm32��72M��Ƶ�£���߿ɲ�160Khz���źš�����ٴ󣬽��޷�������


*/

class E_INCAPTURE : T_BASE
{
    public:
        typedef struct
        {
            uint32_t frq;          ///<���ε�Ƶ��
            uint32_t peroid;       ///<���ε�����
            uint32_t high_duty;    ///<���εĸߵ�ƽռ�ձ�
            uint32_t low_duty;     ///<���εĵ͵�ƽռ�ձ�
            uint32_t high_time;    ///<���εĸߵ�ƽʱ��
            uint32_t low_time;     ///<���εĵ͵�ƽʱ��
        }ICResault_t;
        
    typedef enum
    {
        SIMPLE = 0,
        COMPLEX = 1
    } ICMode_t;

    typedef enum
    {
        Rising = LL_TIM_IC_POLARITY_RISING,
        Falling = LL_TIM_IC_POLARITY_FALLING
    } ICPolarity_t;
public:
    
	E_INCAPTURE(TIM_TypeDef *TIMx, E_GPIO *pin);

    void SetPolarity(ICPolarity_t porlicy);
    void begin(uint16_t prescaler = 1, ICMode_t mode = SIMPLE); //ʹ��Ĭ�ϲ�������Ƶϵ��Ϊ1���������Ϊ120s

    //���εĻ����Ĳ�������
    void complex_event();       //������Ҫ�����ռ�ձȵ������������������ܵ���4us****
    void simple_event();        //���������������ִ��Ч�ʸߣ���߿ɲ�180K,���ǲ��ܲ���ռ�ձ�

uint32_t get_timer_clock();

    bool update_resault();
    void attach(void (*fptr)(void))
    {
        _pirq.attach(fptr);
    }
    template <typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _pirq.attach(tptr, mptr);
    }

    ICResault_t res;
private:
    uint8_t _chIndex;
    uint16_t _duty;        // ռ�ձ�
    uint8_t _accuracy;     // ����
    __IO uint8_t _polarity; // ����

    uint32_t *_overflow_times;
    __IO uint32_t _lastValue;   //���ֵ
    __IO uint32_t _capture;      //����ֵ
    __IO bool _available;        //�Ƿ���Ч
    __IO uint32_t _highCapture; //�ߵ�ƽ����
    __IO uint32_t _lowCapture;  //�͵�ƽ����

    uint32_t _timeClock; //timeʱ��
    uint16_t *ccr;
    void _setMode(void);

    FunctionPointer _pirq;
    static void _irqHandler(uint32_t id);
};

#endif
