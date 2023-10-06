/**
  ******************************************************************************
  * @file    example.cpp
  * @author  cat_li
  * @brief   ebox application example .
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
#include "ebox_power.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 sleep,RTC wakeup example"
#define EXAMPLE_DATE	"2018-04-021"
#define DEMO_VER			"1.0"

// 串口，led
E_GPIO led(PC13_ID);
//E_EXTI buttn(PC_13);
E_EXTI rx(&PA10);
// 注意E_RTC没有显示构造函数，默认为无参构造，所以rtc后面不跟括号
E_RTC rtc;


void testirq()
{
  static uint8_t i = 0;
  led.toggle();
  UART.read();   // 清空read中断
	UART.write(0x30+i++);
  if (i >= 5){
    eboxExitSleepOnExti();
    i = 0;
  }
}
Time_T time = {LL_RTC_TIME_FORMAT_AM_OR_24, 22, 56, 1};
void setup()
{


  eboxInit();
  led.mode(OUTPUT_PP);
  UART.begin(115200);
  UART.printf("rtc wakeup example");
  // EOK,初始化成功，并且RTC时间在运行，不需要设置日期，时间。否则需要设置
  if (rtc.begin() != EOK)
  {
    rtc.setTime(time);
  }
  // 设置闹铃
  rtc.getTime(&time);
  time.Minutes += 1;
  time.Seconds += 0;
  
  rtc.setAlarm(time,eRtc_MaskDateWeekDay|eRtc_MaskHours);
  UART.printf("\n\r 闹铃每分钟一次 ");
  rtc.attach(&testirq);
  rtc.setInterrupt(ENABLE);
}

void test(void)
{
  if (eboxIsWakeFromSB())
  {
    rtc.setInterrupt(DISABLE);
    UART.printf("\r\n复位类型：0x%x ,standby模式唤醒,60s后进入主循环 \r\n", eboxGetResetType());
    delayMs(60000);
  }
  else
  {
    // 绑定中断,用来唤醒MCU
    // UART.attach(&testirq,C_UartRxIrq);
    // UART.interruptEnable(C_UartRxIrq);

    UART.printf("\r\n复位类型：0x%x ,30s后进入低功耗演示\r\n", eboxGetResetType());

    // sleep mode test
    UART.printf("\r\n sleep now模式,等待闹铃唤醒\r\n");
    eboxSleep(sleepNow, wfi);
    UART.printf("\r\n 已经唤醒，关闭闹铃\r\n");
    // 设置闹铃
    rtc.getTime(&time);
    time.Minutes += 1;
    time.Seconds += 0;

    rtc.setAlarm(time, eRtc_MaskDateWeekDay | eRtc_MaskHours | eRtc_MaskSeconds);
    UART.printf("\r\n sleep_on_exti模式?5次中断后唤醒\r\n");
    eboxSleep(sleepOnExti);
    // stop mode test
    UART.printf("\r\n 已经唤醒，30后进如stop模式\r\n");
    delayMs(30000);
    UART.printf("\r\n stop模式，user(外部中断)键唤醒\r\n");
    eboxStop(sleepNow);
    UART.printf("\r\n standby模式，默认PA0唤醒\r\n");
    eboxStandby();
  }
  UART.printf("\r\n 即将进入主循环,led闪烁\r\n");
  while (1)
  {
    delayMs(500);
    led.toggle();
  }
}
