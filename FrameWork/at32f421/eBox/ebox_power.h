/**
  ******************************************************************************
  * @file    ebox power.h
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

#ifndef __EBOX_POWER_MANAGER_H__
#define __EBOX_POWER_MANAGER_H__

#include "stm32f0xx_ll_pwr.h" //��ֲ����ӦоƬ�����ļ�


#ifdef __cplusplus
extern "C"
{
#endif

// power manage
 typedef enum {
  sleepOnExti, // ִ���껽��˯�ߵ��жϺ��Զ�����˯��
  sleepNow	// ִ���껽��˯�ߵ��жϺ������ѭ��
 }sleep_t;

 typedef enum {
  wfi,	// �ⲿ�жϻ���
  wfe	// �ⲿ�¼����ѣ�û���жϽ�����ɵ�ʱ���ӳ�
 }entry_t;

// ��λ����
#define RST_LOWPOWER 0x80	// �͹��ĸ�λ
#define RST_WWDOG 0x40		// ���ڿ��Ź���λ
#define RST_IWDOG 0x20		// �������Ź���λ
#define RST_SOFT 0x10		// �����λ
#define RST_POWER 0x08		// PDR,POR��λ
#define RST_NRST 0x04		// ���Ÿ�λ
#define RST_OBL 0x02		// 
	//����ģʽ
	extern void eboxSleep(sleep_t isSleepnow = sleepNow, entry_t entry = wfi); // ���ر�CPUʱ�ӣ��������������У�����ͨ���κ��жϻ���,IO��������ǰ״̬
	extern void eboxStop(sleep_t isSleepnow = sleepNow, entry_t entry = wfi); //1.8v������ʱ�ӹرգ�SRAM�ͼĴ������棬����ͨ���κ��ⲿ�ж�/�¼�����,�ض�����(CEC, USART, I2C)�ж�(����Ϊ����ģʽ), IO��������ǰ״̬
	extern void eboxStandby(uint16_t wakeUpPin = LL_PWR_WAKEUP_PIN1); // wkup pin����,�ⲿ��λ,rtc alarm,�ڲ����Ź��ɻ���,Ĭ��PA0
	
	/* ���ebox_sleep,ebox_stop����Ϊsleep_on_extiģʽ,���ж��е��øú��������˳�,������ѭ�� */
	__STATIC_INLINE void eboxExitSleepOnExti()
	{
		CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
		CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
	}
	/* �����ϴ��Ƿ�Ϊstandbyģʽ������ձ�־λ*/
	__STATIC_INLINE bool eboxIsWakeFromSB()
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
		bool r = (READ_BIT(PWR->CSR, PWR_CSR_SBF) == (PWR_CSR_SBF));
		SET_BIT(PWR->CR, PWR_CR_CSBF);
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
		return r;
	}
	// Ĭ�Ϸ���ֵΪ0x0c,��RST_POWER|RST_NRST
	__STATIC_INLINE uint8_t eboxGetResetType()
	{
		return (RCC->CSR) >> 24;
	}

	// ���������ִ�к�λ�Ĵ�������ֵΪ0x1c
	__STATIC_INLINE void eboxRest(void)
	{
		NVIC_SystemReset();
	}

	__STATIC_INLINE void eboxClarRestFlag(void) // �����λ��־
	{
		RCC->CSR |= RCC_CSR_RMVF;
	}
#ifdef __cplusplus
}
#endif

#endif
