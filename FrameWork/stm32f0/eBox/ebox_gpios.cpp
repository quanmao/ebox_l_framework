/**
  ******************************************************************************
  * @file    ebox_gpios.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  *     2019/6/10  cat_li  修改实现，使之可以操作任意IO，同一个端口连续IO，同一个
  *       类 Gpios 可以操作最多16个任意IO组合
  *       类 Port 可以操作同一个端口的连续IO，不连续IO
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
#include "ebox_gpio.h"
#include "ebox_gpios.h"

/*class gpios*/
/**
 * @brief Construct a new e gpios::e gpios object
 * @param pin **pin GPIO对象数组指针，保存要操作的gpio
 * @param num gpios 数量
 */
E_GPIOS::E_GPIOS(E_GPIO **pin, uint8_t num)
{
    _num = num;
    _bit = pin;
}

/**
 *@brief    Gpio组模式设置
 *@param    mode:   PIN_MODE枚举变量类型
 *@retval   None
 */
void E_GPIOS::mode(PinMode_t mode)
{
    for (uint8_t i = 0; i < _num; i++)
    {
        (_bit[i])->mode(mode);
    }
}

/**
 *@name    write(uint16_t data)
 *@brief    Gpio组输出数据
 *@param    data：输出数据
 *@retval   NONE
 */
void E_GPIOS::write(uint16_t val)
{
    for (uint8_t i = 0; i < _num; i++)
    {
        _bit[i]->write(val & (0x01 << i));
    }
}

/**
 *@name     read(void)
 *@brief    读取Gpio组输入数据
 *@param    NONE
 *@retval   读取Gpio组的数据
 */
uint16_t E_GPIOS::read(void)
{
    uint16_t r = 0;
    for (uint8_t i = 0; i < _num; i++)
    {
        r |= _bit[i]->read() << i;
    }
    return r;
}

/*class GPIO port*/
// 该函数实现在ebox_gpio.cpp中，不同平台实现有差异
extern void port_mode(GPIO_TypeDef* port,uint32_t pin, PinMode_t mode,uint8_t af_config=0);

/**
 * @brief Construct a new e port::e port object * 
 * @param port 端口，xe GPIOA,GPIOB
 * @param pinnum pin 数量
 * @param pinoffset 相对pin0的偏移量
 */
E_PORT::E_PORT(uint32_t port, uint8_t pinnum, uint8_t pinoffset)
{
   _mask = (0xffff >> (16 - pinnum)) << pinoffset;
   _offset = pinoffset;
   _port = (GPIO_TypeDef *)port;
}


/**
*@brief    Gpio组模式设置
*@param    mode:   PIN_MODE枚举变量类型
*@retval   None
*/
void E_PORT::mode(PinMode_t mode)
{
   port_mode(_port,_mask,mode);
}

/**
*@name     void ParallelGpio::write(uint8_t data)
*@param    data：输出数据
*@retval   NONE
*/
void E_PORT::write(uint16_t val)
{
   _port->ODR = (_port->ODR & ~_mask) | ((val << _offset) & _mask);
}

/**
*@name     uint16_t ParallelGpio::read(void)
*@param    NONE
*@retval   读取Gpio组的数据
*/
uint16_t E_PORT::read(void)
{
   return (_port->IDR & _mask) >> _offset;
}
