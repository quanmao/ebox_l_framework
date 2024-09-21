/**
  ******************************************************************************
  * @file    ebox_nvic.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/15
  * @brief   此文件问nvic相关配置,此部分一般无需调用，在外设文件中已经调用处理
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#ifndef __NVIC_H
#define __NVIC_H

#include "mcu_config.h"
#include "mcu_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#define eboxNvicPriorityEncode NVIC_EncodePriority // 参数分别为riorityGroup,PreemptPriority,SubPriority。返回值可做为eboxNvicIrqSetPriority的参数
#define eboxNvicPriorityDecode NVIC_DecodePriority // 参数分别为Priority,PriorityGroup,uint32_t* const pPreemptPriority, uint32_t* const pSubPriority \
                                                   // 前两个输入，后两个返回
                                                   // 设置获取中断优先级
#define eboxNvicIrqSetPriority __NVIC_SetPriority  // 参数分别为IRQn,priority(该参数可通过eboxNvicPriorityEncode获取)
#define eboxNvicIrqGetPriority __NVIC_GetPriority  // 参数为IRQn，返回priority


// 根据外设中断号，开启关闭中断
#define eboxNvicIrqDisable __NVIC_DisableIRQ // 参数IRQn_Type irq_num，中断号
#define eboxNvicIrqEnable __NVIC_EnableIRQ   // 参数IRQn_Type irq_num，中断号

/**
 *@brief    根据设备地址和其对应的第几个中断索引，从中断表中查找中断号
 *@param    *table：中断表
            dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            index:  设备的第几个中断入口。最大支持4个，即0-3，一般设备只有一个，即可填0
 *@retval   中断号
*/
__STATIC_INLINE IRQn_Type eboxDevToIrq(const DevToIRQn_t *table, uint32_t dev, uint8_t index)
{
    uint8_t i = 0;

    while (table[i].dev != 0)
    {
        if (table[i].dev == dev)
        {
            return (IRQn_Type)table[i].irqn[index];
        }
        i++;
    }
    return IRQn_Type(0);
}

#ifdef __cplusplus
}
#endif
#endif
