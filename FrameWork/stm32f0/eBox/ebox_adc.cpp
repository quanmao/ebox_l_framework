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
#include "stm32f0xx_ll_adc.h"
#include "ebox_gpio.h"
#include "ebox_adc.h"

static uint32_t _interChannel = LL_ADC_PATH_INTERNAL_NONE;

/**
 *@name     void ADC1_init(void)
 *@brief    ADC��ʼ��
 *@param    NONE
 *@retval   NONE
*/
void E_ADC::_init()
{
 
  /* Enable ADC clock (core clock) */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);
    
  // ADCû������,����ADCʱ��
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Set ADC clock (conversion clock) */
    LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV4);
    
    /* Set ADC data resolution */
    LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
    
    /* Set ADC conversion data alignment */
    LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    
    /* Set ADC low power mode */
    LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);
    
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       Therefore, sampling time is configured here under ADC instance   */
    /*       scope (not under channel scope as on some other STM32 devices    */
    /*       on which sampling time is channel wise).                         */
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_71CYCLES_5);
    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC group regular ############*/
  // ����ADC
  if ((LL_ADC_IsEnabled(ADC1) == 0)||(LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC group regular trigger source */
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);    
    /* Set ADC group regular trigger polarity */
    // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);    
    /* Set ADC group regular continuous mode */
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);    
    /* Set ADC group regular conversion data transfer */
    // LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);    
    /* Set ADC group regular overrun behavior */
    LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);    
    /* Set ADC group regular sequencer */
  }
  
  /*## Configuration of ADC hierarchical scope: channels #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on either groups regular or injected.                              */
  if ((LL_ADC_IsEnabled(ADC1) == 0)||(LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       See sampling time configured above, at ADC instance scope.       */
    
  }

}

/**
 *@name     uint16_t analog_read(uint32_t *channel)
 *@brief    ��ȡADC1ĳ�������ϵ�ģ��ת�����
 *@param    channel��ADC1 ͨ��
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��12bit��ADCת�����
            ������Ŵ��󷵻�0��
*/
uint16_t E_ADC::read(void)
{

	if (LL_ADC_IsEnabled(ADC1) == 0)
	{
		/* Run ADC self calibration */
		LL_ADC_StartCalibration(ADC1);

		while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
		{

		}

		/* Enable ADC */
		LL_ADC_Enable(ADC1);

		while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
		{

		}
	}

	LL_ADC_REG_SetSequencerChannels(ADC1, _channel);
	LL_ADC_REG_StartConversion(ADC1);
	
	while (!LL_ADC_IsActiveFlag_EOC(ADC1));
	return LL_ADC_REG_ReadConversionData12(ADC1);
}

#if USE_INTERVREF
uint16_t calcInterCalRef(uint16_t v)
{
  return __LL_ADC_CALC_VREFANALOG_VOLTAGE(v,LL_ADC_RESOLUTION_12B);;
}
#endif

/**
 *@brief    ��ȡ��ǰͨ����ģ���ѹ
 *@param    none
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��ģ���ѹ,��λmv
*/
uint16_t calcVoltage(uint16_t v, uint16_t ref)
{
  return __LL_ADC_CALC_DATA_TO_VOLTAGE(ref, v, LL_ADC_RESOLUTION_12B);
}

float calcTemperature(uint16_t v, uint16_t ref)
{
	return __LL_ADC_CALC_TEMPERATURE(ref,v,LL_ADC_RESOLUTION_12B);
}

/**
 *@brief    ���캯��
 *@param    E_GPIO *io
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
 *@brief    ���캯��
 *@param    ADC_CH ch  �ڲ�ͨ�� temperature_ch & bettery_ch
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
 *@brief    �����ڲ�ͨ��
 *@param    none
 *@retval   
*/
void E_ADC::_setInterChannel(void){
	if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
	{
		LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1),_interChannel);
	}
}


//uint8_t E_AdcDMA::_channelNum = 0;

///**
// *@brief    ���캯��
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
// *@brief    ���캯��
// *@param    uint32_t channel  ͨ��  LL_ADC_CHANNEL_0 ... ll_adc_channel_15 
// *		   uint8_t chNum     ͨ��������Ҫ��ǰ��ı���һ��
// *		   uint8_t deep      ������� 0 ��������  ���� ����������������ָ��������ֹͣ
// *@retval   none
//*/
//E_AdcDMA::E_AdcDMA(uint8_t chNum,uint8_t deep){
//	_channelNum = chNum;
//	_deep = deep;
//}

///**
// *@brief    ���캯��
// *@param    uint32_t channel  ͨ��  LL_ADC_CHANNEL_0 ... ll_adc_channel_15 
// *		   uint8_t chNum     ͨ��������Ҫ��ǰ��ı���һ��
// *		   uint8_t deep      ������� 0 ��������  ���� ����������������ָ��������ֹͣ
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
//	// ����ת������
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
// *@brief    ����DMA����
// *@param    none
// *@retval   none
//*/
//void E_AdcDMA::dmaStart()
//{
//	__IO uint32_t backup_setting_adc_dma_transfer = 0;  // ����DMA����

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
// *@brief    �������ݣ����ڷ�����ת������µ���
// *@param    none
// *@retval   none
//*/
//void E_AdcDMA::update(void){
//	// ����ADC,���ȴ�����
//	LL_ADC_REG_StartConversion(ADC1);
//	while (!LL_DMA_IsActiveFlag_TC1(DMA1));
//}
