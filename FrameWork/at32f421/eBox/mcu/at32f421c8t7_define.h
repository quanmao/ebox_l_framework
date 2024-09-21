/******************************************************************************
程 序 名： at32f421c8t7_define.h　
编 写 人： cat_li
编写时间： 2019年06月16日
接口说明： at32f421c8t7外设定义.16kRAM,64kFLASH	
修改日志：　　
******************************************************************************/

#ifndef __AT32F421C8T7_DEFINE_H_
#define __AT32F421C8T7_DEFINE_H_

#include "at32f421_conf.h"
//#include "stm32f0xx_ll_bus.h"
//#include "stm32f0xx_ll_gpio.h"
//#include "stm32f0xx_ll_tim.h"
//#include "stm32f0xx_ll_DMA.h"
//#include "stm32f0xx_ll_EXTI.h"
//#include "stm32f0xx_ll_adc.h"
//// #include "stm32f0xx_ll_cortex.h"
//// #include "stm32f0xx_ll_pwr.h"
#include "mcu_define.h"
#include "ebox_config.h"

//外设->时钟源查询表,相关信息可在at32f421_crm.h中查找
static const DevToRcc_t RccTableGPIO[] = {
		{GPIOA_BASE,CRM_GPIOA_PERIPH_CLOCK},
    {GPIOB_BASE,CRM_GPIOB_PERIPH_CLOCK},
#ifdef GPIOC_BASE
    {GPIOC_BASE,CRM_GPIOC_PERIPH_CLOCK},
#endif
#ifdef GPIOD_BASE
    {GPIOD_BASE, LL_AHB1_GRP1_PERIPH_GPIOD},
#endif
#ifdef GPIOE_BASE
    {GPIOE_BASE, LL_AHB1_GRP1_PERIPH_GPIOE},
#endif
#ifdef GPIOF_BASE
   {GPIOF_BASE,CRM_GPIOF_PERIPH_CLOCK},
#endif
#ifdef GPIOG_BASE
    {GPIOG_BASE, LL_AHB1_GRP1_PERIPH_GPIOG},
#endif
#ifdef GPIOH_BASE
    {GPIOH_BASE, LL_AHB1_GRP1_PERIPH_GPIOH},
#endif
    0};

static const DevToRcc_t RccTableUSART[] = {
   {USART1_BASE, CRM_USART1_PERIPH_CLOCK},
	 {USART2_BASE, CRM_USART2_PERIPH_CLOCK},
   0};

//static const DevToRcc_t RccTableSPI[] = {
//   {SPI1_BASE, LL_APB1_GRP2_PERIPH_SPI1},

//    0};

//static const DevToRcc_t RccTableTIM[] = {
//#if (USE_TIM1 && defined TIM1_BASE)
//    {TIM1_BASE, LL_APB1_GRP2_PERIPH_TIM1},
//#endif
//#if (USE_TIM2 && defined TIM2_BASE)
//    {TIM2_BASE, LL_APB1_GRP1_PERIPH_TIM2},
//#endif
//#if (USE_TIM3 && defined TIM3_BASE)
//    {TIM3_BASE, LL_APB1_GRP1_PERIPH_TIM3},
//#endif
//#if (USE_TIM4 && defined TIM4_BASE)
//    {TIM4_BASE, LL_APB1_GRP1_PERIPH_TIM4},
//#endif
//#if (USE_TIM5 && defined TIM5_BASE)
//    {TIM5_BASE, LL_APB1_GRP1_PERIPH_TIM5},
//#endif
//#if (USE_TIM16 && defined TIM16_BASE)
//    {TIM16_BASE, LL_APB1_GRP2_PERIPH_TIM16},
//#endif
//#if (USE_TIM17 && defined TIM17_BASE)
//    {TIM17_BASE, LL_APB1_GRP2_PERIPH_TIM17},
//#endif
//    0};


//		
//static const DevToRcc_t RccTableI2C[] = {
//    {I2C1_BASE, LL_APB1_GRP1_PERIPH_I2C1},
//	0
//};
//		
////static const DevToRcc_t dev_to_rcc_table[] = {
////    {SPI1_BASE, LL_APB1_GRP2_PERIPH_SPI1},
////    {I2C1_BASE, LL_APB1_GRP1_PERIPH_I2C1},
////    {DMA1_BASE, LL_AHB1_GRP1_PERIPH_DMA1},
////};

static const DevToIRQn_t IrqTableUSART[] = {
#if (USE_UART1 && defined USART1_BASE)
   {USART1_BASE, USART1_IRQn},
#endif
#if (USE_UART2 && defined USART2_BASE)
   {USART2_BASE, USART2_IRQn},
#endif
   0};

//static const DevToIRQn_t IrqTableTIM[] = {
//    {TIM1_BASE, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn},
//    {TIM3_BASE, TIM3_IRQn},
//    0};

//static const AF_FUN_S ADC_MAP[] = {
//	PA0_ID,AIN,0,LL_ADC_CHANNEL_0,
//	PA1_ID,AIN,0,LL_ADC_CHANNEL_1,
//	PA2_ID,AIN,0,LL_ADC_CHANNEL_2,
//	PA3_ID,AIN,0,LL_ADC_CHANNEL_3,
//	PA4_ID,AIN,0,LL_ADC_CHANNEL_4,
//	PA5_ID,AIN,0,LL_ADC_CHANNEL_5,
//	PA6_ID,AIN,0,LL_ADC_CHANNEL_6,
//	PA7_ID,AIN,0,LL_ADC_CHANNEL_7,

//	PB1_ID,AIN,0,LL_ADC_CHANNEL_9,

//	P_NC
//};
//// PA2,9,14 tx; PA3,10 rx
static const AF_FUN_S UART_MAP[] = {
    PA2_ID, AF_PP_PU, GPIO_MUX_1, USART2_BASE,
    PA3_ID, AF_PP_PU, GPIO_MUX_1, USART2_BASE,
    PA9_ID, AF_PP, GPIO_MUX_1, USART1_BASE,
    PA10_ID, AF_PP, GPIO_MUX_1, USART1_BASE,
    P_NC};

//// PA9,PB6 scl,PA10,PB7 SDA
//static const AF_FUN_S I2C_MAP[] = {
//    // i2c1
//    PA9_ID, AF_PP_PU, LL_GPIO_AF_4, I2C1_BASE,
//    PA10_ID, AF_PP_PU, LL_GPIO_AF_4, I2C1_BASE,
//	  PB6_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
//    PB7_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
//    (PinId_t)0xff};

//// sck,miso,mosi
//static const AF_FUN_S SPI_MAP[] = {
//    PA5_ID, AF_PP_PU, 0, SPI1_BASE,
//    PA6_ID, AF_PP_PU, 0, SPI1_BASE,
//    PA7_ID, AF_PP_PU, 0, SPI1_BASE,
//    PB3_ID, AF_PP_PU, 0, SPI1_BASE,
//	  PB4_ID, AF_PP_PU, 0, SPI1_BASE,
//    PB5_ID, AF_PP_PU, 0, SPI1_BASE,
//    P_NC};

//#define TIMxCH1 0x00
//#define TIMxCH2 0x01
//#define TIMxCH3 0x02
//#define TIMxCH4 0x03
//static const AF_FUN_S TIM_MAP[] =
//{
//    // TIM3,CH1,CH2,CH4
//    PA6_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH1,
//    PA7_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH2,
//    PB1_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH4,
//    // TIM16,CH1
//    PA6_ID, AF_PP, LL_GPIO_AF_5, TIM16_BASE + TIMxCH1,
//    // TIM17,CH1
//    PA7_ID, AF_PP, LL_GPIO_AF_5, TIM17_BASE + TIMxCH1,
//    // TIM14,CH1
//    PA4_ID, AF_PP, LL_GPIO_AF_4, TIM14_BASE + TIMxCH1,
//    // TIM1,CH2,CH3
//    PA8_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH1,
//    PA9_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH2,
//    PA10_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH3,
//    (PinId_t)0xff
//};

#endif
