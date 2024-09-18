/**
  ******************************************************************************
  * @file    ebox_gpio.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/8
  * @brief   GPIO操作
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
 E_GPIO(PinId_t pin_id); // 构造函数，接收一个PinId_t类型的参数pin_id，用于初始化GPIO对象
 void mode(PinMode_t mode, uint8_t af_configration = 0); // 设置GPIO的模式，可以是输入、输出、复用等
 void set(); // 设置GPIO的值为高电平
 void reset(); // 设置GPIO的值为低电平
 void write(uint8_t val); // 设置GPIO的值，val为0时为低电平，为1时为高电平
 uint8_t read(void); // 读取GPIO的值，返回0或1
 void toggle(); // 切换GPIO的值，即如果当前值为高电平，则变为低电平；如果当前值为低电平，则变为高电平

public:
  gpio_type *_port; /**< 引脚的端口 */
  uint16_t _pin;       /**< 引脚的序号 */
  PinId_t id;
};
#endif
