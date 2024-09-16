/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   Ӳ�������Ϣ����
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
#include "bsp_ebox.h"

//// �����ʹ��object.cpp, ʹ�����з�ʽ����IO����
#if USE_OBJECT == 0
// �����ʹ��object.cpp, ʹ�����з�ʽ����IO����
E_GPIO PA0(PA0_ID);
E_GPIO PA1(PA1_ID);
E_GPIO PA2(PA2_ID);
E_GPIO PA3(PA3_ID);
E_GPIO PA4(PA4_ID);
E_GPIO PA5(PA5_ID);
E_GPIO PA6(PA6_ID);
E_GPIO PA7(PA7_ID);
E_GPIO PA8(PA8_ID);
E_GPIO PA9(PA9_ID);
E_GPIO PA10(PA10_ID);

E_GPIO PA13(PA13_ID);
E_GPIO PA14(PA14_ID);

E_GPIO PB0(PB0_ID);
E_GPIO PB1(PB1_ID);

E_GPIO PB7(PB7_ID);
E_GPIO PB9(PB9_ID);
E_GPIO PB11(PB11_ID);

E_GPIO PF0(PF0_ID);
E_GPIO PF1(PF1_ID);

E_GPIO PB3(PB3_ID);
E_GPIO PB4(PB4_ID);
E_GPIO PB5(PB5_ID);

E_UART uart1(USART1, &PA9, &PA10);

//E_EXTI BTN(&PA4);
E_EXTI BTN(&PB11);
#endif

/*
*********************************************************************************************************
*	�� �� ��: PrintfLog
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void print_log(const char *name, const char *date)
{
//    UART.printf("\n\r");
//    UART.printf("*************************************************************\n\r");
//    UART.printf("* \r\n");	/* ��ӡһ�пո� */
//    UART.printf("* ��������      : %s\r\n", name);	/* ��ӡ�������� */
//    UART.printf("* ��������      : %s\r\n", date);	/* ��ӡ�������� */

//    UART.printf("* Ӳ��ƽ̨      : %s \r\n", HARDWARE);
//    UART.printf("* EBOX��汾    : %s (ebox)\r\n", EBOX_VERSION);
//    UART.printf("* \r\n");	/* ��ӡһ�пո� */
//    UART.printf("*                     CPU ��Ϣ\r\n");	/* ��ӡһ�пո� */
//    UART.printf("* \r\n");	/* ��ӡһ�пո� */
//    UART.printf("* CPU TYPE      : %d\r\n", MCU_TYPE);	/* ��ӡһ�пո� */
//    UART.printf("* CPU PINS      : %d\r\n", STM32_PINS);	/* ��ӡһ�пո� */
//    UART.printf("* CPU COMPANY   : %s\r\n", MCU_COMPANY);	/* ��ӡһ�пո� */
//    UART.printf("* CPUID         : %08X %08X %08X\n\r"
//                , cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
//    UART.printf("* core          : %0.3fMhz\r\n", cpu.clock.core / 1000000.0);
//    UART.printf("* hclk          : %0.3fMhz\r\n", cpu.clock.hclk / 1000000.0);
//    UART.printf("* pclk1         : %0.3fMhz\r\n", cpu.clock.pclk1 / 1000000.0);
//    UART.printf("* pclk2         : %0.3fMhz\r\n", cpu.clock.pclk2 / 1000000.0);
//    UART.printf("* ability/s     : %0.3fMhz\r\n", cpu.ability / 1000000.0);

//    UART.printf("* flash size    : %d    \tKB \r\n", cpu.flash_size);
//    UART.printf("* flash used    : %0.3f \tKB\r\n", MCU_FLASH_USED / 1024.0);	/* ��ӡһ�пո� */
//    UART.printf("* flash remaind : %0.3f \tKB\r\n", MCU_FLASH_REMAIND / 1024.0);	/* ��ӡһ�пո� */

//    UART.printf("* mem size      : %0.3f \tKB \r\n", MCU_SRAM1_SIZE / 1024.0);
//    UART.printf("* mem used      : %0.3f \tKB\r\n", MCU_SRAM1_USED / 1024.0);	/* ��ӡһ�пո� */
//    UART.printf("* mem remaind   : %0.3f \tKB\r\n", MCU_SRAM1_REMAIND / 1024.0);	/* ��ӡһ�пո� */
//    UART.printf("* heap used     : %0.3f \tKB\r\n", ebox_mem_used() / 1024.0);	/* ��ӡһ�пո� */
//    UART.printf("* heap free     : %0.3f \tKB\r\n", ebox_get_free() / 1024.0);	/* ��ӡһ�пո� */
//    UART.printf("* \r\n");	/* ��ӡһ�пո� */
//    UART.printf("*************************************************************\n\r");
}



