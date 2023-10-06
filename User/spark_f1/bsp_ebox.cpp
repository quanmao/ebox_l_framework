/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "bsp_ebox.h"

//// 如果不使用object.cpp, 使用下列方式创建IO对象
#if USE_OBJECT == 0
// 如果不使用object.cpp, 使用下列方式创建IO对象
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
E_GPIO PA15(PA15_ID);

E_GPIO PB0(PB0_ID);
E_GPIO PB1(PB1_ID);
E_GPIO PB2(PB2_ID);
E_GPIO PB3(PB3_ID);
E_GPIO PB4(PB4_ID);
E_GPIO PB5(PB5_ID);
E_GPIO PB6(PB6_ID);
E_GPIO PB7(PB7_ID);
E_GPIO PB8(PB8_ID);
E_GPIO PB9(PB9_ID);
E_GPIO PB10(PB10_ID);
E_GPIO PB11(PB12_ID);
E_GPIO PB12(PB12_ID);
E_GPIO PB13(PB13_ID);
E_GPIO PB14(PB14_ID);
E_GPIO PB15(PB15_ID);

E_GPIO PF0(PF0_ID);
E_GPIO PF1(PF1_ID);
E_GPIO PE2(PE2_ID);

E_EXTI BTN(&PA8);
E_UART uart1(UART1);
#endif
/*
*********************************************************************************************************
*	函 数 名: PrintfLog
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void print_log(const char *name, const char *date)
{
//    UART.printf("\n\r");
//    UART.printf("*************************************************************\n\r");
//    UART.printf("* \r\n");	                /* 打印一行空格 */
//    UART.printf("* 例程名称      : ");	    /* 打印例程名称 */
//    UART.printf(name);
//    UART.printf("* 发布日期      : ");	    /* 打印例程日期 */
//    UART.printf(date);

//    UART.print("* 硬件平台      : ");       /* 硬件平台 */
//    UART.println(HARDWARE);
//    UART.print("* EBOX库版本    : ebox_" );/* 版本信息 */
//    UART.println(EBOX_VERSION);
//    UART.print("*                     CPU 信息\r\n");	/* CPU信息 */
//    UART.println();
//    UART.print("* CPU TYPE      : ");	    /* 打印CPU类型 */
//    UART.println(MCU_TYPE);
//    UART.print("* CPU PINS      : ");	    /* 打印CPU引脚数量 */
//    UART.println(STM32_PINS);
//    UART.print("* CPU COMPANY   : ");	    /* 打印CPU公司 */
//    UART.println(MCU_COMPANY);


//    UART.print("* CPUID         : ");       /* 打印CPU唯一ID */
//    UART.print(cpu.chip_id[2], HEX);
//    UART.print(cpu.chip_id[1], HEX);
//    UART.println(cpu.chip_id[0], HEX);


//    UART.print("* core          : ");
//    UART.print(cpu.clock.core / 1000000.0);	/* 打印时钟树信息 */
//    UART.println("Mhz");
//    UART.print("* hclk          : ");
//    UART.print(cpu.clock.hclk / 1000000.0);
//    UART.println("Mhz");
//    UART.print("* pclk1         : ");
//    UART.print(cpu.clock.pclk1 / 1000000.0);
//    UART.println("Mhz");
//    UART.print("* pclk2         : ");
//    UART.print(cpu.clock.pclk2 / 1000000.0);
//    UART.println("Mhz");

//    UART.print("* ability/s     : ");
//    UART.print(cpu.ability / 1000000.0);
//    UART.println("Mhz");


//    UART.print("* flash size    : ");       /* 打印flash大小 */
//    UART.print(cpu.flash_size);
//    UART.println("KB");

//    UART.print("* flash used    : ");	    /* 打印flash使用了多少KB */
//    UART.print(MCU_FLASH_USED / 1024.0);
//    UART.println("KB");

//    UART.print("* flash remaind : ");	    /* 打印flash剩余了多少KB  */
//    UART.print(MCU_FLASH_REMAIND / 1024.0);
//    UART.println("KB");

//    UART.print("* mem size      : ");       /* 打印SRAM大小 */
//    UART.print(MCU_SRAM1_SIZE / 1024.0);
//    UART.println("KB");

//    UART.print("* mem used      : ");	    /* 打印SRAM使用了多少KB */
//    UART.print(MCU_SRAM1_USED / 1024.0);
//    UART.println("KB");

//    UART.print("* mem remaind   : ");	    /* 打印SRAM剩余了多少KB */
//    UART.print(MCU_SRAM1_REMAIND / 1024.0);
//    UART.println("KB");

//    UART.print("* heap used     : ");	    /* 打印heap使用率 */
//    UART.print(ebox_mem_usage());
//    UART.println("%");

//    UART.print("* heap used     : ");	    /* 打印heap使用率 */
//    UART.print(ebox_mem_used() / 1024.0);
//    UART.println("KB");

//    UART.print("* heap free     : ");	    /* 打印heap使用了多少KB */
//    UART.print(ebox_get_free() / 1024.0);
//    UART.println("KB");

//    UART.print("* \r\n");	                /* 打印一行空格 */
//    UART.print("*************************************************************\n\r");
//    UART.flush();
}


