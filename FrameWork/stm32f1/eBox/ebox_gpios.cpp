/**
  ******************************************************************************
  * @file    ebox_gpios.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  *     2019/6/10  cat_li  �޸�ʵ�֣�ʹ֮���Բ�������IO��ͬһ���˿�����IO��ͬһ��
  *       �� Gpios ���Բ������16������IO���
  *       �� Port ���Բ���ͬһ���˿ڵ�����IO��������IO
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
 * @param pin **pin GPIO��������ָ�룬����Ҫ������gpio
 * @param num gpios ����
 */
E_GPIOS::E_GPIOS(E_GPIO **pin, uint8_t num)
{
    _num = num;
    _bit = pin;
}

/**
 *@brief    Gpio��ģʽ����
 *@param    mode:   PIN_MODEö�ٱ�������
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
 *@brief    Gpio���������
 *@param    data���������
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
 *@brief    ��ȡGpio����������
 *@param    NONE
 *@retval   ��ȡGpio�������
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
// �ú���ʵ����ebox_gpio.cpp�У���ͬƽ̨ʵ���в���
extern void port_mode(GPIO_TypeDef* port,uint32_t pin, PinMode_t mode,uint8_t af_config=0);

/**
 * @brief Construct a new e port::e port object * 
 * @param port �˿ڣ�xe GPIOA,GPIOB
 * @param pinnum pin ����
 * @param pinoffset ���pin0��ƫ����
 */
E_PORT::E_PORT(uint32_t port, uint8_t pinnum, uint8_t pinoffset)
{
   _mask = (0xffff >> (16 - pinnum)) << pinoffset;
   _offset = pinoffset;
   _port = (GPIO_TypeDef *)port;
}


/**
*@brief    Gpio��ģʽ����
*@param    mode:   PIN_MODEö�ٱ�������
*@retval   None
*/
void E_PORT::mode(PinMode_t mode)
{
   port_mode(_port,_mask,mode);
}

/**
*@name     void ParallelGpio::write(uint8_t data)
*@param    data���������
*@retval   NONE
*/
void E_PORT::write(uint16_t val)
{
   _port->ODR = (_port->ODR & ~_mask) | ((val << _offset) & _mask);
}

/**
*@name     uint16_t ParallelGpio::read(void)
*@param    NONE
*@retval   ��ȡGpio�������
*/
uint16_t E_PORT::read(void)
{
   return (_port->IDR & _mask) >> _offset;
}
