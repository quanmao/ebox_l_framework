/**
  ******************************************************************************
  * @file    ebox_nvic.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/15
  * @brief   ���ļ���nvic�������,�˲���һ��������ã��������ļ����Ѿ����ô���
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

#define eboxNvicPriorityEncode NVIC_EncodePriority // �����ֱ�ΪriorityGroup,PreemptPriority,SubPriority������ֵ����ΪeboxNvicIrqSetPriority�Ĳ���
#define eboxNvicPriorityDecode NVIC_DecodePriority // �����ֱ�ΪPriority,PriorityGroup,uint32_t* const pPreemptPriority, uint32_t* const pSubPriority \
                                                   // ǰ�������룬����������
                                                   // ���û�ȡ�ж����ȼ�
#define eboxNvicIrqSetPriority __NVIC_SetPriority  // �����ֱ�ΪIRQn,priority(�ò�����ͨ��eboxNvicPriorityEncode��ȡ)
#define eboxNvicIrqGetPriority __NVIC_GetPriority  // ����ΪIRQn������priority


// ���������жϺţ������ر��ж�
#define eboxNvicIrqDisable __NVIC_DisableIRQ // ����IRQn_Type irq_num���жϺ�
#define eboxNvicIrqEnable __NVIC_EnableIRQ   // ����IRQn_Type irq_num���жϺ�

/**
 *@brief    �����豸��ַ�����Ӧ�ĵڼ����ж����������жϱ��в����жϺ�
 *@param    *table���жϱ�
            dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            index:  �豸�ĵڼ����ж���ڡ����֧��4������0-3��һ���豸ֻ��һ����������0
 *@retval   �жϺ�
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
