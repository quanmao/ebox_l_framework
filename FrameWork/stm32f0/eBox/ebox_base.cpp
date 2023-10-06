/**
  ******************************************************************************
  * @file    ebox_base.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/8
  * @brief   ���ļ�Ϊebox�Ļ����ļ�����Ҫ�ṩʱ�ӳ�ʼ����systick����ʱ��mcu��Ϣ��ȡ��
				�����ر�����ʱ��
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


volatile uint32_t gMs;     //�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
static uint8_t gMicroPara; //����us�ã����ټ�����

/**
	*@brief    ����clock,Ĭ��ʹ��HSI��48M
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
	*@brief    ��ʼ��������ʱ�ӣ�systemtick��systemtick callback������jtag
	*@param    mcu
	*@retval   none
	*/
void eboxInit(void)
{
    Clock_t clock;

    eboxClockConfig();
    eboxGetClockInfo(&clock);

    SysTick_Config(clock.core / 1000);                  //  ÿ�� 1ms����һ���ж�
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK); //systemticks clock��
    gMicroPara = clock.core / 1000000;                  //����micros����������

    attachSystickCallBack(nullFun);
}

void eboxReset(void)
{
    NVIC_SystemReset();
}

/**
	*@brief    ��ȡus
	*@param    mcu
	*@retval   none
	*/
uint64_t eboxGetUs(void)
{
    uint64_t micro;
    //        uint32_t temp = __get_PRIMASK();//����֮ǰ�ж�����
    //        no_interrupts();
    micro = (eboxGetMs() * 1000 + (1000 - (SysTick->VAL) / (gMicroPara)));
    //        __set_PRIMASK(temp);//�ָ�֮ǰ�ж�����

    return micro;
}

/**
	*@brief    ��ȡms
	*@param    mcu
	*@retval   none
	*/
uint32_t eboxGetMs(void)
{
    if (SysTick->CTRL & (1 << 16)) // COUNTFLAG == 1,���������
    {
        if (__get_IPSR() || (__get_PRIMASK())) //ȫ���ж�(get_PRIMASK)�����λ��ߵ�ǰ�����жϱ�����
            gMs++;
    }
    return gMs;
}

/**
	*@brief    ms��ʱ
	*@param    uint32_t ms  Ҫ��ʱ��ʱ������С1ms
	*@retval   none
	*/
void delayMs(uint32_t ms)
{
    uint32_t end = ENDTIME(ms);
    while (eboxGetMs() < end)
        ;
}
/**
	*@brief    us��ʱ,ʹ��systick��������48Mhz������ʱ��ʱ��������us(1.3)���ȡ�
	*          8Mhzʱ��С6-7us,24Mhzʱ��С2.2us,16Mhzʱ��С3.5us
	*@param    uint32_t us  Ҫ��ʱ��ʱ������С1us
	*@retval   none
	*/
void delayUs(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
		if(us == 0) return;
    ticks = (us - 1) * gMicroPara; /* �������� */
    tcnt = 0;
    told = SysTick->VAL; /* ���浱ǰ����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;
        tcnt += (tnow < told) ? (told - tnow) : (SysTick->LOAD - tnow + told);
        if (tcnt >= ticks)
            break;
        told = tnow;
    }
}

// systick �жϻص�����ָ�룬
fun_noPara_t callBackFun;
static uint16_t _multiple = 1;

/**
	*@brief    ע���жϻص�����������ָ���������� = �ж����� * multiple
	*@param    SystickCallBack_T fun �ص��������޲Σ�, uint8_t multiple �����������趨��������
	*@retval   E_OK  ע��ɹ��� E_NG ע��ʧ��
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
	*@brief    ��ȡϵͳʱ��
	*@param    *clock��  ʱ��ָ�룬����ϵͳʱ��
	*@retval   none
	*/
void eboxGetClockInfo(Clock_t *clock)
{
	// ����ϵͳ��ȡ��ֱ�Ӹ�ֵ�ķ�ʽ���Լ��ٴ�Լ800byte��code
    LL_RCC_ClocksTypeDef RCC_Clock;

    SystemCoreClockUpdate();
    LL_RCC_GetSystemClocksFreq(&RCC_Clock);

    clock->core = RCC_Clock.SYSCLK_Frequency;
    clock->hclk = RCC_Clock.HCLK_Frequency;
    clock->pclk2 = 0;
    clock->pclk1 = RCC_Clock.PCLK1_Frequency;
}

/**
	*@brief    ��ȡϵͳ��Ϣ��CPU_ID,flashsize
	*@param    none
	*@retval   none
	*/
void eboxGetChipInfo(CpuInfo_t *cpu)
{
    //    cpu.type = MCU_TYPE;
    //    memcpy(cpu.company, MCU_COMPANY, sizeof(MCU_COMPANY));
    cpu->chip_id[2] = (uint32_t)(READ_REG(*((uint32_t *)UID_BASE_ADDRESS))); //���ֽ�
    cpu->chip_id[1] = (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 4U))));
    cpu->chip_id[0] = (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 8U)))); //���ֽ�

    cpu->flash_size = (uint16_t)(READ_REG(*((uint32_t *)FLASHSIZE_BASE_ADDRESS))); //оƬflash����
#if EBOX_DEBUG
    cpu->ability = 0;
    gMs = 0;
    SysTick->VAL = 0;
    //ͳ��cpu����������1ms�ڿ����ԼӵĴ���//////////////////
    do
    {
        cpu->ability++; //ͳ��cpu��������
    } while (gMs < 1);
//    cpu->ability = cpu->ability * 1000 * 2;
////////////////////////////////
#endif
}

///**
// *@brief    cpu����,���ں�ֹͣ�����豣�����С�ע��:sleep_on_exti���жϻ��Ѻ�ִ�����жϻ��ٴν���sleepģʽ
// *@param    uint8_t isSleepnow: 0 sleep_on_extiģʽ, 1 sleep nowģʽ
//            uint8_t entry: 0 wfi,�����жϻ���, 1 wfe,�����¼�����
//            Ĭ��Ϊ1 sleep now  0 wfi ģʽ          
// *@retval   none
//*/
//void eboxSleep(sleep_t isSleepnow, entry_t entry)
//{
//    DEBUG("entry sleep mode !");
//    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
//    // ��Ҫ�ر�systick�жϣ���ֹ������
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
//    // ���Ѻ�ָ�sysTick�ж�
//    LL_SYSTICK_EnableIT();
//    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
//    DBG(" || wake up from sleep mode !\r\n");
//}

///**
// *@brief    1.8v������������ʱ��ֹͣ,SRAM�ͼĴ������ݱ���,����IO����������ģʽ״̬
// *@param    uint8_t isSleepnow: 0 sleep_on_extiģʽ, 1 sleep nowģʽ
//            uint8_t entry: 0 wfi,�����ⲿ�жϻ���, 1 wfe,�����ⲿ�¼�����
//            Ĭ��Ϊ1 sleep now  0 wfi ģʽ            
// *@retval   none
//*/
//void eboxStop(sleep_t isSleepnow, entry_t entry)
//{
//    DEBUG("entry stop mode !");
//    // ��Ҫ����PWRʱ�ӣ����ܹ�дPWR�Ĵ���
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
// *@brief    ֻ�б��ݼĴ����ʹ�����·�������˳����൱������
// *@param    uint32_t wakeUpPin: LL_PWR_WAKEUP_PIN1,LL_PWR_WAKEUP_PIN2 �ο�stm32f0xx_ll_pwr.h����                     
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
 *@brief    �����豸��ַ������ر�ʱ��,���ļ��Կͻ�����,����ͷ�ļ���û�иú����ӿڣ������Ҫʹ�ã�����
						��Ӧλ�ü��� extern void eboxRccCmd(const DevToRcc_t *devtable, uint32_t dev, FunctionalState state)
 *@param    const DevToRcc_t *devtable	�豸��ַ��ʱ����Ϣ��Ӧ��
						dev  :  �豸��ַ����(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1�ȵ�
            state:  ENABLE(����ʱ��) ���� DISABLE(�ر�ʱ��)
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
     *@brief    systick�жϴ�����
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
