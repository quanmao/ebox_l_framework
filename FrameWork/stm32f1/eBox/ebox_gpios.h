/**
  ******************************************************************************
  * @file    ebox_gpios.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  *     2019/6/10  cat_li  �޸�ʵ�֣�ʹ֮���Բ�������IO��ͬһ���˿�����IO��ͬһ��
  *       �� Gpios ���Բ������16������IO���
  *       �� Port ���Բ���ͬһ���˿ڵ�����IO��������IO
  * ע�⣺Port��������ebox_gpio�е� port_mode�ӿڣ����ɽ��ýӿ��Ƴ�
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
* @brief Ч�ʱȽϵͣ����ǿ���֧�����16������IO���
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
* @brief Ч�ʸߣ���������ͬһ��PORT�˿ڲ���
************************************************************************
*/
class E_PORT : public API_GPIOS
{
    public:
        /**
         *@brief    ����ͬһ��Port�Ķ��pin��ֻ��������Pin
         *@param    port �˿�,���� GPIOA_BASE �� 
         *          pinnum Ҫ������pin��
         *          pinoffset �����pin0��ƫ��
         *    ���� Ҫ���� PA2,PA3,PA4; �� port = GPIOB_BASE, pinnum = 3, pinoffset = 2
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
