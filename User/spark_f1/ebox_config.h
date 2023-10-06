/**
  ******************************************************************************
  * @file    ebox_config.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox���òü��ļ�
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

#ifndef __EBOX_CONFIG_H
#define __EBOX_CONFIG_H

#define USE_PRINTF 1

#if  USE_PRINTF == 1				
#include <stdio.h>
#include <stdarg.h>
#define ebox_vsnprintf(...)    vsnprintf(__VA_ARGS__)
#define ebox_snprintf(...)     snprintf(__VA_ARGS__)
#define ebox_vsprintf(...)     vsprintf(__VA_ARGS__)
#define ebox_sprintf(...)      sprintf(__VA_ARGS__)

#elif  USE_PRINTF == 2
#include "Myprintf.h"
#define ebox_vsnprintf(...)    _ebox_vsnprintf(__VA_ARGS__)
#define ebox_snprintf(...)     _ebox_snprintf(__VA_ARGS__)
#define ebox_vsprintf(...)     _ebox_vsprintf(__VA_ARGS__)
#define ebox_sprintf(...)      _ebox_sprintf(__VA_ARGS__)
#endif

#if USE_PRINTF
//�Ƿ�ʹ��DEBUG, 1 ʹ��  0 ��ʹ��
#define EBOX_DEBUG  1
#endif

#if EBOX_DEBUG
#define DBG_UART   	uart1
#define DBG(...)    DBG_UART.printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

//spi�Ƿ�ʹ��9-15bit�ֽ����շ�����,Ĭ��ʹ��8bit
#define USE_SPI_WORD 0
#if USE_SPI_WORD
  #define DATA_T uint16_t
#else
  #define DATA_T uint8_t
#endif

/* �Ƿ�ʹ��IRQ��������Ӱ�����������ж�(systick�жϳ���)
 * 0 ��ʹ��IRQ
 * 1 ʹ��IRQ
 */
#define USE_IRQ	1

#if	USE_IRQ
	#define USE_UART_IRQ	1		// �Ƿ�ʹ��UART_IRQ
	#define USE_EXTI			1		// �Ƿ�ʹ��EXTI
#endif

/* ����ʹ����ЩTIM��Ĭ�Ͽ���TIM1-TIM5���رղ�ʹ�õ�TIM�����Լ����ڴ�ռ��
 * 0 ��ʹ��
 * 1 ʹ��
 */
#define USE_TIM1		1
#define USE_TIM2		1
#define USE_TIM3		1
#define USE_TIM4		1
#define USE_TIM5		1

#if (USE_TIM1 | USE_TIM2 | USE_TIM3 | USE_TIM4 | USE_TIM5)
	#define USE_UPDATE 1
	#define USE_CH1		1
	#define USE_CH2		1
	#define	USE_CH3		1
	#define USE_CH4		1
#endif

/* ����ʹ����ЩUART��Ĭ�Ͽ���UART1���رղ�ʹ�õ�UART�����Լ����ڴ�ռ��
 * 0 ��ʹ��
 * 1 ʹ��
 */
#define USE_UART1 1
#define USE_UART2 0
#define USE_UART3 0
#define USE_UART4 0
#define USE_UART5 0

/* flashд��������ʱ���Ƿ�������ǵ�ǰ�������������;
 * 0 д�������ݣ���������ǰ��������λ�þ����ݡ� ��Ҫռ��FLASH_PAGE_SIZE��С���ڴ���Ϊ������
 * 1 ֻ������д������ݡ� ��ʡ�ڴ�
 */
#define FLASH_OVERRIDE  1

/* �Ƿ�ʹ��object��������Ӱ��object.cpp & ebox.h
 * 0 ��ʹ��object���û���Ҫ������Ҫʵ������,������bsp_ebox��ʵ��
 * 1 ʹ��Ĭ��ʵ��
 */
#define	USE_OBJECT			0

/* �Ƿ�ʹ��SW�ӿڣ���ֹ�û���ر�
 * 0 ���Խ�SW�˿�����Ϊ��ͨIO��
 * 1 ��ֹ��SW�˿�����Ϊ��ͨIO��
 */
#define ENABLE_USESWD		1

#if ENABLE_USESWD
	#ifndef SWD_PORT				//  PA13,PA14
		#define SWD_PORT GPIOA
		#define SWD_PIN	(1<<13 | 1<< 14) 
	#endif
#endif

#endif

