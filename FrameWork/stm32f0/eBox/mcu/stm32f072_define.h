/******************************************************************************
程 序 名： stm32f072_define.h　
编 写 人： cat_li
编写时间： 2017年10月16日
接口说明： stm32f072Rx外设定义.
	2个spi,i2c,4个usart,1个adc(16+3ch),1个DAC(2ch),51gpios,9tim(6general(2,3,14,15,16,17),2base(6,7)1advance(1)16kRAM,128kFLASH
修改日志：　　
	NO.1-  初始版本
				2017/10/16 usart info 改用Periph_SS定义，信息中包含不同usart的时钟使能函数
******************************************************************************/

#ifndef __STM32F072_DEFINE_H_
#define __STM32F072_DEFINE_H_

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_DMA.h"
#include "stm32f0xx_ll_EXTI.h"
#include "stm32f0xx_ll_adc.h"
#include "mcu_define.h"
#include "ebox_config.h"
#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

// 以下信息可以在对应的头文件中查找到，ex stm32f072xb.h
//外设->时钟源查询表
static const DevToRcc_t RccTableGPIO[] = {
    {GPIOA_BASE, LL_AHB1_GRP1_PERIPH_GPIOA},
    {GPIOB_BASE, LL_AHB1_GRP1_PERIPH_GPIOB},
#ifdef GPIOC_BASE
    {GPIOC_BASE, LL_AHB1_GRP1_PERIPH_GPIOC},
#endif
#ifdef GPIOD_BASE
    {GPIOD_BASE, LL_AHB1_GRP1_PERIPH_GPIOD},
#endif
#ifdef GPIOE_BASE
    {GPIOE_BASE, LL_AHB1_GRP1_PERIPH_GPIOE},
#endif
#ifdef GPIOF_BASE
    {GPIOF_BASE, LL_AHB1_GRP1_PERIPH_GPIOF},
#endif
#ifdef GPIOG_BASE
    {GPIOG_BASE, LL_AHB1_GRP1_PERIPH_GPIOG},
#endif
#ifdef GPIOH_BASE
    {GPIOH_BASE, LL_AHB1_GRP1_PERIPH_GPIOH},
#endif
    0};

static const DevToRcc_t RccTableUSART[] = {
    {USART1_BASE, LL_APB1_GRP2_PERIPH_USART1},
    0};

static const DevToRcc_t RccTableSPI[] = {
   {SPI1_BASE, LL_APB1_GRP2_PERIPH_SPI1},

    0};

static const DevToRcc_t RccTableTIM[] = {
#if (USE_TIM1 && defined TIM1_BASE)
    {TIM1_BASE, LL_APB1_GRP2_PERIPH_TIM1},
#endif
#if (USE_TIM2 && defined TIM2_BASE)
    {TIM2_BASE, LL_APB1_GRP1_PERIPH_TIM2},
#endif
#if (USE_TIM3 && defined TIM3_BASE)
    {TIM3_BASE, LL_APB1_GRP1_PERIPH_TIM3},
#endif
#if (USE_TIM4 && defined TIM4_BASE)
    {TIM4_BASE, LL_APB1_GRP1_PERIPH_TIM4},
#endif
#if (USE_TIM5 && defined TIM5_BASE)
    {TIM5_BASE, LL_APB1_GRP1_PERIPH_TIM5},
#endif
    0};


static const DevToRcc_t RccTableI2C[] = {
    {I2C1_BASE, LL_APB1_GRP1_PERIPH_I2C1},
	0
};
		
static const DevToRcc_t dev_to_rcc_table[] = {		
		{TIM14_BASE, LL_APB1_GRP1_PERIPH_TIM14},
    {TIM15_BASE, LL_APB1_GRP2_PERIPH_TIM15},
		{TIM16_BASE, LL_APB1_GRP2_PERIPH_TIM16},
		{TIM17_BASE, LL_APB1_GRP2_PERIPH_TIM17},


    {SPI1_BASE, LL_APB1_GRP2_PERIPH_SPI1},
    {SPI2_BASE, LL_APB1_GRP1_PERIPH_SPI2},

    {I2C1_BASE, LL_APB1_GRP1_PERIPH_I2C1},
    {I2C2_BASE, LL_APB1_GRP1_PERIPH_I2C2},


    {USART1_BASE, LL_APB1_GRP2_PERIPH_USART1},
    {USART2_BASE, LL_APB1_GRP1_PERIPH_USART2},
    {USART3_BASE, LL_APB1_GRP1_PERIPH_USART3},
    {USART4_BASE, LL_APB1_GRP1_PERIPH_USART4},

    {DMA1_BASE, LL_AHB1_GRP1_PERIPH_DMA1},
};

static const DevToIRQn_t IrqTableUSART[] = {
#if (USE_UART1 && defined USART1_BASE)
    {USART1_BASE, USART1_IRQn},
#endif
#if (USE_UART2 && defined USART2_BASE)
    {USART2_BASE, USART2_IRQn},
#endif
#if (USE_UART3 && defined USART3_BASE)
    {USART3_BASE, USART3_IRQn},
#endif
#if (USE_UART4 && defined USART4_BASE)
    {USART4_BASE, USART4_IRQn},
#endif
#if (USE_UART5 && defined USART5_BASE)
    {USART5_BASE, USART5_IRQn},
#endif
    0};


static const DevToIRQn_t tim_irq_table[] = {
    {TIM1_BASE, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn},
    {TIM3_BASE, TIM3_IRQn},
    0};
//设备->中断号查询表

static const AF_FUN_S ADC_MAP[] = {
	PA0_ID,AIN,0,LL_ADC_CHANNEL_0,
	PA1_ID,AIN,0,LL_ADC_CHANNEL_1,
	PA2_ID,AIN,0,LL_ADC_CHANNEL_2,
	PA3_ID,AIN,0,LL_ADC_CHANNEL_3,
	PA4_ID,AIN,0,LL_ADC_CHANNEL_4,
	PA5_ID,AIN,0,LL_ADC_CHANNEL_5,
	PA6_ID,AIN,0,LL_ADC_CHANNEL_6,
	PA7_ID,AIN,0,LL_ADC_CHANNEL_7,

	PB1_ID,AIN,0,LL_ADC_CHANNEL_9,

	P_NC
};
const DevToIRQn_t dev_to_IRQn_table[] =
{
    {TIM1_BASE, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn},
    {TIM2_BASE, TIM2_IRQn},
    {TIM3_BASE, TIM3_IRQn},
		
		{TIM6_BASE, TIM6_DAC_IRQn},
		{TIM7_BASE, TIM7_IRQn},

		{TIM14_BASE, TIM14_IRQn},
		{TIM15_BASE, TIM15_IRQn},
		{TIM16_BASE, TIM16_IRQn},
		{TIM17_BASE, TIM17_IRQn},


    {SPI1_BASE, SPI1_IRQn},
    {SPI2_BASE, SPI2_IRQn},

    {I2C1_BASE, I2C1_IRQn},
    {I2C2_BASE, I2C2_IRQn},

    {LL_DMA_CHANNEL_1, DMA1_Channel1_IRQn},
    {LL_DMA_CHANNEL_2, DMA1_Channel2_3_IRQn},
    {LL_DMA_CHANNEL_3, DMA1_Channel2_3_IRQn},
    {LL_DMA_CHANNEL_4, DMA1_Channel4_5_6_7_IRQn},
    {LL_DMA_CHANNEL_5, DMA1_Channel4_5_6_7_IRQn},
    {LL_DMA_CHANNEL_6, DMA1_Channel4_5_6_7_IRQn},
    {LL_DMA_CHANNEL_7, DMA1_Channel4_5_6_7_IRQn},


    {USART1_BASE, USART1_IRQn},
    {USART2_BASE, USART2_IRQn},
    {USART3_BASE, USART3_4_IRQn},
    {USART4_BASE, USART3_4_IRQn},

    {LL_EXTI_LINE_0, EXTI0_1_IRQn},
    {LL_EXTI_LINE_1, EXTI0_1_IRQn},
    {LL_EXTI_LINE_2, EXTI2_3_IRQn},
    {LL_EXTI_LINE_3, EXTI2_3_IRQn},
    {LL_EXTI_LINE_4, EXTI4_15_IRQn},
    {LL_EXTI_LINE_5, EXTI4_15_IRQn},
    {LL_EXTI_LINE_6, EXTI4_15_IRQn},
    {LL_EXTI_LINE_7, EXTI4_15_IRQn},
    {LL_EXTI_LINE_8, EXTI4_15_IRQn},
    {LL_EXTI_LINE_9, EXTI4_15_IRQn},
    {LL_EXTI_LINE_10, EXTI4_15_IRQn},
    {LL_EXTI_LINE_11, EXTI4_15_IRQn},
    {LL_EXTI_LINE_12, EXTI4_15_IRQn},
    {LL_EXTI_LINE_13, EXTI4_15_IRQn},
    {LL_EXTI_LINE_14, EXTI4_15_IRQn},
    {LL_EXTI_LINE_15, EXTI4_15_IRQn},
};

// PA2,9,PA0,PB10 tx; PA3,10,PA1,PB11 rx
static const AF_FUN_S UART_MAP[] =
{
	  PA9_ID, AF_PP_PU, LL_GPIO_AF_1, USART1_BASE,
    PA10_ID, AF_PP_PU, LL_GPIO_AF_1, USART1_BASE,
	
		PB6_ID, AF_PP_PU, LL_GPIO_AF_0, USART1_BASE,
    PB7_ID, AF_PP_PU, LL_GPIO_AF_0, USART1_BASE,
	
    PA2_ID, AF_PP_PU, LL_GPIO_AF_1, USART2_BASE,
    PA3_ID, AF_PP_PU, LL_GPIO_AF_1, USART2_BASE,
	
		PB10_ID, AF_PP_PU, LL_GPIO_AF_4, USART3_BASE,
    PB11_ID, AF_PP_PU, LL_GPIO_AF_4, USART3_BASE,
	
	
	  PA0_ID, AF_PP_PU, LL_GPIO_AF_4, USART4_BASE,
    PA1_ID, AF_PP_PU, LL_GPIO_AF_4, USART4_BASE,
    P_NC
};

// PB9,PB6,PB10,PB13 scl,PB9,PB7,PB11,PB14 SDA
static const AF_FUN_S I2C_MAP[] =
{   // i2c1
    PB8_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
    PB9_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
	  PB6_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
    PB7_ID, AF_PP_PU, LL_GPIO_AF_1, I2C1_BASE,
	  PB10_ID, AF_PP_PU, LL_GPIO_AF_1, I2C2_BASE,
    PB11_ID, AF_PP_PU, LL_GPIO_AF_1, I2C2_BASE,
	  PB13_ID, AF_PP_PU, LL_GPIO_AF_5, I2C2_BASE,
    PB14_ID, AF_PP_PU, LL_GPIO_AF_5, I2C2_BASE,
	  
    (PinId_t)0xff
};

//PA5,PB3,PB13 sck,PA6,PB4,PB14 miso, PA7,PB5,PB15mosi
static const AF_FUN_S SPI_MAP[] =
{
    PA5_ID, AF_PP_PU, 0, SPI1_BASE,
    PA6_ID, AF_PP_PU, 0, SPI1_BASE,
    PA7_ID, AF_PP_PU, 0, SPI1_BASE,
    PB3_ID, AF_PP_PU, 0, SPI1_BASE,
	  PB4_ID, AF_PP_PU, 0, SPI1_BASE,
    PB5_ID, AF_PP_PU, 0, SPI1_BASE,
	  PB13_ID, AF_PP_PU, 0, SPI2_BASE,
	  PB14_ID, AF_PP_PU, 0, SPI2_BASE,
    PB15_ID, AF_PP_PU, 0, SPI2_BASE,
    P_NC
};

static const AF_FUN_S TIM_MAP[] =
{
    // TIM3,CH1,CH2,CH4
    PA6_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH1,
    PA7_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH2,
    PB1_ID, AF_PP, LL_GPIO_AF_1, TIM3_BASE + TIMxCH4,
    // TIM16,CH1
    PA6_ID, AF_PP, LL_GPIO_AF_5, TIM16_BASE + TIMxCH1,
    // TIM17,CH1
    PA7_ID, AF_PP, LL_GPIO_AF_5, TIM17_BASE + TIMxCH1,
    // TIM14,CH1
    PA4_ID, AF_PP, LL_GPIO_AF_4, TIM14_BASE + TIMxCH1,
    // TIM1,CH2,CH3
    PA8_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH1,
    PA9_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH2,
    PA10_ID, AF_PP, LL_GPIO_AF_2, TIM1_BASE + TIMxCH3,
    (PinId_t)0xff
};

#endif
