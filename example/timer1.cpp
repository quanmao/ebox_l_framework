/**
  ******************************************************************************
  * @file    .cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
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


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"
#include "bsp_ebox.h"

/**
  *	1	此例程演示了timer定时中断，使用tim1
  *	2	定时器频率为1hz，每秒中断1次
  * 3   表现为LED闪烁
  */

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Timer interrupt example"
#define EXAMPLE_DATE	"2018-08-08"

E_TIMER timer1(TIMER);

void t2it()
{
    LED1.toggle();
}
class timTest
{
public:
    void event()
    {
        LED1.toggle();
    }
};
timTest tst;

void setup()
{
    eboxInit();
    UART.begin(115200);
//    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    LED1.mode(OUTPUT_PP);

	timer1.setMs(50000);
//		timer1.setFrequency(5);
//    timer1.attach(t2it);
    timer1.attach(&tst, &timTest::event);
//    timer1.interrupt(ENABLE);
    timer1.start();

    UART.printf("\r\ntimer clock       = %dMhz", timer1.getSourceClock()/ 1000000);
    UART.printf("\r\nmax interrupt frq = %dKhz", timer1.getMaxFrequency() / 1000);
}


void test(void)
{
    setup();
    while(1)
    {

    }


}








