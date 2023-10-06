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

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 sleep example"
#define EXAMPLE_DATE	"2018-04-07"
#define DEMO_VER			"1.0"

// 串口，led
//E_UART UART(UART1,PA_9,PA_10);
E_GPIO led(PC13_ID);
//E_EXTI buttn(PC_13);
E_EXTI rx(&PA10);

void setup()
{
  eboxInit();
  led.mode(OUTPUT_PP);
  rx.begin(); // 注意，一定要先配置rx pin为外部中断，在初始化UART
  UART.begin(115200);

}

void testirq()
{
  static uint8_t i = 0;
  led.toggle();
  UART.read();   // 清空read中断
	UART.write(0x30+i++);
  if (i > 5){
    eboxExitSleepOnExti();
    i = 0;
  }
}

void test(void)
{
  UART.printf("ebox_base.cpp中debug定义为：1，可以输出更多调试信息\r\n");
  if (eboxIsWakeFromSB())
  {
    UART.printf("复位类型：0x%x ,standby模式唤醒,60s后进入主循环 \r\n", eboxGetResetType());
    /* Clear Standby flag */
    LL_PWR_ClearFlag_SB();
    delayMs(60000);
  }
  else
  {
    // 绑定中断,用来唤醒MCU
    UART.attach(&testirq, C_UartRxIrq);
    UART.interruptEnable(C_UartRxIrq);

    //    buttn.attach(&test,FALLING);
    //    buttn.ENABLE(FALLING);

    UART.printf("复位类型：0x%x ,30s后进入低功耗演示\r\n", eboxGetResetType());

    // sleep mode test
    delayMs(30000);
    UART.printf("sleep now模式,UART,user键(任意中断)唤醒,  ");
    eboxSleep(sleepNow);
    UART.printf("已经唤醒，30后进入sleep_on_exti模式\r\n");
    delayMs(30000);
    UART.printf("sleep_on_exti模式,UART,user键(任意中断)唤醒,唤醒后执行完中断任务继续进入休眠，5次中断后唤醒, ");
    eboxSleep(sleepOnExit);
    // stop mode test,注意，需要将调试线去掉，否则会自动退出stop mode.
    
    rx.enable(FALL_RISING);
    UART.printf("已经唤醒，30后进如stop模式\r\n");
    delayMs(30000);
  //  UART.begin(115200);
    UART.printf("stop模式，user(外部中断)键唤醒, ");
    eboxStop(sleepNow);
    rx.disable(FALL_RISING);
    UART.printf("已经唤醒，30后进如standby模式\r\n");
    delayMs(30000);
    UART.printf("standby模式，默认PA0唤醒, ");
    eboxStandby();
  }
  UART.printf("即将进入主循环,led闪烁\r\n");
  while (1)
  {
    delayMs(500);
    led.toggle();
  }
}
