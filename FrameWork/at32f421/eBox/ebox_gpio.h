/**
  ******************************************************************************
  * @file    ebox_gpio.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/8
  * @brief   GPIO����
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __GPIO_H
#define __GPIO_H
#include "mcu_config.h"
#include "mcu_define.h"


class E_GPIO
{
public:
 E_GPIO(PinId_t pin_id); // ���캯��������һ��PinId_t���͵Ĳ���pin_id�����ڳ�ʼ��GPIO����
 void mode(PinMode_t mode, uint8_t af_configration = 0); // ����GPIO��ģʽ�����������롢��������õ�
 void set(); // ����GPIO��ֵΪ�ߵ�ƽ
 void reset(); // ����GPIO��ֵΪ�͵�ƽ
 void write(uint8_t val); // ����GPIO��ֵ��valΪ0ʱΪ�͵�ƽ��Ϊ1ʱΪ�ߵ�ƽ
 uint8_t read(void); // ��ȡGPIO��ֵ������0��1
 void toggle(); // �л�GPIO��ֵ���������ǰֵΪ�ߵ�ƽ�����Ϊ�͵�ƽ�������ǰֵΪ�͵�ƽ�����Ϊ�ߵ�ƽ

public:
  gpio_type *_port; /**< ���ŵĶ˿� */
  uint16_t _pin;       /**< ���ŵ���� */
  PinId_t id;
};
#endif
