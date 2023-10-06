/**
  ******************************************************************************
  * @file    ebox_exti.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/16
  * @brief   EXTI操作,默认支持上升沿，下降沿触发，16个通道。
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

// 配置是是否使用下降沿，上升沿中断，1 使用，0 不使用
#if USE_EXTI
#define EXTI_FALL 1
#define EXTI_RISE 1
#endif
// 配置使用的最大中断号,比如使用PA15作为外不准段，则max_exti = 16,和使用了多少中断无关，减少有利于降低内存
#define MAX_EXTI  16
/*
	1.提供一个io中断
	*注意：stm32一个中断线EXTI_Linex只能连接到一个port的GPIO_Pin_x，即设置PA0为中断源之后就不能设置PB0，PC0等为中断源
    trigger: 引脚触发中的条件，可以是一下三种模式中的一种：
        - RISING: 上升沿触发中断
        - FALLING: 下降沿触发中断
        - CHANGE: 上升沿和下降沿均触发中断
 */

// 触发类型
enum TrigType
{
    FALL = 0,	// 下降沿触发
    RISE,			// 上升沿触发
    FALL_RISING		// 上升沿下降沿
};

class E_EXTI
{
public:
    E_EXTI(E_GPIO *exti_pin);
    void begin(PinMode_t mode = INPUT );

		// 开启/关闭中断
    void enable(TrigType trig, uint32_t priority = 0);
    void disable(TrigType trig);
#if (EXTI_FALL | EXTI_RISE)
		// 绑定中断回调函数
    void attach(void (*fptr)(void), TrigType type);
		// 绑定类成员函数作为回调函数，类名::函数名
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), TrigType type);
#endif
private:

    E_GPIO	*_pin;
    uint16_t	_extiLine;	//外部中断0-15
#if (EXTI_FALL | EXTI_RISE)
		static void _irqHandler(uint32_t pObj);
		FunctionPointer _pirq[EXTI_FALL + EXTI_RISE];
#endif
};

#endif
