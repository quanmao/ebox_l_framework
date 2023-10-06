/**
  ******************************************************************************
  * @file    example.cpp
  * @author  cat_li
  * @brief   ebox application example .
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
/**
 * -LQM (2017/7/12)
 *     ע�⣺�˲��ֺ�F1ϵ�в�ͨ�ã��ṩ��2��ADCģʽ��һ���ǵ�ͨ���βɼ���������DMA
 *  һ�������ɼ���ָ��������������ͨ�����ͨ���� 
 *  ��ͨ�����βɼ�,�ο���ѹ������ebox_analog.h�� VDDA_APPLI 
 *    1  ��������   E_Analog adc1(new E_PinBase(PA_0));
 *    2  �ṩread()��getVoltage()����
 */

#include "ebox.h"
#include "bsp_ebox.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 ANALOG example"
#define EXAMPLE_DATE	"2017-07-24"
#define DEMO_VER			"1.0"


E_ADC adc1(&PA0);
E_ADC intRef(E_ADC::ch_intervRef);
E_ADC tem(E_ADC::ch_temperature);
//#if defined(ADC_CCR_VBATEN)
//E_ADC vbat(battery_ch);
//#endif
E_ADC ad2(&PA1);


// E_AnalogDMA adcs(new E_PinBase(PA_0));

void setup()
{
    eboxInit();
		uart1.begin(115200);
}

void test(void)
{
		uint16_t i=0,j=0;
    while(1)
    {
			i = adc1.read();
			j = intRef.read();
			j = calcInterCalRef(j);	
			i = calcVoltage(i,j);
			uart1.printf("ch1 vol = %d mv  ref vol = %d mv  ",i,j);
			i = tem.read();
			i = calcTemperature(i,j);
			uart1.printf("tem = %d ",i);
			i= ad2.read();
			i = calcVoltage(i,j);
			uart1.printf("ch2 read = %d mv \r\n",i);
		delayMs(800);
#if defined(ADC_CCR_VBATEN)
		usart.printf("ch1 = %d ",vbat.getVoltage()*2);
		delay_ms(800);
#endif
//		usart.printf("ch2 = %f \r\n",tem.getTemperature());
    delayMs(2000);
		 }
}
