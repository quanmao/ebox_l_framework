/*
file   : gtimer.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __TIMER_IT_H
#define __TIMER_IT_H

#include "ebox_config.h"
#include "mcu_define.h"

enum TIM_Index{
#if USE_TIM1
	tim1,
#endif
#if (USE_TIM2 && defined TIM2_BASE)
	tim2,
#endif
#if (USE_TIM3 && defined TIM3_BASE)
	tim3,
#endif
#if (USE_TIM4 && defined TIM4_BASE)
	tim4,
#endif
#if (USE_TIM5 && defined TIM5_BASE)
	tim5,
#endif
#if (USE_TIM16 && defined TIM16_BASE)
	tim16,
#endif
#if (USE_TIM17 && defined TIM17_BASE)
	tim17,
#endif
	tims    // 使用的TIM数量
};

enum IT_Index{
#if (USE_UPDATE)
	update,     // 更新中断
#endif
#if (USE_CH1)
	cc1,        // 捕获，比较中断
#endif
#if (USE_CH2)
	cc2,        // 捕获，比较中断
#endif
#if (USE_CH3)
	cc3,        // 捕获，比较中断
#endif
#if (USE_CH4)
	cc4,        // 捕获，比较中断
#endif
	its // 每个TIM使用的中断数量
};

//enum Tim_It_Index
//{
//    TIM1_IT_Update  = 0,
//    TIM2_IT_Update  = 1,
//    TIM3_IT_Update  = 2,
//    TIM4_IT_Update  = 3,
//    TIM5_IT_Update  = 4,
//    TIM6_IT_Update  = 5,
//    TIM7_IT_Update  = 6,
//    TIM1_IT_CC1     = 7,
//    TIM1_IT_CC2     = 8,
//    TIM1_IT_CC3     = 9,
//    TIM1_IT_CC4     = 10,
//    TIM2_IT_CC1     = 11,
//    TIM2_IT_CC2     = 12,
//    TIM2_IT_CC3     = 13,
//    TIM2_IT_CC4     = 14,
//    TIM3_IT_CC1     = 15,
//    TIM3_IT_CC2     = 16,
//    TIM3_IT_CC3     = 17,
//    TIM3_IT_CC4     = 18,
//    TIM4_IT_CC1     = 19,
//    TIM4_IT_CC2     = 20,
//    TIM4_IT_CC3     = 21,
//    TIM4_IT_CC4     = 22,
//    TIM5_IT_CC1     = 23,
//    TIM5_IT_CC2     = 24,
//    TIM5_IT_CC3     = 25,
//    TIM5_IT_CC4     = 26,
//} ;

//#define TIM_IT_CC_OFFSET    6
#define TIM_IRQ_ID_NUM      tims*its

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*irqHandler)(uint32_t pObj);
void timIrqAdd(uint8_t index, irqHandler handler, uint32_t id);

//void tim_irq_callback(uint8_t index);

#ifdef __cplusplus
}
#endif

#endif
