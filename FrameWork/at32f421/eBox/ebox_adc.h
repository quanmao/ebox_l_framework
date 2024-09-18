/**
  ******************************************************************************
  * @file    ebox_analog.h
  * @author  cat_li
  * @brief   
	*		2017/7/31	 LQM
			1	��ͨ��ADCת��ģʽ
	*		2017/8/2		 LQM
			1	��ͨ����������
			2	��ͨ�����β���,��������
			2017/8/28
			1	��ͨ��ģʽ��ͨ��ָ��������ȣ���������������ָ����������
	* ��֪bug
			1 �ڲ��ο���ѹ��һ�ζ�ȡ��ȷ��֮��ÿ�ζ�ȡ��Ҫ���800ms�����ܱ�֤�ο���ѹ��ȷ
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

#ifndef __EBOX_ANALOG_H_
#define __EBOX_ANALOG_H_

#include "mcu_config.h"
#include "mcu_define.h"

#define REFVOLTAGE 3300 // ADC�ο���ѹ
#define USE_INTERVREF 1
#ifdef USE_TIMEOUT
#define TIMEOUT 1000 // Ĭ�ϳ�ʱʱ��Ϊ1000us
#endif

class E_ADC
{
public:
	typedef enum
	{
		ch_temperature = 0,
#if defined(ADC_CCR_VBATEN)
		ch_battery,
#endif
#if defined(USE_INTERVREF)
		ch_intervRef
#endif
	} ADC_CH;
public:
	E_ADC(E_GPIO *io);
	E_ADC(ADC_CH ch);

	uint16_t read();


private:
	uint32_t _channel; // adcͨ��
	void _setInterChannel(void);
	void _init();
};

extern uint16_t calcVoltage(uint16_t v, uint16_t ref = REFVOLTAGE);
#if defined(USE_INTERVREF)
extern uint16_t calcInterCalRef(uint16_t v);
#endif
extern float calcTemperature(uint16_t v, uint16_t ref = REFVOLTAGE);

//class E_AdcDMA{
//public:
//	E_AdcDMA(E_PinID id);
//	E_AdcDMA(uint8_t chNum,uint8_t deep = 0);
//	E_AdcDMA(uint32_t channel,uint8_t chNum,uint8_t deep = 0);

//	void addChannel(E_PinID id);

//	void dmaConfig();
//	void dmaStart();
//	void update();

//	uint16_t *r_buffer;		// ת�����
//private:
//	uint32_t _channel;
//	uint8_t  _deep;
//	static uint8_t  _channelNum;
//};

#endif
