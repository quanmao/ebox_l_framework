/**
  ******************************************************************************
  * @file    rtc.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __RTC_H
#define __RTC_H
#include "stm32f0xx_ll_rtc.h"
#include "ebox_base.h"
#include "mcu_define.h"
#include "FunctionPointer.h"


/**
 * 说明:
 * -LQM (2016/9/12)
 *  1 LSE为时钟，当Vbat维持，VDD掉电，RTC仍然继续工作
 *  2 LSI为时钟，VDD掉电后，RTC状态不定
 *  3 HSE/32为时钟，VDD掉电或内部调压器（V1.8）掉电，RTC状态不定
 */

typedef struct
{
    uint32_t Format12_24;	//
    uint8_t Hours;			// 0-12 or 0-23
    uint8_t Minutes;		// 0-59
    uint8_t Seconds;		// 0-59
} Time_T;

typedef struct
{
    Time_T   time;
    uint32_t Day;
    uint32_t WeekDaySel;
} Alarm_T;

typedef struct
{
    uint8_t WeekDay;	// 0-6 周日到周六
    uint8_t Day;		// 1-31
    uint8_t Month;		// 1-12
    uint8_t Year;		// 0-99
} Date_T;

typedef struct
{
    uint8_t Year;
    uint8_t Month;
    uint8_t Day;
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
    uint8_t WeekDay;
} date_time_t;

typedef enum
{
    clock_lse = 1, //0x100
    clock_lsi,     //0x200,
    clock_hse      //0x300
} ClockS;

enum Rtc_IrqType    // FO中无意义，F1系列有效
{
    Sec_Irq = 0,
    Alr_Irq = 1,
    Ow_Irq = 2
};

#define eRtc_NoMask                         LL_RTC_ALMA_MASK_NONE          // 不屏蔽,date,hour,minutes,seconds均参与匹配
#define eRtc_MaskDateWeekDay                LL_RTC_ALMA_MASK_DATEWEEKDAY   // 不匹配年月日
#define eRtc_MaskHours                      LL_RTC_ALMA_MASK_HOURS         // 不匹配小时
#define eRtc_MaskMinutes                    LL_RTC_ALMA_MASK_MINUTES       // 不匹配分钟
#define eRtc_MaskSeconds                    LL_RTC_ALMA_MASK_SECONDS       // 不匹配秒
#define eRtc_MaskAll                        (RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1) //屏蔽所有位,此时闹铃每秒触发一次

typedef void (*rtc_irq_handler)(uint32_t id, Rtc_IrqType type);

class E_RTC
{
public:
    /**
    	* @brief  初始化RTC,默认LSE,如果LSE启动失败,会自动转为LSI
    	* @param  ClockS clock 时钟源，clock_lse,clock_lsi
    	* @retval E_TIMEOUT 时钟配置失败,E_OK 时钟配置成功,无需设置时钟，E_NG 时钟配置成功，需要设置时钟
    	*/
    int begin(ClockS clock = clock_lse);

    /** 设置闹铃
     *  @param time 定时时间
     *  @param mask 屏蔽闹铃响应位(详细看define)，默认不匹配日期。f1系列中该参数无效，f0中有效。
     */
    void setAlarm(Time_T time, uint32_t mask = eRtc_MaskDateWeekDay);
    void setInterrupt(FunctionalState state, Rtc_IrqType type = Alr_Irq);   // type 参数在f0系列中无效，f1系列可用

    // 设置日期，时间
    void setDate(Date_T date);
    void setTime(Time_T time);
    // 读取日期，时间
    void getDateTime(date_time_t *datetime);
    void getTime(Time_T *time);
    void getDate(Date_T *date);

    /** 绑定回调函数
     *  @param fptr 函数指针，指向一个无返回值的子函数, 或 0 指向空函数
     *  @param type 中断类型，默认 alarm中断。f1系列中该参数有效，f0中无效
     */
    void attach(void (*fptr)(void), Rtc_IrqType type = Alr_Irq);

    /** 绑定成员回调函数
     *  @param tptr 对象指针，一般为类名
     *  @param mptr 成员指针，格式一般为 &类名::成员名
     *  @param type 中断类型，默认 alarm中断。f1系列中该参数有效，f0中无效
     */
    template <typename T>
    void attach(T *tptr, void (T::*mptr)(void), Rtc_IrqType type = Sec_Irq)
    {
        if ((mptr != NULL) && (tptr != NULL))
        {
            _irq.attach(tptr, mptr);
        }
    }

private:
    int _config(ClockS clock);
    void _setTimeFlag(uint16_t configFlag);
    uint8_t _getTimeFlag(void);
    void _nvic(FunctionalState state);
    void _setFormat(uint32_t formant);
    void _updateTime(Time_T *time);

protected:
    FunctionPointer _irq;
    static void _irq_handler(uint32_t id, Rtc_IrqType irq_type);
};

#endif
