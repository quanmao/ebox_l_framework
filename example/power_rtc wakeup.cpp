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

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 sleep,RTC wakeup example"
#define EXAMPLE_DATE	"2018-04-021"
#define DEMO_VER			"1.0"

// ���ڣ�led
E_GPIO led(PC13_ID);
//E_EXTI buttn(PC_13);
E_EXTI rx(&PA10);
// ע��E_RTCû����ʾ���캯����Ĭ��Ϊ�޲ι��죬����rtc���治������
E_RTC rtc;


void testirq()
{
  static uint8_t i = 0;
  led.toggle();
  UART.read();   // ���read�ж�
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
  // EOK,��ʼ���ɹ�������RTCʱ�������У�����Ҫ�������ڣ�ʱ�䡣������Ҫ����
  if (rtc.begin() != EOK)
  {
    rtc.setTime(time);
  }
  // ��������
  rtc.getTime(&time);
  time.Minutes += 1;
  time.Seconds += 0;
  
  rtc.setAlarm(time,eRtc_MaskDateWeekDay|eRtc_MaskHours);
  UART.printf("\n\r ����ÿ����һ�� ");
  rtc.attach(&testirq);
  rtc.setInterrupt(ENABLE);
}

void test(void)
{
  if (eboxIsWakeFromSB())
  {
    rtc.setInterrupt(DISABLE);
    UART.printf("\r\n��λ���ͣ�0x%x ,standbyģʽ����,60s�������ѭ�� \r\n", eboxGetResetType());
    delayMs(60000);
  }
  else
  {
    // ���ж�,��������MCU
    // UART.attach(&testirq,C_UartRxIrq);
    // UART.interruptEnable(C_UartRxIrq);

    UART.printf("\r\n��λ���ͣ�0x%x ,30s�����͹�����ʾ\r\n", eboxGetResetType());

    // sleep mode test
    UART.printf("\r\n sleep nowģʽ,�ȴ����廽��\r\n");
    eboxSleep(sleepNow, wfi);
    UART.printf("\r\n �Ѿ����ѣ��ر�����\r\n");
    // ��������
    rtc.getTime(&time);
    time.Minutes += 1;
    time.Seconds += 0;

    rtc.setAlarm(time, eRtc_MaskDateWeekDay | eRtc_MaskHours | eRtc_MaskSeconds);
    UART.printf("\r\n sleep_on_extiģʽ?5���жϺ���\r\n");
    eboxSleep(sleepOnExti);
    // stop mode test
    UART.printf("\r\n �Ѿ����ѣ�30�����stopģʽ\r\n");
    delayMs(30000);
    UART.printf("\r\n stopģʽ��user(�ⲿ�ж�)������\r\n");
    eboxStop(sleepNow);
    UART.printf("\r\n standbyģʽ��Ĭ��PA0����\r\n");
    eboxStandby();
  }
  UART.printf("\r\n ����������ѭ��,led��˸\r\n");
  while (1)
  {
    delayMs(500);
    led.toggle();
  }
}
