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
 * ��ʼ���������Ź�
   �������㷽��
 * pr:��Ƶ��:0~6(ֻ�е� 3 λ��Ч!)
 * ��Ƶ����=4*2^pr.�����ֵֻ���� 256!
 * rlr:��װ�ؼĴ���ֵ:�� 11 λ��Ч.
 * ʱ�����(���):Tout=((4*2^pr)*rlr)/40 = (256*4095)/40 = 26208(ms).
   �������ڲ��Ѿ�������ؼ��㣻
    �������Ϊms��1000����1000ms������1s��ιһ�ι������򽫻Ḵλ
 */

// �������Ź�������ԭ�����һ���ݼ����������ϵ����µݼ������������� 0 ֮ǰ���û��ι���Ļ���������λ
class E_IWWDG
{
public:
    E_IWWDG() {}; // �������Ź�
    /**
    *@brief    �������Ź�,���ʱ26208ms
    *@param    ms ��ʱʱ������λms
    *@retval   NONE
    */
    void begin(uint16_t ms);
    /**
    *@brief    ι��,�����Ź�ȥ�26208ʱ������ι�������Ҫ����25s
    *@param    none
    *@retval   NONE
    */
    void feed();
};

/**
 * ���ڿ��Ź���Ҳ��һ���ݼ����������ϵ����µݼ�������
 * ������һ���̶�ֵ 0X40 ʱ����ι���Ļ���������λ�����ֵ�д��ڵ����ޣ��ǹ̶���ֵ�����ܸı䡣
 * �ڼ���ĳһ����֮ǰι���Ļ�Ҳ����� ��λ�����ֵ�д��ڵ����ޣ�����ֵ���û��������á�
 * ���ڿ��Ź���������ֵ�������ϴ��ں��´���֮��ſ���ι��������Ǵ��ڿ��Ź��д��������ֵĺ���
 * */ 
class E_WWDG
{
public:
    E_WWDG() {}; // ���ڿ��Ź�
    /**
    *@brief    �������Ź�
    *@param    wr ���ڼ����� us�� 0x40����Ϊ��С��ʱ(min). min<=wr<=tr.
    *@param    tr ����ֵ us
    *@retval   NONE
    */
    void begin(uint16_t wr,uint16_t tr);
    /**
     * @brief ����tr�����������ֵ��Сֵ��(tr��ʵ�ʲ���=��С��ʱʱ�䣬��1��ƵΪ������Сֵ85������������170��340....) 
     * @param pr ��Ƶϵ��(0,1,2,3)
     * @param max_us ���ʱʱ��(5461,10922,21845,43690)us
     * @param min_us ��С��ʱʱ��(85,170,341,682)us
     */
    void getMaxMin(uint8_t pr,uint16_t &max_us,uint16_t &min_us);
    void feed();  // tr--��ʼ������tr<-��feed��->wr ������λ��wr>=(feed)>min����ι����
    uint8_t isWWDGRst(void);  // ����WWDG��λ������1.
private:
    uint8_t tr;
};

#endif
