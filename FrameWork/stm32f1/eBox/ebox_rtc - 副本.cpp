/**
  ******************************************************************************
  * @file    rtc.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_rtc.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_pwr.h"
#include "ebox_nvic.h"
#include "ebox_rtc.h"
#include "ebox_config.h"

#if USE_PRINTF
// 是否打印调试信息, 1打印,0不打印
#define debug 1
#endif

#if debug
#include "bsp_ebox.h"
//E_UART UART(USART1,&PA9,&PA10);
#define DBG UART.printf
#define DEBUG(...) DBG("[RTC]  "), DBG(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#define RTC_CFG_FLAG 0XA5A5

/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=40kHz RC */
#define LSI_ASYNCH_PREDIV          ((uint32_t)0x9C3F)

/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 1Hz with LSEFreq=32768Hz */
#define LSE_ASYNCH_PREDIV          ((uint32_t)0x7FFF)
Date_T gDate; // 全局变量，用来计算年月日
static uint32_t rtc_irq_id;
static rtc_irq_handler irq_handler;

void rtc_irq_init(rtc_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    rtc_irq_id = id;
}

/**
  * @brief  等待Time&Date寄存器与RTC(APB)时钟同步
  * @param  None
  * @retval E_OK,E_TIMEOUT
  */
int8_t ebox_WaitForSynchro_RTC(void)
{
    uint32_t end = ENDTIME(1000);
    /* Clear RSF flag */
    LL_RTC_ClearFlag_RS(RTC);
    /* Wait the registers to be synchronised */
    while (LL_RTC_IsActiveFlag_RS(RTC) != 1)
    {
        if (TIMEOUT(end, 1000))
        {
            DEBUG("时钟同步超时\r\n");
            return ETIMEOUT;
        }
    }
    return EOK;
}
/**
  * @brief  进入赋值模式
  * @note   该模式计数器停止，可以更新RTC值
  * @param  None
  * @retval E_OK,E_TIMEOUT
  */
int8_t ebox_Enter_RTC_InitMode(void)
{
    uint32_t end = ENDTIME(1000);
    uint8_t ret = EOK;
	    /* Disable the write protection for RTC registers */
    LL_RTC_DisableWriteProtection(RTC);
    /* Check if the Initialization mode is set */
    while (LL_RTC_IsActiveFlag_RTOF(RTC) != 1)
    {
        if (TIMEOUT(end, 1000))
        {
            DEBUG("init mode timeout,if use lse,please power on again！\r\n");
            ret = ETIMEOUT;
        }
    }

    return ret;
}

/**
  * @brief  退出赋值模式
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
int8_t ebox_Exit_RTC_InitMode(void)
{
    uint32_t end = ENDTIME(1000);
    LL_RTC_EnableWriteProtection(RTC);
    /* Check if the Initialization mode is set */
    while (LL_RTC_IsActiveFlag_RTOF(RTC) != 1)
    {
        if (TIMEOUT(end, 1000))
        {
            DEBUG("init mode timeout,if use lse,please power on again！\r\n");
            return ETIMEOUT;
        }
    }      
    return EOK;
}

static uint8_t _RTC_WeekDayNum(uint32_t nYear, uint8_t nMonth, uint8_t nDay)
{
    uint32_t year = 0, weekday = 0;

    year = 2000 + nYear;

    if (nMonth < 3)
    {
        /*D = { [(23 x month)/9] + day + 4 + year + [(year-1)/4] - [(year-1)/100] + [(year-1)/400] } mod 7*/
        weekday = (((23 * nMonth) / 9) + nDay + 4 + year + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400)) % 7;
    }
    else
    {
        /*D = { [(23 x month)/9] + day + 4 + year + [year/4] - [year/100] + [year/400] - 2 } mod 7*/
        weekday = (((23 * nMonth) / 9) + nDay + 4 + year + (year / 4) - (year / 100) + (year / 400) - 2) % 7;
    }

    //return (uint8_t)((weekday == 0) ? (7) : weekday);
    return (uint8_t)weekday;
}

/**
  * @brief  初始化RTC时钟
  * @param  ClockS clock 时钟源，clock_lse,clock_lsi
  * @retval ETIMEOUT 时钟配置失败,EOK 时钟配置成功,无需设置时钟，ENG 时钟配置成功，需要设置时钟
  */
int Rtc::begin(ClockS clock)
{
    int ret = ENG;
    /* Enable PWR and BKP clocks */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR | LL_APB1_GRP1_PERIPH_BKP);
    LL_PWR_EnableBkUpAccess();

    if (clock == clock_lse)
    {
        if ((_getTimeFlag() == 0) || (LL_RCC_LSE_IsReady() != 1)) //时钟掉电，需要重新设置
        {
            if (_config(clock) != EOK)
            {
                DEBUG("LSE startup failed, please check crystal\r\n");
                _config(clock_lsi);
            }
        }
        else
        {
#if debug
            /* Check if the Power On Reset flag is set */
            if (LL_RCC_IsActiveFlag_PORRST() != RESET)
            {
                DEBUG("Power On Reset occurred.... \r\n\n");
            }
            /* Check if the Pin Reset flag is set */
            else if (LL_RCC_IsActiveFlag_PINRST() != RESET)
            {
                DEBUG("External Reset occurred....\r\n\n ");
            }

            DEBUG("\r\n No need to configure RTC....");
            DEBUG("\r\n step 0....");
#endif
            /* Wait for RTC registers synchronization */
            ebox_WaitForSynchro_RTC();
            /* Enable the RTC Second */
            LL_RTC_EnableIT_SEC(RTC);
            /* Wait until last write operation on RTC registers has finished */
            while (!LL_RTC_IsActiveFlag_RTOF(RTC))
            {
            };
            ret = EOK;
        }
    }
    else
    {
        DEBUG("use lsi clock\r\n");
        ret = _config(clock_lsi);
    }
    rtc_irq_init(Rtc::_irq_handler, (uint32_t)this);
    return ret;
}

/**
  *@name     _config
  *@brief    配置时钟，分频器，时间格式
  *@param    ClockS clock 1 LSE,0 LSI
  *@retval   none
  */
int Rtc::_config(ClockS clock)
{
    uint32_t end = ENDTIME(5000);
    /* Enable PWR and BKP clocks */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR | LL_APB1_GRP1_PERIPH_BKP);
    LL_PWR_EnableBkUpAccess();

    /* Reset Backup Domain */
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    if (clock == clock_lse)
    {
        if (LL_RCC_LSE_IsReady() == 0)
        {
            // 使能LSE
            LL_RCC_LSE_Enable();
            while (LL_RCC_LSE_IsReady() != 1)
            {
                if (TIMEOUT(end, 5000))
                {
                    return ETIMEOUT;
                }
            }
            // 选择LSE作为时钟源
            LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
            /* Enable RTC Clock */
            LL_RCC_EnableRTC();
            DEBUG("LSE  started successfully!\r\n");
        }
    }
    else
    {
        LL_RCC_LSI_Enable();
        /* Wait till LSI is ready */
        while (LL_RCC_LSI_IsReady() != 1)
        {
            if (TIMEOUT(end, 5000))
            {
                return ETIMEOUT;
            }
        }
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
        /* Enable RTC Clock */
        LL_RCC_EnableRTC();
        _setTimeFlag(0xffff); //清除备份域标志
		 DEBUG("LSI started successfully!\r\n");
        }

    /* Wait for RTC registers synchronization */
    ebox_WaitForSynchro_RTC();
    /* Wait until last write operation on RTC registers has finished */
    //    RTC_WaitForLastTask();
    /* Enable the RTC Second */
    LL_RTC_EnableIT_SEC(RTC);

    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC))
    {
    };
    // LSE RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1), LSI RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1)
    LL_RTC_SetAsynchPrescaler(RTC, clock == clock_lse ? LSE_ASYNCH_PREDIV : LSI_ASYNCH_PREDIV);
    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC))
    {
    };
    eboxNvicIrqEnable(RTC_IRQn);
    return EOK;
}
/**
  *@brief    检查时钟是否配置过
  *@param    none
  *@retval   0 不支持或未配置
  */
uint8_t Rtc::_getTimeFlag(void)
{
#if defined(RTC_BACKUP_SUPPORT)
    return (LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR1) == RTC_BKP_DATE_TIME_UPDTATED);
#else
    return 0;
#endif
}
/**
  *@brief    备份域写信息,用来识别时钟是否已经配置过
  *@param    none
  *@retval   none
  */
void Rtc::_setTimeFlag(uint16_t configFlag)
{
#if defined(RTC_BACKUP_SUPPORT)
    /*##-8- Writes a data in a RTC Backup data Register1 #######################*/
    LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, RTC_BKP_DATE_TIME_UPDTATED);
#endif
}

/**
  *@brief    设置日期
  *@param    Date_T date 日期
  *@retval   none
  */
void Rtc::setDate(Date_T date)
{
    uint32_t counter_time = 0, counter_alarm = 0, hours = 0;

    /* Change the current date */
    gDate.Year = date.Year;
    gDate.Month = date.Month;
    gDate.Day = date.Day;

    /* WeekDay set by user can be ignored because automatically calculated */
    gDate.WeekDay = _RTC_WeekDayNum(gDate.Year, gDate.Month, gDate.Day);
    date.WeekDay = gDate.WeekDay;

    /* Reset time to be aligned on the same day */
    /* Read the time counter*/
    counter_time = _getTimeCounter();

    /* Fill the structure fields with the read parameters */
    hours = counter_time / 3600;
    if (hours > 24)
    {
        /* Set updated time in decreasing counter by number of days elapsed */
        counter_time -= ((hours / 24) * 24 * 3600);
        /* Write time counter in RTC registers */
        _setTimeCounter(counter_time);

        /* Read current Alarm counter in RTC registers */
        counter_alarm = _getAlarmCounter();

        /* Set again alarm to match with new time if enabled */
        if (counter_alarm != (uint32_t)0xFFFFFFFF)
        {
            if (counter_alarm < counter_time)
            {
                /* Add 1 day to alarm counter*/
                counter_alarm += (uint32_t)(24 * 3600);
                //                RTC_WaitForLastTask();
                while (!LL_RTC_IsActiveFlag_RTOF(RTC))
                {
                };
                //                RTC_SetAlarm(counter_alarm);
                LL_RTC_DisableWriteProtection(RTC);
                LL_RTC_ALARM_Set(RTC, counter_alarm);
                LL_RTC_EnableWriteProtection(RTC);
            }
        }
    }
}

/**
  *@name     setTime
  *@brief    设置时间,时间格式为bin
  *@param    Time_T time 时间
  *@retval   none
  */
void Rtc::setTime(Time_T time)
{
    uint32_t counter_time = 0;

    counter_time = (uint32_t)(((uint32_t)time.Hours * 3600) +
                              ((uint32_t)time.Minutes * 60) +
                              ((uint32_t)time.Seconds));
    /* Wait for RTC registers synchronization */
    ebox_WaitForSynchro_RTC();
    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC))
    {
    };
    _setTimeCounter(counter_time);
    _setTimeFlag(RTC_CFG_FLAG);
}

/**
  *@brief    设置闹铃
  *@param    Time_T time 闹铃时间,mask  ALARMA MASK
  *@retval   none
  */
void Rtc::setAlarm(Time_T alarm, uint32_t mask)
{
    uint32_t counter_alarm = 0, counter_time;
    Time_T _time;

    counter_alarm = (uint32_t)(((uint32_t)alarm.Hours * 3600) +
                               ((uint32_t)alarm.Minutes * 60) +
                               ((uint32_t)alarm.Seconds));

    /*GetTime function to update date if counter higher than 24 hours */
    getTime(&_time);
    /* Convert time in seconds */
    counter_time = (uint32_t)(((uint32_t)_time.Hours * 3600) +
                              ((uint32_t)_time.Minutes * 60) +
                              ((uint32_t)_time.Seconds));

    /* Check that requested alarm should expire in the same day (otherwise add 1 day) */
    if (counter_alarm < counter_time)
    {
        /* Add 1 day to alarm counter*/
        counter_alarm += (uint32_t)(24 * 3600);
    }
    //    ebox_WaitForSynchro_RTC();
    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC))
    {
    };
    //    RTC_SetAlarm(counter_alarm);
    _setAlarmCounter(counter_alarm);
}

/**
  *@brief    开启，关闭闹铃
  *@param    FunctionalState state ENABLE DISABLE
  *@retval   none
  */
void Rtc::setInterrupt(FunctionalState state, Rtc_IrqType type)
{
    /* Wait for RTC registers synchronization */
    ebox_WaitForSynchro_RTC();
    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC))
    {
    };

    (state == ENABLE) ? (RTC->CRH |= (1 << type)) : (RTC->CRH &= ~(1 << type));
    // _nvic(state);
}

/**
  *@brief    获取当前日期时间
  *@param    date_time_t *datetime
  *@retval   none
  */
void Rtc::getDateTime(date_time_t *datetime)
{
    Time_T time;
    getTime(&time);
    datetime->Year = gDate.Year;
    datetime->Month = gDate.Month;
    datetime->Day = gDate.Day;
    datetime->WeekDay = gDate.WeekDay;

    datetime->Hours = time.Hours;
    datetime->Minutes = time.Minutes;
    datetime->Seconds = time.Seconds;
}

/**
  *@brief    获取当前时间
  *@param    Time_T *time
  *@retval   none
  */
void Rtc::getTime(Time_T *time)
{
    uint32_t counter_time = 0, counter_alarm = 0, days_elapsed = 0, hours = 0;

    /* Wait for RTC registers synchronization */
    ebox_WaitForSynchro_RTC();
    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC))
    {
    };

    counter_time = _getTimeCounter();

    /* Fill the structure fields with the read parameters */
    hours = counter_time / 3600;
    time->Minutes = (uint8_t)((counter_time % 3600) / 60);
    time->Seconds = (uint8_t)((counter_time % 3600) % 60);

    if (hours >= 24)
    {
        /* Get number of days elapsed from last calculation */
        days_elapsed = (hours / 24);
        /* Set Hours in RTC_TimeTypeDef structure*/
        time->Hours = (hours % 24);
        /* Read Alarm counter in RTC registers */
        counter_alarm = _getAlarmCounter();

        /* 计算alarm剩余时间 (only if set and not yet expired)*/
        if ((counter_alarm != (uint32_t)0xFFFFFFFF) && (counter_alarm > counter_time))
        {
            counter_alarm -= counter_time;
        }
        else
        {
            /* 如果 counter_alarm < counter_time ，alarm 已经发生且没有解除*/
            counter_alarm = (uint32_t)0xFFFFFFFF;
        }

        /* Set updated time in decreasing counter by number of days elapsed */
        counter_time -= (days_elapsed * 24 * 3600);
        /* Write time counter in RTC registers */
        //        RTC_SetCounter(counter_time);
        _setTimeCounter(counter_time);

        /* Set updated alarm to be set */
        if (counter_alarm != (uint32_t)0xFFFFFFFF)
        {
            counter_alarm += counter_time;
        }
        /* Write time counter in RTC registers */
        // while (!LL_RTC_IsActiveFlag_RTOF(RTC))
        // {
        // };
        //        RTC_SetAlarm(counter_alarm);
        _setAlarmCounter(counter_alarm);

        /* Update date */
        _updateDate(days_elapsed);
    }
    else
    {
        time->Hours = hours;
    }

    time->Format12_24 = 0;
}

/**
  *@brief    获取当前日期
  *@param    Date_T *date
  *@retval   none
  */
void Rtc::getDate(Date_T *date)
{
    date->Year = gDate.Year;
    date->Month = gDate.Month;
    date->Day = gDate.Day;
    date->WeekDay = gDate.WeekDay;
}

void Rtc::attach(void (*fptr)(void), Rtc_IrqType type)
{
    if (fptr)
    {
        _irq[type].attach(fptr);
    }
}

void Rtc::_irq_handler(uint32_t id, Rtc_IrqType irq_type)
{
    Rtc *handler = (Rtc *)id;
    handler->_irq[irq_type].call();
}

/**
  *@brief    	设置time count
  *@param    	uint32_t count
  *@retval		none
  */
void Rtc::_setTimeCounter(uint32_t count)
{
    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC)){};
    /* Change the current time */
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_TIME_Set(RTC, count);
    LL_RTC_EnableWriteProtection(RTC);
}

void Rtc::_setAlarmCounter(uint32_t count)
{
    /* Wait until last write operation on RTC registers has finished */
    while (!LL_RTC_IsActiveFlag_RTOF(RTC)){ };
    /* Change the current time */
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_ALARM_Set(RTC, count);
    LL_RTC_EnableWriteProtection(RTC);
}
/**
  *@brief    获取time count
  *@param    none
  *@retval   返回time count
  */
uint32_t Rtc::_getTimeCounter(void)
{
    uint16_t high1 = 0, high2 = 0, low = 0;
    uint32_t timecounter = 0;

    high1 = READ_REG(RTC->CNTH & RTC_CNTH_RTC_CNT);
    low = READ_REG(RTC->CNTL & RTC_CNTL_RTC_CNT);
    high2 = READ_REG(RTC->CNTH & RTC_CNTH_RTC_CNT);

    if (high1 != high2)
    {
        /* In this case the counter roll over during reading of CNTL and CNTH registers,
        read again CNTL register then return the counter value */
        timecounter = (((uint32_t)high2 << 16) | READ_REG(RTC->CNTL & RTC_CNTL_RTC_CNT));
    }
    else
    {
        /* No counter roll over during reading of CNTL and CNTH registers, counter
        value is equal to first value of CNTL and CNTH */
        timecounter = (((uint32_t)high1 << 16) | low);
    }

    return timecounter;
}

/**
  *@brief    判断是闰年平年（能被4整除且不能被100整除的为闰年。能被100整除且能被400整除的为闰年，其他为平年）
  *@param    nYear 年份
  *@retval   1 闰年，0 平年
  */
uint8_t Rtc::_isLeapYear(uint16_t nYear)
{
    if ((nYear % 4) != 0)
        return 0;
    if ((nYear % 100) != 0)
        return 1;
    return ((nYear % 400) == 0) ? 1 : 0;
}

/**
  *@brief    获取alarm count
  *@param    none
  *@retval   返回alarm count
  */
uint32_t Rtc::_getAlarmCounter(void)
{
    uint16_t high1 = 0, low = 0;

    high1 = READ_REG(RTC->ALRH & RTC_CNTH_RTC_CNT);
    low = READ_REG(RTC->ALRL & RTC_CNTL_RTC_CNT);

    return (((uint32_t)high1 << 16) | low);
}

/**
  *@brief    更新年月日信息，更新后的结果保存在全局变量gDate中
  *@param    uint32_t dayElapsed 已过去的天数
  *@retval   none
  */
void Rtc::_updateDate(uint32_t dayElapsed)
{
    uint32_t year = 0, month = 0, day = 0;
    uint32_t loop = 0;

    /* Get the current year*/
    year = gDate.Year;
    /* Get the current month and day */
    month = gDate.Month;
    day = gDate.Day;

    for (loop = 0; loop < dayElapsed; loop++)
    {
        if ((month == 1) || (month == 3) || (month == 5) || (month == 7) ||
            (month == 8) || (month == 10) || (month == 12))
        {
            if (day < 31)
            {
                day++;
            }
            else /* Date structure member: day = 31 */
            {
                if (month != 12)
                {
                    month++;
                    day = 1;
                }
                else /* Date structure member: day = 31 & month =12 */
                {
                    month = 1;
                    day = 1;
                    year++;
                }
            }
        }
        else if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
        {
            if (day < 30)
            {
                day++;
            }
            else /* Date structure member: day = 30 */
            {
                month++;
                day = 1;
            }
        }
        else if (month == 2)
        {
            if (day < 28)
            {
                day++;
            }
            else if (day == 28)
            {
                /* Leap year */
                if (_isLeapYear(year))
                {
                    day++;
                }
                else
                {
                    month++;
                    day = 1;
                }
            }
            else if (day == 29)
            {
                month++;
                day = 1;
            }
        }
    }
    /* Update year */
    gDate.Year = year;
    /* Update day and month */
    gDate.Month = month;
    gDate.Day = day;
    /* Update day of the week */
    gDate.WeekDay = _RTC_WeekDayNum(year, month, day);
}

extern "C"
{
    void RTC_IRQHandler(void)
    {
        if (LL_RTC_IsActiveFlag_SEC(RTC) != RESET)
        {
            irq_handler(rtc_irq_id, Sec_Irq);
            LL_RTC_ClearFlag_SEC(RTC);
        }
        if (LL_RTC_IsActiveFlag_ALR(RTC) != RESET)
        {
            irq_handler(rtc_irq_id, Alr_Irq);
            LL_RTC_ClearFlag_ALR(RTC);
        }
        if (LL_RTC_IsActiveFlag_OW(RTC) != RESET)
        {
            irq_handler(rtc_irq_id, Ow_Irq);
            LL_RTC_ClearFlag_OW(RTC);
        }
    }
}
