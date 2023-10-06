/**
  ******************************************************************************
  * @file    rtc_calendar.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/17
  * @brief   rtc ����
	*     STM32F0RTC���̣�ע�⣬��F1ϵ�в�ͨ��
  *    1 ����rtc����
  * 	 2 ����begin()��ʼ��ʱ�ӡ�1 LSE  0 LSI
  *	   3 ����ʱ��
  *	   4 �������壬���������жϻص�����
	*		 5 ʱ�Ӵ�LSI��ΪLSEʱ����Ҫ�ϵ�����һ��LSE����Ч

	��֪bug��LSEʱ������Ч
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

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1 ��������ʾ���������ʱ�䣬���ڣ�����
  * 2 F0����RTC�����ṩ�����գ�ʱ��������㡣 F1��������һ���򵥶�ʱ���������գ�ʱ������ͨ��������ʽ�ṩ��
  *   ÿ�����ٵ�������getTime�������Ը�����������Ϣ
  * 3 setAlarm��Ҫ�ṩ��ʱʱ�������λ����������Ĭ���������ں����ڡ�F0������λ��Ч��F1����Ч
  * 4 setInterrupt���������ر��ж�,�ڶ�������Ϊ�ж����ͣ�F0����Ч��F1�п��Էֱ���Ӧ���壬�룬���������Ĭ
      ��Ϊ���塣attach�е�irq_typeͬ��Ĭ�����壬F0��Ч��F1��Ч
  * 5 setDateʱ��F0���������û�ָ����F1����������������õ��������в���
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32 RTC example"
#define EXAMPLE_DATE	"2018-11-04"

E_UART UART(UART1);

// ע��E_RTCû����ʾ���캯����Ĭ��Ϊ�޲ι��죬����rtc���治������
E_RTC rtc;

//void eboxClockConfig(void)
//{
//  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

//  LL_RCC_HSE_Enable();

//   /* Wait till HSE is ready */
//  while(LL_RCC_HSE_IsReady() != 1)
//  {
//    
//  }
//  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_6);
//  LL_RCC_PLL_Enable();

//   /* Wait till PLL is ready */
//  while(LL_RCC_PLL_IsReady() != 1)
//  {
//    
//  }
//  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
//  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
//  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
//  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
//}

// ÿ�������һ������
void exit()
{
    Time_T time;
    UART.printf("\n\r\n\r");
    UART.printf("#######################  ����  ##############################\n\r");
    rtc.getTime(&time);
    UART.printf("#####                 %2d:%02d:%02d                       ######", time.Hours, time.Minutes, time.Seconds);
    time.Minutes += 1;
    rtc.setAlarm(time);
    UART.printf("\r\n");
    UART.printf("#############################################################\n\r");
}

void setup()
{
    Date_T date = {0, 4, 11, 18};
    Time_T time = {0, 22, 56, 1};

    eboxInit();
    UART.begin(115200);

    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    // EOK,��ʼ���ɹ�������RTCʱ�������У�����Ҫ�������ڣ�ʱ�䡣������Ҫ����
    if (rtc.begin(clock_hse) != EOK);
//    {
        rtc.setDate(date);
        rtc.setTime(time);
//    }
    // ��������

    rtc.getTime(&time);
    time.Minutes += 1;
    time.Seconds += 0;
    rtc.setAlarm(time, eRtc_MaskDateWeekDay | eRtc_MaskHours);
    UART.printf("\n\r ����ʱ���趨Ϊ��%2d:%02d:%02d��", time.Hours, time.Minutes, time.Seconds);
    rtc.attach(&exit, Alr_Irq);
    rtc.setInterrupt(ENABLE);
}

uint8_t Week[7][3] = {{"һ"}, {"��"}, {"��"}, {"��"}, {"��"}, {"��"}, {"��"}};

void test(void)
{
    Time_T time;
    Date_T date;

    while (1)
    {
        // ÿ30s��ȡһ��ʱ��
        rtc.getTime(&time);
        rtc.getDate(&date);
        UART.printf("\n\r %2d:%02d:%02d��", time.Hours, time.Minutes, time.Seconds);
        UART.printf("\n\r 20%2d��%02d��%2d�� ����%02d", date.Year, date.Month, date.Day, date.WeekDay);
        delayMs(30000);
    }
}