/**
  ******************************************************************************
  * @file    ebox_analog.cpp
  * @author  cat_li
  * @brief   
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
#include "stm32f1xx_ll_adc.h"
#include "ebox_gpio.h"
#include "ebox_adc.h"

static uint32_t _interChannel = LL_ADC_PATH_INTERNAL_NONE;

#define VREFINT_CAL_VREF                   ( 1200U)                    /* Analog voltage reference (Vref+) value with which temperature sensor has been calibrated in production (tolerance: +-10 mV) (unit: mV). */

#define __LL_ADC_CALC_VREFANALOG_VOLTAGE(__VREFINT_ADC_DATA__,\
                                         __ADC_RESOLUTION__)                   \
  ((uint32_t)(VREFINT_CAL_VREF*__LL_ADC_DIGITAL_SCALE(__ADC_RESOLUTION__))/__VREFINT_ADC_DATA__                \
  )

/**
 *@name     void ADC1_init(void)
 *@brief    ADC初始化
 *@param    NONE
 *@retval   NONE
*/
void E_ADC::_init()
{

  /* Enable ADC clock (core clock) */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, in order to be compliant with other STM32 series          */
  /*       and to show the best practice usages, ADC state is checked.        */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
//  if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
//  {
//    /* Note: Call of the functions below are commented because they are       */
//    /*       useless in this example:                                         */
//    /*       setting corresponding to default configuration from reset state. */

//    /* Set ADC measurement path to internal channels */
//    // LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_NONE);
//  }

  /*## Configuration of ADC hierarchical scope: ADC group regular ############*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, ADC state is checked anyway with standard requirements    */
  /*       (refer to description of this function).                           */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Set ADC group regular trigger source */
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);

    /* Set ADC group regular trigger polarity */
    // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);

    /* Set ADC group regular continuous mode */
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);

    /* Set ADC group regular conversion data transfer */
    // LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);

    /* Set ADC group regular sequencer */
    /* Note: On this STM32 serie, ADC group regular sequencer is              */
    /*       fully configurable: sequencer length and each rank               */
    /*       affectation to a channel are configurable.                       */
    /*       Refer to description of function                                 */
    /*       "LL_ADC_REG_SetSequencerLength()".                               */

    /* Set ADC group regular sequencer length and scan direction */
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);

    /* Set ADC group regular sequencer discontinuous mode */
    // LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);

    /* Set ADC group regular sequence: channel on the selected sequence rank. */
    //    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
  }
  /* Set ADC channels sampling time */
  LL_ADC_SetChannelSamplingTime(ADC1, _channel, LL_ADC_SAMPLINGTIME_41CYCLES_5);
}

/**
 *@name     uint16_t analog_read(uint32_t *channel)
 *@brief    读取ADC1某个引脚上的模拟转换结果
 *@param    channel：ADC1 通道
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的12bit的ADC转换结果
            如果引脚错误返回0；
*/
uint16_t E_ADC::read(void)
{
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Enable ADC */
    LL_ADC_Enable(ADC1);
    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1);

    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
      ;
  }

  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, _channel);
  LL_ADC_REG_StartConversionSWStart(ADC1);

  while (!LL_ADC_IsActiveFlag_EOS(ADC1))
    ;
  return LL_ADC_REG_ReadConversionData12(ADC1);
}

#if USE_INTERVREF
uint16_t calcInterCalRef(uint16_t v)
{
  return (uint32_t)(VREFINT_CAL_VREF * __LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B)) / v;
}
#endif

/**
 *@brief    读取当前通道的模拟电压
 *@param    none
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压,单位mv
*/
uint16_t calcVoltage(uint16_t v, uint16_t ref)
{
  return __LL_ADC_CALC_DATA_TO_VOLTAGE(ref, v, LL_ADC_RESOLUTION_12B);
}

float calcTemperature(uint16_t v, uint16_t ref)
{
  return __LL_ADC_CALC_TEMPERATURE_TYP_PARAMS(4300, 1430, 25, ref, v, LL_ADC_RESOLUTION_12B);
}

/**
 *@brief    构造函数
 *@param    E_PinID id, Pin id
 *		   uint16_t refVoltage 参考电压，默认3300
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压，默认参考电压为3.3V
*/
E_ADC::E_ADC(E_GPIO *io)
{
  uint8_t index;
  index = getIndex(io->id, ADC_MAP);
  _channel = ADC_MAP[index]._periph;
  io->mode(ADC_MAP[index]._pinMode);

  _init();
}

/**
 *@brief    构造函数
 *@param    ADC_CH ch  内部通道 temperature_ch & bettery_ch
 *@retval   none
*/
E_ADC::E_ADC(ADC_CH ch)
{
  switch ((uint8_t)ch)
  {
  case ch_temperature:
    _channel = LL_ADC_CHANNEL_TEMPSENSOR;
    _interChannel |= LL_ADC_PATH_INTERNAL_TEMPSENSOR;
    break;
#if defined(ADC_CCR_VBATEN)
  case ch_battery:
    _channel = LL_ADC_CHANNEL_VBAT;
    _interChannel |= LL_ADC_PATH_INTERNAL_VBAT;
    break;
#endif
  case ch_intervRef:
    _channel = LL_ADC_CHANNEL_VREFINT;
    _interChannel |= LL_ADC_PATH_INTERNAL_VREFINT;
    break;
  }
  _setInterChannel();
  _init();
}

/**
 *@brief    设置内部通道
 *@param    none
 *@retval   
*/
void E_ADC::_setInterChannel(void)
{
//  if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
//  {
//    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), _interChannel);
//  }
}

//uint8_t E_AdcDMA::_channelNum = 0;

///**
// *@brief    构造函数
// *@param    E_PinID id   pin_id
// *@retval   none
//*/
//E_AdcDMA::E_AdcDMA(E_PinID id){
//	uint8_t index;
//	index = getIndex(id,ADC_MAP);
//	_channel = ADC_MAP[index]._periph_OR_ch;
//	_channelNum = 1;
//}

///**
// *@brief    构造函数
// *@param    uint32_t channel  通道  LL_ADC_CHANNEL_0 ... ll_adc_channel_15 
// *		   uint8_t chNum     通道数，需要和前面的保持一致
// *		   uint8_t deep      采样深度 0 连续采样  其它 非连续采样，采样指定次数后停止
// *@retval   none
//*/
//E_AdcDMA::E_AdcDMA(uint8_t chNum,uint8_t deep){
//	_channelNum = chNum;
//	_deep = deep;
//}

///**
// *@brief    构造函数
// *@param    uint32_t channel  通道  LL_ADC_CHANNEL_0 ... ll_adc_channel_15 
// *		   uint8_t chNum     通道数，需要和前面的保持一致
// *		   uint8_t deep      采样深度 0 连续采样  其它 非连续采样，采样指定次数后停止
// *@retval   none
//*/
//E_AdcDMA::E_AdcDMA(uint32_t channel,uint8_t chNum,uint8_t deep){
//	_channel = channel;
//	_channelNum = chNum;
//	_deep = deep;
//}

//void E_AdcDMA::addChannel(E_PinID id){
//	uint8_t index;
//	index = getIndex(id,ADC_MAP);
//	_channel |= ADC_MAP[index]._periph_OR_ch;
//	_channelNum++;

//}

//void E_AdcDMA::dmaConfig()
//{
//	if (r_buffer != NULL)
//	{
//		free(r_buffer);
//	}
//	/* Enable ADC clock (core clock) */
//	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);

//	/*## Configuration of ADC hierarchical scope: ADC instance #################*/
//	if (LL_ADC_IsEnabled(ADC1) == 0)
//	{
//		/* Set ADC clock (conversion clock) */
//		LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);

//		/* Set ADC data resolution */
//		// LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);

//		/* Set ADC conversion data alignment */
//		// LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);

//		/* Set ADC low power mode */
//		// LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);

//		/* Set ADC channels sampling time */
//		LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_41CYCLES_5);
//	}
//	// 设置转换规则
//	if ((LL_ADC_IsEnabled(ADC1) == 0)||(LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
//	{
//		/* Set ADC group regular trigger source */
//		LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
//		/* Set ADC group regular trigger polarity */
//		// LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
//		/* Set ADC group regular continuous mode */
//		LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
//		/* Set ADC group regular conversion data transfer */
////    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
//		/* Set ADC group regular overrun behavior */
//		LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

//		/* Set ADC group regular sequencer */
//		/* channel number.                                                        */
//		LL_ADC_REG_SetSequencerChannels(ADC1, _channel);
//	}

//	/*## Configuration of ADC transversal scope: oversampling ##################*/

//	/* Set ADC oversampling scope */
//	// LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
//	/* Set ADC oversampling parameters */
//	// LL_ADC_ConfigOverSamplingRatioShift(ADC1, LL_ADC_OVS_RATIO_2, LL_ADC_OVS_SHIFT_NONE);

//	/* Enable the peripheral clock of DMA */
//	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
//	
//	if (_deep == 0){
//		r_buffer = (uint16_t*)malloc(_channelNum);
//		/* Set DMA transfer size */
//		LL_DMA_SetDataLength(DMA1,
//		                     LL_DMA_CHANNEL_1,
//		                     _channelNum);
//		LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
//	}else{
//		r_buffer = (uint16_t*)malloc(_channelNum*_deep);
//		LL_DMA_SetDataLength(DMA1,
//		                     LL_DMA_CHANNEL_1,
//		                     _channelNum*_deep);
//		LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_LIMITED);
//	}

//	/* Configure the DMA transfer */
//	/*  - DMA transfer in circular mode to match with ADC configuration:        */
//	/*    DMA unlimited requests.                                               */
//	/*  - DMA transfer from ADC without address increment.                      */
//	/*  - DMA transfer to memory with address increment.                        */
//	/*  - DMA transfer from ADC by half-word to match with ADC configuration:   */
//	/*    ADC resolution 12 bits.                                               */
//	/*  - DMA transfer to memory by half-word to match with ADC conversion data */
//	/*    buffer variable type: half-word.                                      */
//	LL_DMA_ConfigTransfer(DMA1,
//	                      LL_DMA_CHANNEL_1,
//	                      LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
//	                      LL_DMA_MODE_CIRCULAR              |
//	                      LL_DMA_PERIPH_NOINCREMENT         |
//	                      LL_DMA_MEMORY_INCREMENT           |
//	                      LL_DMA_PDATAALIGN_HALFWORD        |
//	                      LL_DMA_MDATAALIGN_HALFWORD        |
//	                      LL_DMA_PRIORITY_HIGH               );

//	/* Set DMA transfer addresses of source and destination */
//	LL_DMA_ConfigAddresses(DMA1,
//	                       LL_DMA_CHANNEL_1,
//	                       LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
//	                       (uint32_t)r_buffer,
//	                       LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

///* Configure NVIC to enable DMA interruptions */
////  NVIC_SetPriority(DMA1_Channel1_IRQn, 1); /* DMA IRQ lower priority than ADC IRQ */
////  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
//	
////  /* Enable DMA transfer interruption: transfer complete */
////  LL_DMA_EnableIT_TC(DMA1,LL_DMA_CHANNEL_1);

////  /* Enable DMA transfer interruption: half transfer */
////  LL_DMA_EnableIT_HT(DMA1,LL_DMA_CHANNEL_1);

////  /* Enable DMA transfer interruption: transfer error */
////  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

//	/* Enable the DMA transfer */
//	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
//}

///**
// *@brief    启动DMA采样
// *@param    none
// *@retval   none
//*/
//void E_AdcDMA::dmaStart()
//{
//	__IO uint32_t backup_setting_adc_dma_transfer = 0;  // 保存DMA设置

//	if (LL_ADC_IsEnabled(ADC1) == 0)
//	{
//		backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(ADC1);
//		LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);

//		/* Run ADC self calibration */
//		LL_ADC_StartCalibration(ADC1);

//		while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
//		{

//		}

//		/* Restore ADC DMA transfer request after calibration */
//		LL_ADC_REG_SetDMATransfer(ADC1, backup_setting_adc_dma_transfer);
//		/* Enable ADC */
//		LL_ADC_Enable(ADC1);

//		while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
//		{

//		}

//		if ((LL_ADC_IsEnabled(ADC1) == 1)&&(LL_ADC_IsDisableOngoing(ADC1) == 0)&&(LL_ADC_REG_IsConversionOngoing(ADC1) == 0))
//		{
//			//LL_ADC_REG_StartConversion(ADC1);
//			update();
//		}
//	}
//}

///**
// *@brief    更新数据，仅在非连续转换情况下调用
// *@param    none
// *@retval   none
//*/
//void E_AdcDMA::update(void){
//	// 启动ADC,并等待结束
//	LL_ADC_REG_StartConversion(ADC1);
//	while (!LL_DMA_IsActiveFlag_TC1(DMA1));
//}
