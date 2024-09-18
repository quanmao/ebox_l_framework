/**
  ******************************************************************************
  * @file    ebox power.c
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
#include "ebox_base.h"
#include "ebox_power.h"



#if USE_PRINTF
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#include "ebox_debug.h"
#define DEBUG(...) DBG("[CORE]  "), DBG(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
/**
 *@brief    cpu休眠,仅内核停止，外设保持运行。注意:sleep_on_exti被中断唤醒后执行完中断会再次进入sleep模式
 *@param    uint8_t isSleepnow: 0 sleep_on_exti模式, 1 sleep now模式
            uint8_t entry: 0 wfi,任意中断唤醒, 1 wfe,任意事件唤醒
            默认为1 sleep now  0 wfi 模式          
 *@retval   none
 */
	void eboxSleep(sleep_t isSleepnow , entry_t entry)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
		//需要关闭systick中断，防止被唤醒
		LL_SYSTICK_DisableIT();
		/* Set STOP_LPREGU mode when CPU enters deepsleep */
		LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_MAINREGU);
		/* Set SLEEPDEEP bit of Cortex System Control Register */
		LL_LPM_EnableSleep();
		if (isSleepnow)
		{
			//		DBG(" || sleepNow");
			LL_LPM_DisableSleepOnExit();
			/* Request Wait For Interrupt */
			if (entry == 0)
			{
				// DBG(" || wfi mode");
				__WFI();
			}
			else
			{
				// DBG(" || wfe mode");
				__SEV();
				__WFE();
				__WFE();
			}
		}
		else
		{
			LL_LPM_EnableSleepOnExit();
			__WFI();
		}
		// 唤醒后恢复sysTick中断
		LL_SYSTICK_EnableIT();
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
	}
/**
 *@brief    1.8v供电区域所有时钟停止,SRAM和寄存器内容保留,所有IO保持在运行模式状态
 *@param    uint8_t isSleepnow: 0 sleep_on_exti模式, 1 sleep now模式
            uint8_t entry: 0 wfi,任意外部中断唤醒, 1 wfe,任意外部事件唤醒
            默认为1 sleep now  0 wfi 模式            
 *@retval   none
 */
	void eboxStop(sleep_t isSleepnow , entry_t entry)
	{
		// 需要开启PWR时钟，才能够写PWR寄存器
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
		/* Set STOP_LPREGU mode when CPU enters deepsleep */
		LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_LPREGU);
		/* Set SLEEPDEEP bit of Cortex System Control Register */
		LL_LPM_EnableDeepSleep();
		/* Request Wait For Interrupt */
		if (isSleepnow)
		{
			//    DBG(" || sleepNow");
			LL_LPM_DisableSleepOnExit();
			/* Request Wait For Interrupt */
			if (entry == 0)
			{
				//  DBG(" || wfi mode");
				__WFI();
			}
			else
			{
				//  DBG(" || wfe mode");
				__SEV();
				__WFE();
				__WFE();
			}
		}
		else
		{
			//    DBG(" || sleep_on_exti");
			LL_LPM_EnableSleepOnExit();
			//    DBG(" || wfi mode");
			__WFI();
		}

        // 唤醒后恢复sysTick中断
//		LL_SYSTICK_EnableIT();
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
        eboxClockConfig();
		//  DBG(" || wake up from stop mode !\r\n");
	}
/**
 *@brief    只有备份寄存器和待机电路工作，退出后相当与重启
 *@param    uint32_t wakeUpPin: LL_PWR_WAKEUP_PIN1,LL_PWR_WAKEUP_PIN2 参考stm32f0xx_ll_pwr.h定义                     
 *@retval   none
 */
	void eboxStandby(uint16_t wakeUpPin)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
		/* Disable all used wakeup sources */
		LL_PWR_DisableWakeUpPin(wakeUpPin);

		/* Clear all wake up Flag */
		LL_PWR_ClearFlag_WU();

		/* Enable wakeup pin */
		LL_PWR_EnableWakeUpPin(wakeUpPin);
		/* Set STANDBY mode when CPU enters deepsleep */
		LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
		/* Set SLEEPDEEP bit of Cortex System Control Register */
		LL_LPM_EnableDeepSleep();
		LL_LPM_EnableSleepOnExit();
		/* Request Wait For Interrupt */
		__WFI();
	}

#ifdef __cplusplus
}
#endif

