/**
  ******************************************************************************
  * @file    ebox_exti.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/16
  * @brief   EXTI操作,默认支持上升沿，下降沿触发，16个通道。
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_ll_system.h"
#include "ebox_gpio.h"
#include "ebox_nvic.h"
#include "ebox_exti.h"

// pin.id 为8bit，高4位标识port 0-5,低4位为io,0-15
#define GETEXTIPORT(a) ((uint32_t)a >>4 )
#define GETEXTILINE(a) ((uint32_t)1<<( a & 0x0f ))
#define GETPINNUMBER(a)(a & 0x0f)


// uint32_t pObj 类对象

typedef void (*irqHandler)(uint32_t pObj);
#if (EXTI_FALL | EXTI_RISE)
static irqHandler 	pIrqHandler; 						// 声明函数指针变量，指向类的静态成员
static uint32_t 		irqIds[MAX_EXTI];     	// 保存对象地址，供静态成员识别对象，并访问对象的普通成员

void extiIrqAdd(uint8_t index, irqHandler handler, uint32_t id)
{
    irqIds[index] = id; // 保存对象地址
    pIrqHandler = handler;   // 指向回调函数
}

const uint32_t EXTISorce_MAP[] =
    {
        LL_SYSCFG_EXTI_LINE0, LL_SYSCFG_EXTI_LINE1, LL_SYSCFG_EXTI_LINE2, LL_SYSCFG_EXTI_LINE3,
        LL_SYSCFG_EXTI_LINE4, LL_SYSCFG_EXTI_LINE5, LL_SYSCFG_EXTI_LINE6, LL_SYSCFG_EXTI_LINE7,
        LL_SYSCFG_EXTI_LINE8, LL_SYSCFG_EXTI_LINE9, LL_SYSCFG_EXTI_LINE10, LL_SYSCFG_EXTI_LINE11,
        LL_SYSCFG_EXTI_LINE12, LL_SYSCFG_EXTI_LINE13, LL_SYSCFG_EXTI_LINE14, LL_SYSCFG_EXTI_LINE15
    };
#endif

/**
 * @brief    Exti构造函数，实例化一个对象
 * @param    *pin: 指定的外部中断引脚PA0~PG15
 * @return   NONE
 */
E_EXTI::E_EXTI(E_GPIO *pin)
{
    _pin = pin;
    _extiLine = GETEXTILINE(_pin->id);
}
/**
 * @brief    Exti构造函数，实例化一个对象
 * @param    mode: gpio模式，type 中断类型，IT，EVENT,IT_EVENT。默认为中断
 * @return   NONE
 */
void E_EXTI::begin(PinMode_t mode)
{
    _pin->mode(mode);
#if (EXTI_FALL | EXTI_RISE)
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
    LL_SYSCFG_SetEXTISource(GETEXTIPORT(_pin->id), EXTISorce_MAP[GETPINNUMBER(_pin->id)]);
    extiIrqAdd(GETPINNUMBER(_pin->id), (&E_EXTI::_irqHandler), (uint32_t)this);
#endif
    LL_EXTI_EnableIT_0_31(_extiLine);
    LL_EXTI_DisableEvent_0_31(_extiLine);
}

/**
  *@brief    使能中断
  *@param    trig 触发类型
  *@retval   NONE
  */
void E_EXTI::enable(TrigType trig, uint32_t priority)
{
#if (EXTI_FALL & EXTI_RISE)
	switch (trig) // 使能触发类型
    {
    case FALL:
        LL_EXTI_EnableFallingTrig_0_31(_extiLine);
        LL_EXTI_DisableRisingTrig_0_31(_extiLine);
        break;
    case RISE:
        LL_EXTI_EnableRisingTrig_0_31(_extiLine);
        LL_EXTI_DisableFallingTrig_0_31(_extiLine);
        break;
    case FALL_RISING:
        LL_EXTI_EnableFallingTrig_0_31(_extiLine);
        LL_EXTI_EnableRisingTrig_0_31(_extiLine);
        break;
    }
#else
#if(EXTI_RISE)
		LL_EXTI_EnableRisingTrig_0_31(_extiLine);
		LL_EXTI_DisableFallingTrig_0_31(_extiLine);
#else
		LL_EXTI_EnableFallingTrig_0_31(_extiLine);
		LL_EXTI_DisableRisingTrig_0_31(_extiLine);
#endif
#endif
    switch (GETPINNUMBER(_pin->id)) // 使能对应中断
    {
    case 0:
    case 1:
				eboxNvicIrqSetPriority(EXTI0_1_IRQn, priority);
        eboxNvicIrqEnable(EXTI0_1_IRQn);
        break;
    case 2:
    case 3:
				eboxNvicIrqSetPriority(EXTI2_3_IRQn, priority);
        eboxNvicIrqEnable(EXTI2_3_IRQn);
        break;
    default:
				eboxNvicIrqSetPriority(EXTI4_15_IRQn, priority);
        eboxNvicIrqEnable(EXTI4_15_IRQn);		
        break;
    }
}

/**
  *@brief    禁止触发
  *@param    trig 触发类型
  *@retval   NONE
  */
void E_EXTI::disable(TrigType trig)
{
#if (EXTI_FALL & EXTI_RISE)
	switch (trig) // 使能触发类型
    {
    case FALL:
        LL_EXTI_DisableFallingTrig_0_31(_extiLine);
        break;
    case RISE:
        LL_EXTI_DisableRisingTrig_0_31(_extiLine);
        break;
    case FALL_RISING:
        LL_EXTI_DisableRisingTrig_0_31(_extiLine);
        LL_EXTI_DisableFallingTrig_0_31(_extiLine);
        break;
    }
#else
#if(EXTI_RISE)
        LL_EXTI_DisableRisingTrig_0_31(_extiLine);
#else
        LL_EXTI_DisableFallingTrig_0_31(_extiLine);
#endif
#endif
}

#if (EXTI_FALL | EXTI_RISE)

/**
 *@brief    E_EXTI 静态成员函数，在中断中调用，解析执行相关回调函数
 *@param    uint32_t id 对象地址，用来识别对象；IrqType irq_type 中断类型
 *@retval   NONE
*/
void E_EXTI::_irqHandler(uint32_t pObj)
{
    E_EXTI *handler = (E_EXTI *)pObj; // 指向回调函数地址
#if (EXTI_FALL + EXTI_RISE) == 1
    handler->_pirq[0].call();
#else
    handler->_pirq[handler->_pin->read()].call();
#endif
}
/**
 * @brief   绑定中断的回调函数，产生中断后程序将执行callback_fun()
 * @param   void (*fptr)(void)类型函数的指针。
 *
 * @return  NONE
 */
void E_EXTI::attach(void (*fptr)(void), TrigType type)
{
#if (EXTI_FALL + EXTI_RISE) == 2
    if (type == FALL_RISING)
    {
        _pirq[FALL].attach(fptr);
        _pirq[RISE].attach(fptr);
    }
    else
    {
        _pirq[type].attach(fptr);
    }
#else
    _pirq[FALL].attach(fptr);
#endif
}
template <typename T>
void E_EXTI::attach(T *tptr, void (T::*mptr)(void), TrigType type)
{

#if (EXTI_FALL + EXTI_RISE) == 2
    if (type == FALL_RISING)
    {
        _pirq[FALL].attach(tptr, mptr);
        _pirq[RISE].attach(tptr, mptr);
    }
    else
    {
        _pirq[type].attach(tptr, mptr);
    }
#else
    _pirq[FALL].attach(tptr, mptr);
#endif
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#if (EXTI_FALL | EXTI_RISE)
    void EXTI4_15_IRQHandler(void)
    {
        for (uint8_t i = 4; i <= 15; i++)
        {
            if (LL_EXTI_IsActiveFlag_0_31(1 << i) != RESET)
            {
                pIrqHandler(irqIds[i]);
                LL_EXTI_ClearFlag_0_31(1 << i);
            }
        }
    }

    void EXTI0_1_IRQHandler(void)
    {
        if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
        {
            LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
            pIrqHandler(irqIds[0]);
        }
        else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
        {
            LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
            pIrqHandler(irqIds[1]);
        }
    }

    void EXTI2_3_IRQHandler(void)
    {
        if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
        {
            LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
            pIrqHandler(irqIds[2]);
        }
        else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
        {
            LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
            pIrqHandler(irqIds[3]);
        }
    }
#endif
#ifdef __cplusplus
}
#endif
