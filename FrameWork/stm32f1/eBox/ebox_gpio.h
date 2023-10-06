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
  E_GPIO(PinId_t pin_id);
  void mode(PinMode_t mode, uint8_t af_configration = 0);
  void set();
  void reset();
  void write(uint8_t val);
  uint8_t read(void);
  void toggle();

public:
  GPIO_TypeDef *_port; /**< 引脚的端口 */
  uint16_t _pin;       /**< 引脚的序号 */
  PinId_t id;
};
#endif
