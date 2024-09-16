/**
  ******************************************************************************
  * @file    bsp_ebox.h
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
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
#ifndef __BSP_EBOX_H
#define __BSP_EBOX_H
#include "ebox.h"

#define	HARDWARE	"stm32nucleo，STM32F030F4"
#define F0

#if USE_OBJECT == 0
extern E_GPIO PA0;
extern E_GPIO PA1;
extern E_GPIO PA2;
extern E_GPIO PA3;
extern E_GPIO PA4;
extern E_GPIO PA5;
extern E_GPIO PA6;
extern E_GPIO PA7;
extern E_GPIO PA8;
extern E_GPIO PA9;
extern E_GPIO PA10;

extern E_GPIO PA13;
extern E_GPIO PA14;

extern E_GPIO PB0;
extern E_GPIO PB1;
extern E_GPIO PB3;
extern E_GPIO PB4;
extern E_GPIO PB5;
extern E_GPIO PB9;

extern E_GPIO PF0;
extern E_GPIO PF1;


extern E_UART uart1;
extern E_EXTI BTN;
#endif

//#define LED PA5
#define LED PB0
#define LED1 PA5
#define LED2 PA5
#define UART uart1
#define PWM	 TIM1,&PA8
#define TIMER TIM3

#define UART1 USART1,&PA9,&PA10
//#define UART1 USART1,&PA2,&PA3

#define I22C I2C1,&PA9,&PA10
//#define I2CS &PA9,&PA10

#define HSPI SPI1,&PA5,&PA6,&PA7
#define SSPI &PA5,&PA6,&PA7
//#define SSPI &PB3,&PB4,&PB5

void print_log(const char *name, const char *date);




#endif

