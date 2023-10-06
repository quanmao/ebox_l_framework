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
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_ll_gpio.h"
#include "ebox_base.h"
#include "ebox_gpio.h"

// ��ȡPin��port,pin����ֵ0x00-0x80,port����f0��f1�Ļ�ȡ��ʽ��һ��
#define GETPIN(A) (uint16_t)(1 << (A & 0x0f))
#define GETPORT(A) (GPIO_TypeDef *)(((((A)&0xf0)) << 6) + AHB2PERIPH_BASE)
// ����pin mode
#define GETMODE(A) ((A & 0xf0) >> 4)
#define GETOUTPUT(A) ((A & 0x04) >> 2)
#define GETPP(A) ((A & 0x03))

// �˺����ᱻ parallel��gpio.cpp���ã������Ƴ�
void port_mode(GPIO_TypeDef *port, uint32_t pin, PinMode_t mode,uint8_t af_configration=0)
{
#if ENABLE_USESWD
    if (SWD_PORT == port)
    {
        pin = pin & ~SWD_PIN;
    }
#endif

    eboxRccCmd(RccTableGPIO, (uint32_t)port, ENABLE);
    LL_GPIO_InitTypeDef _gpioInit;

    _gpioInit.Mode = GETMODE(mode);
    _gpioInit.Pin = pin;
    _gpioInit.Pull = GETPP(mode);
    _gpioInit.OutputType = GETOUTPUT(mode);
    _gpioInit.Speed = LL_GPIO_SPEED_HIGH;
		_gpioInit.Alternate = af_configration;
    LL_GPIO_Init(port, &_gpioInit);
}

/**
  *@brief    ���캯��
  *@param    PIN_ID_t pin_id
  *@retval   None
  */
E_GPIO::E_GPIO(PinId_t pin_id)
{
    id = pin_id;
    _port = GETPORT(id);
    _pin = GETPIN(id);
}

/**
  *@brief    GPIOģʽ����
  *@param    mode: PIN_MODEö�ٱ������ͣ�af_configration �ڶ�����
  *@retval   None
  */
void E_GPIO::mode(PinMode_t mode, uint8_t af_configration)
{
		port_mode(_port, _pin, mode,af_configration);
}
/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::set()
{
    _port->BSRR = _pin;
}

/**
 *@brief    GPIO����͵�ƽ
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::reset()
{
    _port->BRR = _pin;
}

/**
 *@brief    GPIO�������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
void E_GPIO::write(uint8_t val)
{
    // val==0,ִ��_port->BRR = _pin������_port->BSRR = _pin
    (val == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
}

/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
//void E_GPIO::read(uint8_t *val)
//{
//    *val = _port->IDR & _pin;
//}

/**
 *@brief    ��ȡGPIO����״̬
 *@param    NONE
 *@retval   �������ŵ�ǰ�ĵ�ƽ״̬
*/
uint8_t E_GPIO::read(void)
{
    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}

/**
 *@brief    GPIO�����ƽ��ת
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::toggle()
{
    _port->ODR ^= _pin;
}

//// �൱��read()
//int E_GPIO::operator =(E_GPIO &)
//{
//    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
//}
//// �൱��read()
//E_GPIO::operator int()
//{
//    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
//}
//// �൱��write()
//E_GPIO E_GPIO::operator= ( int value)
//{
//    (value == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
//    return *this;
//}
