/**
  ******************************************************************************
  * @file    ebox power.h
  * @author  cat_li
  * @brief   电源管理，提供软件复位，sleep,stop&standby三种休眠模式
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

#ifndef __EBOX_POWER_MANAGER_H__
#define __EBOX_POWER_MANAGER_H__

#include "stm32f0xx_ll_pwr.h" //移植到对应芯片定义文件


#ifdef __cplusplus
extern "C"
{
#endif

// power manage
 typedef enum {
  sleepOnExti, // 执行完唤醒睡眠的中断后自动进入睡眠
  sleepNow	// 执行完唤醒睡眠的中断后进入主循环
 }sleep_t;

 typedef enum {
  wfi,	// 外部中断唤醒
  wfe	// 外部事件唤醒，没有中断进出造成的时间延迟
 }entry_t;

// 复位类型
#define RST_LOWPOWER 0x80	// 低功耗复位
#define RST_WWDOG 0x40		// 窗口看门狗复位
#define RST_IWDOG 0x20		// 独立看门狗复位
#define RST_SOFT 0x10		// 软件复位
#define RST_POWER 0x08		// PDR,POR复位
#define RST_NRST 0x04		// 引脚复位
#define RST_OBL 0x02		// 
	//休眠模式
	extern void eboxSleep(sleep_t isSleepnow = sleepNow, entry_t entry = wfi); // 仅关闭CPU时钟，所有外设仍运行，可以通过任何中断唤醒,IO保持休眠前状态
	extern void eboxStop(sleep_t isSleepnow = sleepNow, entry_t entry = wfi); //1.8v域所有时钟关闭，SRAM和寄存器保存，可以通过任何外部中断/事件唤醒,特定外设(CEC, USART, I2C)中断(需编程为唤醒模式), IO保持休眠前状态
	extern void eboxStandby(uint16_t wakeUpPin = LL_PWR_WAKEUP_PIN1); // wkup pin拉高,外部复位,rtc alarm,内部看门狗可唤醒,默认PA0
	
	/* 如果ebox_sleep,ebox_stop设置为sleep_on_exti模式,在中断中调用该函数可以退出,返回主循环 */
	__STATIC_INLINE void eboxExitSleepOnExti()
	{
		CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
		CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
	}
	/* 返回上次是否为standby模式，并清空标志位*/
	__STATIC_INLINE bool eboxIsWakeFromSB()
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
		bool r = (READ_BIT(PWR->CSR, PWR_CSR_SBF) == (PWR_CSR_SBF));
		SET_BIT(PWR->CR, PWR_CR_CSBF);
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
		return r;
	}
	// 默认返回值为0x0c,即RST_POWER|RST_NRST
	__STATIC_INLINE uint8_t eboxGetResetType()
	{
		return (RCC->CSR) >> 24;
	}

	// 软件重启，执行后复位寄存器返回值为0x1c
	__STATIC_INLINE void eboxRest(void)
	{
		NVIC_SystemReset();
	}

	__STATIC_INLINE void eboxClarRestFlag(void) // 清除复位标志
	{
		RCC->CSR |= RCC_CSR_RMVF;
	}
#ifdef __cplusplus
}
#endif

#endif
