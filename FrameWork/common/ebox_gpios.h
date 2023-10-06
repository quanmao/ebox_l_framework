/**
  ******************************************************************************
  * @file    ebox_gpios.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  *     2019/6/10  cat_li  修改实现，使之可以操作任意IO，同一个端口连续IO，同一个
  *       类 Gpios 可以操作最多16个任意IO组合
  *       类 Port 可以操作同一个端口的连续IO，不连续IO
  * 注意：Port操作依赖ebox_gpio中的 port_mode接口，不可将该接口移除
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

#ifndef  __PARALLEL_GPIO_H
#define  __PARALLEL_GPIO_H


class API_GPIOS
{
    public:
        API_GPIOS() {};
        virtual void mode(PinMode_t mode) = 0;
        virtual void write(uint16_t val) = 0;
        virtual uint16_t read(void) = 0;
};

/**
************************************************************************
* @brief 效率比较低，但是可以支持最多16个任意IO组合
************************************************************************
*/
class E_GPIOS : public API_GPIOS
{
    public:
        E_GPIOS(E_GPIO **pin,uint8_t num);
        virtual void mode(PinMode_t mode);
        virtual void write(uint16_t val);
        virtual uint16_t read(void);
    private:
        E_GPIO      **_bit;
        uint8_t		_num;
};

/**
************************************************************************
* @brief 效率高，但必须是同一个PORT端口才行
************************************************************************
*/
class E_PORT : public API_GPIOS
{
    public:
        /**
         *@brief    操作同一个Port的多个pin，只能是连续Pin
         *@param    port 端口,例如 GPIOA_BASE ； 
         *          pinnum 要操作的pin，
         *          pinoffset 相对与pin0的偏移
         *    例： 要控制 PA2,PA3,PA4; 则 port = GPIOB_BASE, pinnum = 3, pinoffset = 2
         *@retval   NONE
         */
        E_PORT(uint32_t port, uint8_t pinnum, uint8_t pinoffset = 0);
        virtual void mode(PinMode_t mode);
        virtual void write(uint16_t val);
        virtual uint16_t read(void);
    private:
        uint16_t _mask;
        uint8_t _offset;
        GPIO_TypeDef *_port;
};

#endif
