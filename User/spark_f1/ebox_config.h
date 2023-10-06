/**
  ******************************************************************************
  * @file    ebox_config.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox配置裁剪文件
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
//是否使用DEBUG, 1 使用  0 不使用
#define EBOX_DEBUG  1
#endif

#if EBOX_DEBUG
#define DBG_UART   	uart1
#define DBG(...)    DBG_UART.printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

//spi是否使用9-15bit字节来收发数据,默认使用8bit
#define USE_SPI_WORD 0
#if USE_SPI_WORD
  #define DATA_T uint16_t
#else
  #define DATA_T uint8_t
#endif

/* 是否使用IRQ，该配置影响所有外设中断(systick中断除外)
 * 0 不使用IRQ
 * 1 使用IRQ
 */
#define USE_IRQ	1

#if	USE_IRQ
	#define USE_UART_IRQ	1		// 是否使用UART_IRQ
	#define USE_EXTI			1		// 是否使用EXTI
#endif

/* 配置使用哪些TIM，默认开启TIM1-TIM5，关闭不使用的TIM，可以减少内存占用
 * 0 不使用
 * 1 使用
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

/* 配置使用哪些UART，默认开启UART1，关闭不使用的UART，可以减少内存占用
 * 0 不使用
 * 1 使用
 */
#define USE_UART1 1
#define USE_UART2 0
#define USE_UART3 0
#define USE_UART4 0
#define USE_UART5 0

/* flash写入新数据时，是否擦除覆盖当前区块的其它数据;
 * 0 写入新数据，并保留当前区块其他位置旧数据。 需要占用FLASH_PAGE_SIZE大小的内存作为缓冲区
 * 1 只保留新写入的数据。 节省内存
 */
#define FLASH_OVERRIDE  1

/* 是否使用object，该配置影响object.cpp & ebox.h
 * 0 不使用object，用户需要根据需要实例对象,建议在bsp_ebox中实例
 * 1 使用默认实例
 */
#define	USE_OBJECT			0

/* 是否使用SW接口，防止用户误关闭
 * 0 可以将SW端口设置为普通IO口
 * 1 禁止将SW端口设置为普通IO口
 */
#define ENABLE_USESWD		1

#if ENABLE_USESWD
	#ifndef SWD_PORT				//  PA13,PA14
		#define SWD_PORT GPIOA
		#define SWD_PIN	(1<<13 | 1<< 14) 
	#endif
#endif

#endif

