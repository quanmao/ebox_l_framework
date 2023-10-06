/**
  ******************************************************************************
  * @file    ebox_base.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/8
  * @brief   此文件为ebox的基础文件，主要提供时钟初始化，systick，延时，mcu信息获取。
				开启关闭外设时钟
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "ebox_type.h"
#include "ebox_config.h"
#include "mcu_define.h"
#include "ebox_base.h"

#if EBOX_DEBUG
#include "bsp_ebox.h"
#define  _DEBUG false
#endif

#if _DEBUG
#define  DEBUG(...) DBG("[CORE]  "),DBG(__VA_ARGS__)
#else
#define  DEBUG(...)
#endif


volatile uint32_t gMs;     //提供一个mills()等效的全局变量。降低cpu调用开销
static uint8_t gMicroPara; //计算us用，减少计算量

/**
	*@brief    配置clock,默认使用HSI，48M
	*@param    mcu
	*@retval   none
	*/
__WEAK void eboxClockConfig()
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

//    if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
//    {
//    }
    LL_RCC_HSI_Enable();

    /* Wait till HSI is ready */
    while (LL_RCC_HSI_IsReady() != 1)
    {
    }
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1)
    {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
}

/**
	*@brief    初始化，设置时钟，systemtick，systemtick callback，禁用jtag
	*@param    mcu
	*@retval   none
	*/
void eboxInit(void)
{
    Clock_t clock;

    eboxClockConfig();
    eboxGetClockInfo(&clock);

    SysTick_Config(clock.core / 1000);                  //  每隔 1ms产生一次中断
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK); //systemticks clock；
    gMicroPara = clock.core / 1000000;                  //减少micros函数计算量

    attachSystickCallBack(nullFun);
}

void eboxReset(void)
{
    NVIC_SystemReset();
}

/**
	*@brief    获取us
	*@param    mcu
	*@retval   none
	*/
uint64_t eboxGetUs(void)
{
    uint64_t micro;
    //        uint32_t temp = __get_PRIMASK();//保存之前中断设置
    //        no_interrupts();
    micro = (eboxGetMs() * 1000 + (1000 - (SysTick->VAL) / (gMicroPara)));
    //        __set_PRIMASK(temp);//恢复之前中断设置

    return micro;
}

/**
	*@brief    获取ms
	*@param    mcu
	*@retval   none
	*/
uint32_t eboxGetMs(void)
{
    if (SysTick->CTRL & (1 << 16)) // COUNTFLAG == 1,发生了溢出
    {
        if (__get_IPSR() || (__get_PRIMASK())) //全局中断(get_PRIMASK)被屏蔽或者当前程序中断被屏蔽
            gMs++;
    }
    return gMs;
}

/**
	*@brief    ms延时
	*@param    uint32_t ms  要延时的时长，最小1ms
	*@retval   none
	*/
void delayMs(uint32_t ms)
{
    uint32_t end = ENDTIME(ms);
    while (eboxGetMs() < end)
        ;
}
/**
	*@brief    us延时,使用systick计数器。48Mhz及以上时钟时可以满足us(1.3)精度。
	*          8Mhz时最小6-7us,24Mhz时最小2.2us,16Mhz时最小3.5us
	*@param    uint32_t us  要延时的时长，最小1us
	*@retval   none
	*/
void delayUs(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
		if(us == 0) return;
    ticks = (us - 1) * gMicroPara; /* 计数周期 */
    tcnt = 0;
    told = SysTick->VAL; /* 保存当前计数值 */

    while (1)
    {
        tnow = SysTick->VAL;
        tcnt += (tnow < told) ? (told - tnow) : (SysTick->LOAD - tnow + told);
        if (tcnt >= ticks)
            break;
        told = tnow;
    }
}

// systick 中断回调函数指针，
fun_noPara_t callBackFun;
static uint16_t _multiple = 1;

/**
	*@brief    注册中断回调函数，可以指定调用周期 = 中断周期 * multiple
	*@param    SystickCallBack_T fun 回调函数（无参）, uint8_t multiple 倍数。用来设定调用周期
	*@retval   E_OK  注册成功， E_NG 注册失败
	*/
void attachSystickCallBack(fun_noPara_t fun, uint16_t multiple)
{

//    if (callBackFun == NULL || callBackFun == nullFun)
//    {
        callBackFun = fun;
        _multiple = (multiple == 0) ? 1 : multiple;
//        return EOK;
//    }
//    else
//    {
//        return ENG;
//    }
}

/**
	*@brief    获取系统时钟
	*@param    *clock：  时钟指针，返回系统时钟
	*@retval   none
	*/
void eboxGetClockInfo(Clock_t *clock)
{
	// 不从系统获取，直接赋值的方式可以减少大约800byte的code
    LL_RCC_ClocksTypeDef RCC_Clock;

    SystemCoreClockUpdate();
    LL_RCC_GetSystemClocksFreq(&RCC_Clock);

    clock->core = RCC_Clock.SYSCLK_Frequency;
    clock->hclk = RCC_Clock.HCLK_Frequency;
    clock->pclk2 = 0;
    clock->pclk1 = RCC_Clock.PCLK1_Frequency;
}

/**
	*@brief    获取系统信息，CPU_ID,flashsize
	*@param    none
	*@retval   none
	*/
void eboxGetChipInfo(CpuInfo_t *cpu)
{
    //    cpu.type = MCU_TYPE;
    //    memcpy(cpu.company, MCU_COMPANY, sizeof(MCU_COMPANY));
    cpu->chip_id[2] = (uint32_t)(READ_REG(*((uint32_t *)UID_BASE_ADDRESS))); //低字节
    cpu->chip_id[1] = (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 4U))));
    cpu->chip_id[0] = (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 8U)))); //高字节

    cpu->flash_size = (uint16_t)(READ_REG(*((uint32_t *)FLASHSIZE_BASE_ADDRESS))); //芯片flash容量
#if EBOX_DEBUG
    cpu->ability = 0;
    gMs = 0;
    SysTick->VAL = 0;
    //统计cpu计算能力，1ms内可以自加的次数//////////////////
    do
    {
        cpu->ability++; //统计cpu计算能力
    } while (gMs < 1);
//    cpu->ability = cpu->ability * 1000 * 2;
////////////////////////////////
#endif
}

///**
// *@brief    cpu休眠,仅内核停止，外设保持运行。注意:sleep_on_exti被中断唤醒后执行完中断会再次进入sleep模式
// *@param    uint8_t isSleepnow: 0 sleep_on_exti模式, 1 sleep now模式
//            uint8_t entry: 0 wfi,任意中断唤醒, 1 wfe,任意事件唤醒
//            默认为1 sleep now  0 wfi 模式          
// *@retval   none
//*/
//void eboxSleep(sleep_t isSleepnow, entry_t entry)
//{
//    DEBUG("entry sleep mode !");
//    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
//    // 需要关闭systick中断，防止被唤醒
//    LL_SYSTICK_DisableIT();
//    /* Set STOP_LPREGU mode when CPU enters deepsleep */
//    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_MAINREGU);
//    /* Set SLEEPDEEP bit of Cortex System Control Register */
//    LL_LPM_EnableSleep();
//    if (isSleepnow)
//    {
//        DBG(" || sleepNow");
//        LL_LPM_DisableSleepOnExit();
//        /* Request Wait For Interrupt */
//        if (entry == 0)
//        {
//            DBG(" || wfi mode");
//            __WFI();
//        }
//        else
//        {
//            DBG(" || wfe mode");
//            __SEV();
//            __WFE();
//            __WFE();
//        }
//    }
//    else
//    {
//        DBG(" || sleep_on_exti");
//        LL_LPM_EnableSleepOnExit();
//        DBG(" || wfi mode");
//        __WFI();
//    }
//    // 唤醒后恢复sysTick中断
//    LL_SYSTICK_EnableIT();
//    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
//    DBG(" || wake up from sleep mode !\r\n");
//}

///**
// *@brief    1.8v供电区域所有时钟停止,SRAM和寄存器内容保留,所有IO保持在运行模式状态
// *@param    uint8_t isSleepnow: 0 sleep_on_exti模式, 1 sleep now模式
//            uint8_t entry: 0 wfi,任意外部中断唤醒, 1 wfe,任意外部事件唤醒
//            默认为1 sleep now  0 wfi 模式            
// *@retval   none
//*/
//void eboxStop(sleep_t isSleepnow, entry_t entry)
//{
//    DEBUG("entry stop mode !");
//    // 需要开启PWR时钟，才能够写PWR寄存器
//    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
//    /* Set STOP_LPREGU mode when CPU enters deepsleep */
//    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_LPREGU);
//    /* Set SLEEPDEEP bit of Cortex System Control Register */
//    LL_LPM_EnableDeepSleep();
//    /* Request Wait For Interrupt */
//    if (isSleepnow)
//    {
//        DBG(" || stopNow");
//        LL_LPM_DisableSleepOnExit();
//        /* Request Wait For Interrupt */
//        if (entry == 0)
//        {
//            DBG(" || wfi mode");
//            __WFI();
//        }
//        else
//        {
//            DBG(" || wfe mode");
//            __SEV();
//            __WFE();
//            __WFE();
//        }
//    }
//    else
//    {
//        DBG(" || sleep_on_exti");
//        LL_LPM_EnableSleepOnExit();
//        DBG(" || wfi mode");
//        __WFI();
//    }
//    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
//    eboxClockConfig();
//    DBG(" || wake up from stop mode !\r\n");
//}

///**
// *@brief    只有备份寄存器和待机电路工作，退出后相当与重启
// *@param    uint32_t wakeUpPin: LL_PWR_WAKEUP_PIN1,LL_PWR_WAKEUP_PIN2 参考stm32f0xx_ll_pwr.h定义                     
// *@retval   none
//*/
//void eboxStandby(uint16_t wakeUpPin)
//{
//    DEBUG("entry standby mode !");
//    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
//    /* Disable all used wakeup sources */
//    LL_PWR_DisableWakeUpPin(wakeUpPin);

//    /* Clear all wake up Flag */
//    LL_PWR_ClearFlag_WU();

//    /* Enable wakeup pin */
//    LL_PWR_EnableWakeUpPin(wakeUpPin);
//    /* Set STANDBY mode when CPU enters deepsleep */
//    LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
//    /* Set SLEEPDEEP bit of Cortex System Control Register */
//    LL_LPM_EnableDeepSleep();
//    LL_LPM_EnableSleepOnExit();
//    /* Request Wait For Interrupt */
//    __WFI();
//}

/**
 *@name     void eboxRccCmd(const DevToRcc_t *devtable, uint32_t dev, FunctionalState state)
 *@brief    根据设备地址开启或关闭时钟,此文件对客户隐藏,所以头文件中没有该函数接口，如果需要使用，请在
						相应位置加入 extern void eboxRccCmd(const DevToRcc_t *devtable, uint32_t dev, FunctionalState state)
 *@param    const DevToRcc_t *devtable	设备地址与时钟信息对应表
						dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            state:  ENABLE(开启时钟) 或者 DISABLE(关闭时钟)
 *@retval   NONE
*/
void eboxRccCmd(const DevToRcc_t *devtable, uint32_t dev, FunctionalState state)
{
		uint8_t i = 0;
		
		while(devtable[i].dev != 0)
		{
			if(devtable[i].dev == dev) break;
			i++;
		}
    if(dev >= AHBPERIPH_BASE)
    {
        state ? LL_AHB1_GRP1_EnableClock(devtable[i].rcc) : LL_AHB1_GRP1_DisableClock(devtable[i].rcc);
    }
    else if(dev >= APBPERIPH_BASE + 0x00010000)
    {
        state ? LL_APB1_GRP2_EnableClock(devtable[i].rcc) : LL_APB1_GRP2_DisableClock(devtable[i].rcc);
    }
    else if(dev >= APBPERIPH_BASE)
    {
        state ? LL_APB1_GRP1_EnableClock(devtable[i].rcc) : LL_APB1_GRP1_DisableClock(devtable[i].rcc);
    }
}

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     *@brief    systick中断处理函数
     *@param    none
     *@retval   none
    */
    void SysTick_Handler(void)
    {
        if (gMs++ % _multiple == 0)
        {
            callBackFun();
        }
    }
#ifdef __cplusplus
}
#endif
