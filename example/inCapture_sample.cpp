/**
  ******************************************************************************
  * @file    *.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright 2015 shentq. All Rights Reserved.
  *
  * Copyright Notice
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq.
  *
  * @Disclaimer
  * This specification is preliminary and is subject to change at any time without
  * notice. shentq assumes no responsibility for any errors contained herein.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了输入捕获简单模式的使用方法。支持频率周期，不支持了占空比的采样
    *        输入捕获实验-测量周期和频率
    *   2   本例程为使用输入捕获模式测量一个PWM信号的周期和频率
    *   3   请将PA0和PB6使用跳线链接起来
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"InCapture simple example"
#define EXAMPLE_DATE	"2018-08-01"


E_INCAPTURE ic0(TIM3,&PA6);//创建一个输入捕获的对象
E_PWM pwm1(PWM);//创建一个PWM输出对象

//E_INCAPTURE ic0(PWM);//创建一个输入捕获的对象
//E_PWM pwm1(TIM3,&PA6);//创建一个PWM输出对象


uint32_t frq = 100000;

uint16_t p;
void setup()
{
    eboxInit();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

		ic0.begin(1); //初始化输入捕获参数，p分频
    pwm1.begin(frq, 500);
}
extern __IO uint32_t master_count[4] ;

void test(void)
{
	UART.printf("setFrq(%0.2f)", frq/100.0);
    while(1)
    {
        if(ic0.update_resault())
        {
						
            UART.printf("  peroid    = %0.2fus  ", GETREALFLOAT(ic0.res.peroid));
					  UART.printf(" = %0.2fhz  ", GETREALFLOAT(ic0.res.frq));
					UART.printf("highDuty = %0.2f%%  ", GETREALFLOAT(ic0.res.high_duty));
					UART.printf("lowDuty  = %0.2f%%\r\n", GETREALFLOAT(ic0.res.low_duty));
					pwm1.setFrequency(frq+=10000);
					UART.printf("setFrq(%0.2f)", frq/100.0);

        }
        delayMs(1000);
    }
}








