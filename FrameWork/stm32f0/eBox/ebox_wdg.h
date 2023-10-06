/**
  ******************************************************************************
  * @file    wdg.h
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

#ifndef __WDG_H
#define __WDG_H
#include "mcu_config.h"
#include "mcu_define.h"

/**
 * 初始化独立看门狗
   基本计算方法
 * pr:分频数:0~6(只有低 3 位有效!)
 * 分频因子=4*2^pr.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^pr)*rlr)/40 = (256*4095)/40 = 26208(ms).
   本函数内部已经做了相关计算；
    输入参数为ms；1000代表1000ms；请在1s内喂一次狗。否则将会复位
 */

// 独立看门狗，工作原理就是一个递减计数器不断的往下递减计数，当减到 0 之前如果没有喂狗的话，产生复位
class E_IWWDG
{
public:
    E_IWWDG() {}; // 独立看门狗
    /**
    *@brief    启动看门狗,最长延时26208ms
    *@param    ms 延时时长，单位ms
    *@retval   NONE
    */
    void begin(uint16_t ms);
    /**
    *@brief    喂狗,当看门够去最长26208时，建议喂狗间隔不要超过25s
    *@param    none
    *@retval   NONE
    */
    void feed();
};

/**
 * 窗口看门狗，也是一个递减计数器不断的往下递减计数，
 * 当减到一个固定值 0X40 时还不喂狗的话，产生复位，这个值叫窗口的下限，是固定的值，不能改变。
 * 在减到某一个数之前喂狗的话也会产生 复位，这个值叫窗口的上限，上限值由用户独立设置。
 * 窗口看门狗计数器的值必须在上窗口和下窗口之间才可以喂狗，这就是窗口看门狗中窗口两个字的含义
 * */ 
class E_WWDG
{
public:
    E_WWDG() {}; // 窗口看门狗
    /**
    *@brief    启动看门狗
    *@param    wr 窗口计数器 us。 0x40计数为最小超时(min). min<=wr<=tr.
    *@param    tr 计数值 us
    *@retval   NONE
    */
    void begin(uint16_t wr,uint16_t tr);
    /**
     * @brief 返回tr计数器的最大值最小值。(tr的实际步进=最小超时时间，以1分频为例，最小值85，往后依次是170，340....) 
     * @param pr 分频系数(0,1,2,3)
     * @param max_us 最大超时时间(5461,10922,21845,43690)us
     * @param min_us 最小超时时间(85,170,341,682)us
     */
    void getMaxMin(uint8_t pr,uint16_t &max_us,uint16_t &min_us);
    void feed();  // tr--开始计数，tr<-（feed）->wr 发生复位。wr>=(feed)>min正常喂狗。
    uint8_t isWWDGRst(void);  // 发生WWDG复位，返回1.
private:
    uint8_t tr;
};

#endif
