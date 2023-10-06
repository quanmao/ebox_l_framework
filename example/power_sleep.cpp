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
#include "bsp_ebox.h"
#include "ebox_power.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 sleep example"
#define EXAMPLE_DATE	"2018-04-07"
#define DEMO_VER			"1.0"

// ���ڣ�led
//E_UART UART(UART1,PA_9,PA_10);
E_GPIO led(PC13_ID);
//E_EXTI buttn(PC_13);
E_EXTI rx(&PA10);

void setup()
{
  eboxInit();
  led.mode(OUTPUT_PP);
  rx.begin(); // ע�⣬һ��Ҫ������rx pinΪ�ⲿ�жϣ��ڳ�ʼ��UART
  UART.begin(115200);

}

void testirq()
{
  static uint8_t i = 0;
  led.toggle();
  UART.read();   // ���read�ж�
	UART.write(0x30+i++);
  if (i > 5){
    eboxExitSleepOnExti();
    i = 0;
  }
}

void test(void)
{
  UART.printf("ebox_base.cpp��debug����Ϊ��1������������������Ϣ\r\n");
  if (eboxIsWakeFromSB())
  {
    UART.printf("��λ���ͣ�0x%x ,standbyģʽ����,60s�������ѭ�� \r\n", eboxGetResetType());
    /* Clear Standby flag */
    LL_PWR_ClearFlag_SB();
    delayMs(60000);
  }
  else
  {
    // ���ж�,��������MCU
    UART.attach(&testirq, C_UartRxIrq);
    UART.interruptEnable(C_UartRxIrq);

    //    buttn.attach(&test,FALLING);
    //    buttn.ENABLE(FALLING);

    UART.printf("��λ���ͣ�0x%x ,30s�����͹�����ʾ\r\n", eboxGetResetType());

    // sleep mode test
    delayMs(30000);
    UART.printf("sleep nowģʽ,UART,user��(�����ж�)����,  ");
    eboxSleep(sleepNow);
    UART.printf("�Ѿ����ѣ�30�����sleep_on_extiģʽ\r\n");
    delayMs(30000);
    UART.printf("sleep_on_extiģʽ,UART,user��(�����ж�)����,���Ѻ�ִ�����ж���������������ߣ�5���жϺ���, ");
    eboxSleep(sleepOnExit);
    // stop mode test,ע�⣬��Ҫ��������ȥ����������Զ��˳�stop mode.
    
    rx.enable(FALL_RISING);
    UART.printf("�Ѿ����ѣ�30�����stopģʽ\r\n");
    delayMs(30000);
  //  UART.begin(115200);
    UART.printf("stopģʽ��user(�ⲿ�ж�)������, ");
    eboxStop(sleepNow);
    rx.disable(FALL_RISING);
    UART.printf("�Ѿ����ѣ�30�����standbyģʽ\r\n");
    delayMs(30000);
    UART.printf("standbyģʽ��Ĭ��PA0����, ");
    eboxStandby();
  }
  UART.printf("����������ѭ��,led��˸\r\n");
  while (1)
  {
    delayMs(500);
    led.toggle();
  }
}
