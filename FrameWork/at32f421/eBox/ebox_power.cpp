/**
  ******************************************************************************
  * @file    ebox power.c
  * @author  cat_li
  * @brief   ��Դ�����ṩ�����λ��sleep,stop&standby��������ģʽ
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
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
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
 *@brief    cpu����,���ں�ֹͣ�����豣�����С�ע��:sleep_on_exti���жϻ��Ѻ�ִ�����жϻ��ٴν���sleepģʽ
 *@param    uint8_t isSleepnow: 0 sleep_on_extiģʽ, 1 sleep nowģʽ
            uint8_t entry: 0 wfi,�����жϻ���, 1 wfe,�����¼�����
            Ĭ��Ϊ1 sleep now  0 wfi ģʽ          
 *@retval   none
 */
	void eboxSleep(sleep_t isSleepnow , entry_t entry)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
		//��Ҫ�ر�systick�жϣ���ֹ������
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
		// ���Ѻ�ָ�sysTick�ж�
		LL_SYSTICK_EnableIT();
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
	}
/**
 *@brief    1.8v������������ʱ��ֹͣ,SRAM�ͼĴ������ݱ���,����IO����������ģʽ״̬
 *@param    uint8_t isSleepnow: 0 sleep_on_extiģʽ, 1 sleep nowģʽ
            uint8_t entry: 0 wfi,�����ⲿ�жϻ���, 1 wfe,�����ⲿ�¼�����
            Ĭ��Ϊ1 sleep now  0 wfi ģʽ            
 *@retval   none
 */
	void eboxStop(sleep_t isSleepnow , entry_t entry)
	{
		// ��Ҫ����PWRʱ�ӣ����ܹ�дPWR�Ĵ���
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

        // ���Ѻ�ָ�sysTick�ж�
//		LL_SYSTICK_EnableIT();
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
        eboxClockConfig();
		//  DBG(" || wake up from stop mode !\r\n");
	}
/**
 *@brief    ֻ�б��ݼĴ����ʹ�����·�������˳����൱������
 *@param    uint32_t wakeUpPin: LL_PWR_WAKEUP_PIN1,LL_PWR_WAKEUP_PIN2 �ο�stm32f0xx_ll_pwr.h����                     
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

