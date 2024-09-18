/**
  ******************************************************************************
  * @file    ebox_base.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/8
  * @brief   此文件为ebox的基础文件，主要提供时钟初始化，systick，延时，mcu信息获取。
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_BASE_H
#define __EBOX_BASE_H

//#include "stm32f0xx_ll_rcc.h"
//#include "stm32f0xx_ll_utils.h"
//#include "stm32f0xx_ll_cortex.h"
//#include "stm32f0xx_ll_system.h"
#include "mcu_config.h"
#include "mcu_define.h"
#include "ebox_type.h"

#define ENABLE TRUE


__STATIC_INLINE void nullFun(void) {}         // 空函数，不做任何操作，用来给函数指针赋值，防止函数指针 = null时被调用，出错
typedef void (*fun_noPara_t)(void);          //无参函数指针
typedef void (*fun_onePara_t)(unsigned int); //单参数函数指针
	
	

///////全局变量、函数///////////////////////////////////////////////
extern volatile uint32_t gMs; //提供一个eboxGetMs()等效的全局变量。降低cpu调用开销

// 初始化
extern void eboxInit(void); //ebox系统初始化
// MCU相关
extern void eboxClockConfig(void);            //外部实现该函数，可以修改时钟源以及时钟频率。默认工作在48Mhz
extern void eboxGetClockInfo(Clock_t *clock); // 获取时钟信息
extern void eboxGetChipInfo(CpuInfo_t *cpu);  // 获取mcu信息,包括flash size,chip_id
extern void eboxReset(void);
extern void eboxRccCmd(const DevToRcc_t *devtable, uint32_t dev, confirm_state state);
// 打开关闭所有中断
#define eboxDisableAllIrq   __disable_irq   // 关闭所有中断
#define eboxEnableAllIrq    __enable_irq    // 开启所有中断
// 时间相关接口
extern uint64_t eboxGetUs(void);  //获取S系统当前运行了多长时间。单位：us
extern uint32_t eboxGetMs(void);  //获取系统当前运行了多长时间。单位：ms
extern void delayMs(uint32_t ms); //延时n毫秒。可在任意情况下使用，包括禁止所有中断的情况。
extern void delayUs(uint32_t us); //延时n微秒。可在任意情况下使用，包括禁止所有中断的情况。
// systick中断回调函数
extern void attachSystickCallBack(fun_noPara_t fun, uint16_t multiple = 1); // 绑定中断回调函数，允许则定义周期 = multiple * 1ms; 最大65535

#define ENDTIME(timeOut) (gMs + timeOut)
// 超时,返回1 否则返回0   这里采用millis()获取millis_seconds,可以防止因为关闭中断导致程序死在延时函数里
#define TIMEOUT(endTime, delay) ((uint32_t)(endTime - eboxGetMs()) > delay)

/*----------------------------以下部分为电源管理----------------------------------*/

//// power manage
// typedef enum {
//  sleepOnExit,
//  sleepNow
// }sleep_t;

// typedef enum {
//  wfi,
//  wfe
// }entry_t;

// /* 仅关闭CPU时钟，所有外设仍运行，可以通过任何中断唤醒,IO保持休眠前状态 */
// extern void eboxSleep(sleep_t isSleepnow = sleepNow,entry_t entry = wfi);
// /* 1.8v域所有时钟关闭，SRAM和寄存器保存，可以通过任何外部中断/事件唤醒,
//    特定外设(CEC,USART,I2C)中断(需编程为唤醒模式),IO保持休眠前状态 */
// extern void eboxStop(sleep_t isSleepnow = sleepNow,entry_t entry = wfi); 
// /* 如果ebox_sleep,ebox_stop设置为sleep_on_exti模式,在中断中调用该函数可以退出,返回主循环 */
// __STATIC_INLINE void eboxExitSleepOnExti(){
//  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
//  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
// }

// /* wkup pin拉高,外部复位,rtc alarm,内部看门够可唤醒,默认PA0 */  
// extern void eboxStandby(uint16_t wakeUpPin = LL_PWR_WAKEUP_PIN1);
// /* 返回上次是否为standby模式，并清空标志位*/																			
// __STATIC_INLINE bool isWakeFromSB(){
// 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
// 		bool r = (READ_BIT(PWR->CSR, PWR_CSR_SBF) == (PWR_CSR_SBF));
// 		SET_BIT(PWR->CR, PWR_CR_CSBF);
//    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
// 		return r;
// }

// #define RST_LOWPOWER	0x80
// #define RST_WWDOG			0x40
// #define RST_IWDOG			0x20
// #define RST_SOFT			0x10
// #define RST_POWER			0x08
// #define RST_NRST			0x04
// #define RST_OBL				0x02

// // 默认返回值为0x0c,即RST_POWER|RST_NRST
// __STATIC_INLINE uint8_t eboxResetType(){
// 		return (RCC->CSR)>>24;
// }

// // 软件重启，执行后复位寄存器返回值为0x1c
// __STATIC_INLINE void ebox_rst(){
// 		NVIC_SystemReset();
// }
#endif
