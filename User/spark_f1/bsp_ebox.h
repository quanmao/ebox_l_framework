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

#define	HARDWARE	"ebox_spark，STM32F103C8T6"

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
extern E_GPIO PA15;

extern E_GPIO PB0;
extern E_GPIO PB1;
extern E_GPIO PB2;
extern E_GPIO PB3;
extern E_GPIO PB4;
extern E_GPIO PB5;
extern E_GPIO PB6;
extern E_GPIO PB7;
extern E_GPIO PB8;
extern E_GPIO PB9;
extern E_GPIO PB10;
extern E_GPIO PB11;
extern E_GPIO PB12;
extern E_GPIO PB13;
extern E_GPIO PB14;
extern E_GPIO PB15;


extern E_GPIO PF0;
extern E_GPIO PF1;
extern E_GPIO PE2;


extern E_UART uart1;
extern E_EXTI BTN;

#endif
#define UART uart1
#define LED PB8
#define LED1 PE2
#define LED3 PB10
#define PWM TIM4, &PB8

#define UART1 USART1,&PA9,&PA10
#define UART2 USART2,&PA2,&PA3

#define I22C I2C1,&PB6,&PB7
#define I2CS &PB6,&PB7

#define HSPI SPI1,&PA5,&PA6,&PA7
#define SSPI &PA5,&PA6,&PA7

#define HSPI2 SPI2,&PB13,&PB14,&PB15
#define SSPI2 &PB13,&PB14,&PB15

extern E_SPI  spi1;
extern void print_log(const char *name = "", const char *date = "");

#endif

