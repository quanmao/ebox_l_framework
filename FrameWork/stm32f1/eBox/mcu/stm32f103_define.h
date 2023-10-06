/******************************************************************************
程 序 名： stm32f103_define.h　
编 写 人： cat_li
编写时间： 2017年10月16日
接口说明： stm32f030f4外设定义.
	1个spi,i2c,usart,adc(11ch),15gpios,5tim(4general(3,14,16,17),1advance(1)4kRAM,16kFLASH
修改日志：　　
	NO.1-  初始版本
				2017/10/16 usart info 改用Periph_SS定义，信息中包含不同usart的时钟使能函数
******************************************************************************/

#ifndef __STM32F103_DEFINE_H_
#define __STM32F103_DEFINE_H_

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_EXTI.h"
#include "stm32f1xx_ll_adc.h"
#include "mcu_define.h"
#include "ebox_config.h"

//外设->时钟源查询表,相关信息可在stm32f1xx_ll_bus.h中查找
static const DevToRcc_t RccTableGPIO[] = {
    {GPIOA_BASE, LL_APB2_GRP1_PERIPH_GPIOA},
    {GPIOB_BASE, LL_APB2_GRP1_PERIPH_GPIOB},
#ifdef GPIOC_BASE
    {GPIOC_BASE, LL_APB2_GRP1_PERIPH_GPIOC},
#endif
#ifdef GPIOD_BASE
    {GPIOD_BASE, LL_APB2_GRP1_PERIPH_GPIOD},
#endif
#ifdef GPIOE_BASE
    {GPIOE_BASE, LL_APB2_GRP1_PERIPH_GPIOE},
#endif
#ifdef GPIOF_BASE
    {GPIOF_BASE, LL_APB2_GRP1_PERIPH_GPIOF},
#endif
#ifdef GPIOG_BASE
    {GPIOG_BASE, LL_APB2_GRP1_PERIPH_GPIOG},
#endif
#ifdef GPIOH_BASE
    {GPIOH_BASE, LL_APB2_GRP1_PERIPH_GPIOH},
#endif
    0};

static const DevToRcc_t RccTableUSART[] = {
#if (USE_UART1 && defined USART1_BASE)
    {USART1_BASE, LL_APB2_GRP1_PERIPH_USART1},
#endif
#if (USE_UART2 && defined USART2_BASE)
    {USART2_BASE, LL_APB1_GRP1_PERIPH_USART2},
#endif
#if (USE_UART3 && defined USART3_BASE)
    {USART3_BASE, LL_APB1_GRP1_PERIPH_USART3},
#endif
#if (USE_UART4 && defined USART4_BASE)
    {USART4_BASE, LL_APB1_GRP1_PERIPH_USART4},
#endif
#if (USE_UART5 && defined USART5_BASE)
    {USART5_BASE, LL_APB1_GRP1_PERIPH_USART5},
#endif
    0};

static const DevToRcc_t RccTableSPI[] = {
    {SPI1_BASE, LL_APB2_GRP1_PERIPH_SPI1},
    {SPI2_BASE, LL_APB1_GRP1_PERIPH_SPI2},
    0};

static const DevToRcc_t RccTableTIM[] = {
#if (USE_TIM1 && defined TIM1_BASE)
    {TIM1_BASE, LL_APB2_GRP1_PERIPH_TIM1},
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
    0};

static const DevToRcc_t dev_to_rcc_table[] = {
    {ADC1_BASE, LL_APB2_GRP1_PERIPH_ADC1},
    {I2C1_BASE, LL_APB1_GRP1_PERIPH_I2C1},
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

//设备->中断号查询表，中断号可在stm32f103xx.h中查找
static const DevToIRQn_t IrqTableEXTI[] = {
    {LL_EXTI_LINE_0, EXTI0_IRQn},
    {LL_EXTI_LINE_1, EXTI1_IRQn},
    {LL_EXTI_LINE_2, EXTI2_IRQn},
    {LL_EXTI_LINE_3, EXTI3_IRQn},
    {LL_EXTI_LINE_4, EXTI4_IRQn},
    {LL_EXTI_LINE_5, EXTI9_5_IRQn},
    {LL_EXTI_LINE_6, EXTI9_5_IRQn},
    {LL_EXTI_LINE_7, EXTI9_5_IRQn},
    {LL_EXTI_LINE_8, EXTI9_5_IRQn},
    {LL_EXTI_LINE_9, EXTI9_5_IRQn},
    {LL_EXTI_LINE_10, EXTI15_10_IRQn},
    {LL_EXTI_LINE_11, EXTI15_10_IRQn},
    {LL_EXTI_LINE_12, EXTI15_10_IRQn},
    {LL_EXTI_LINE_13, EXTI15_10_IRQn},
    {LL_EXTI_LINE_14, EXTI15_10_IRQn},
    {LL_EXTI_LINE_15, EXTI15_10_IRQn},
};

static const DevToIRQn_t tim_irq_table[] = {
    {TIM1_BASE, TIM1_UP_IRQn, TIM1_CC_IRQn},
    {TIM2_BASE, TIM2_IRQn},
    {TIM3_BASE, TIM3_IRQn},
    {TIM4_BASE, TIM4_IRQn},
    0};
const DevToIRQn_t dev_to_IRQn_table[] = {

    {SPI1_BASE, SPI1_IRQn},
    {I2C1_BASE, I2C1_EV_IRQn},

    //    {LL_DMA_CHANNEL_1, DMA1_Channel1_IRQn},
    //    {LL_DMA_CHANNEL_2, DMA1_Channel2_3_IRQn},
    //    {LL_DMA_CHANNEL_3, DMA1_Channel2_3_IRQn},
    //    {LL_DMA_CHANNEL_4, DMA1_Channel4_5_6_7_IRQn},
    //    {LL_DMA_CHANNEL_5, DMA1_Channel4_5_6_7_IRQn},
};
// pin mode 请查阅参考手册 Peripherals GPIO configurations
static const AF_FUN_S ADC_MAP[] = {
    PA0_ID, AIN, 0, LL_ADC_CHANNEL_0,
    PA1_ID, AIN, 0, LL_ADC_CHANNEL_1,
    PA2_ID, AIN, 0, LL_ADC_CHANNEL_2,
    PA3_ID, AIN, 0, LL_ADC_CHANNEL_3,
    PA4_ID, AIN, 0, LL_ADC_CHANNEL_4,
    PA5_ID, AIN, 0, LL_ADC_CHANNEL_5,
    PA6_ID, AIN, 0, LL_ADC_CHANNEL_6,
    PA7_ID, AIN, 0, LL_ADC_CHANNEL_7,

    PB1_ID, AIN, 0, LL_ADC_CHANNEL_9,

    P_NC};

// TX AF_PP, RX INPUT
static const AF_FUN_S UART_MAP[] = {
    PA9_ID, AF_PP, 0, USART1_BASE,
    PA10_ID, INPUT, 0, USART1_BASE,
    PA2_ID, AF_PP, 0, USART2_BASE,
    PA3_ID, INPUT, 0, USART2_BASE,
    P_NC};

// scl AF_OD,SDA AF_OD
static const AF_FUN_S I2C_MAP[] = {
    // i2c1
    //		PA9_ID,AF_PP_PU,LL_GPIO_AF_4,I2C1_BASE,
    //		PA10_ID,AF_PP_PU,LL_GPIO_AF_4,I2C1_BASE,
    (PinId_t)0xff};

//master sck AF_PP,miso INPUT,mosi AF_PP
static const AF_FUN_S SPI_MAP[] = {
    PA5_ID, AF_PP, 0, SPI1_BASE,
    PA6_ID, AF_PP, 0, SPI1_BASE,
    PA7_ID, AF_PP, 0, SPI1_BASE,

    PB13_ID, AF_PP, 0, SPI2_BASE,
    PB14_ID, AF_PP, 0, SPI2_BASE,
    PB15_ID, AF_PP, 0, SPI2_BASE,
    P_NC};

#define TIMxCH1 0x00
#define TIMxCH2 0x01
#define TIMxCH3 0x02
#define TIMxCH4 0x03
static const AF_FUN_S TIM_MAP[] = {
    // TIM2,CH1,CH2,CH3,CH4
    PA0_ID, AF_PP, 0, TIM2_BASE + TIMxCH1,
    PA1_ID, AF_PP, 0, TIM2_BASE + TIMxCH2,
    PA2_ID, AF_PP, 0, TIM2_BASE + TIMxCH3,
    PA3_ID, AF_PP, 0, TIM2_BASE + TIMxCH4,
    // TIM3,CH1,CH2,CH3,CH4
    PA6_ID, AF_PP, 0, TIM3_BASE + TIMxCH1,
    PA7_ID, AF_PP, 0, TIM3_BASE + TIMxCH2,
    PB0_ID, AF_PP, 0, TIM3_BASE + TIMxCH3,
    PB1_ID, AF_PP, 0, TIM3_BASE + TIMxCH4,
    // TIM4,CH1,CH2,CH3,CH4
    PB6_ID, AF_PP, 0, TIM4_BASE + TIMxCH1,
    PB7_ID, AF_PP, 0, TIM4_BASE + TIMxCH2,
    PB8_ID, AF_PP, 0, TIM4_BASE + TIMxCH3,
    PB9_ID, AF_PP, 0, TIM4_BASE + TIMxCH4,
    (PinId_t)0xff};

#endif
