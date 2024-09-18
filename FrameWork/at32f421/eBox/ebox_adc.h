/**
  ******************************************************************************
  * @file    ebox_analog.h
  * @author  cat_li
  * @brief   
	*		2017/7/31	 LQM
			1	单通道ADC转换模式
	*		2017/8/2		 LQM
			1	单通道连续采样
			2	多通道单次采样,连续采样
			2017/8/28
			1	多通道模式，通过指定采样深度，决定连续采样和指定次数采样
	* 已知bug
			1 内部参考电压第一次读取正确，之后每次读取需要间隔800ms，才能保证参考电压正确
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

#define REFVOLTAGE 3300 // ADC参考电压
#define USE_INTERVREF 1
#ifdef USE_TIMEOUT
#define TIMEOUT 1000 // 默认超时时间为1000us
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
	uint32_t _channel; // adc通道
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

//	uint16_t *r_buffer;		// 转换结果
//private:
//	uint32_t _channel;
//	uint8_t  _deep;
//	static uint8_t  _channelNum;
//};

#endif
