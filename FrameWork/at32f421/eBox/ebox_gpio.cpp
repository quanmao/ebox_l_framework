/**
  ******************************************************************************
  * @file    ebox_gpio.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/8
  * @brief   GPIO����
  *   20240917 - ��ֲ��at32f421ϵ��
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ebox_base.h"
#include "ebox_gpio.h"

// ��ȡPin��port,pin����ֵ0x00-0x80,port������Ҫ��λ������ƫ�ƴ���
#define GETPIN(A) (uint16_t)(1 << (A & 0x0f))
#define GETPORT(A) (gpio_type *)(((((A)&0xf0)) << 6) + AHBPERIPH2_BASE)
// ����pin mode
#define GETMODE(A) (gpio_mode_type)((A & 0xf0) >> 4)
#define GETOUTPUT(A) (gpio_output_type)((A & 0x04) >> 2)
#define GETPP(A) ((gpio_pull_type)(A & 0x03))

// �˺����ᱻ parallel��gpio.cpp���ã������Ƴ�
void port_mode(gpio_type *port, uint32_t pin, PinMode_t mode,uint8_t af_configration=0)
{
#if ENABLE_USESWD
    if (SWD_PORT == port)
    {
        pin = pin & ~SWD_PIN;
    }
#endif

    eboxRccCmd(RccTableGPIO, (uint32_t)port, TRUE);

		gpio_init_type _gpioInit;
    
		_gpioInit.gpio_mode = GETMODE(mode);
    _gpioInit.gpio_pins = pin;
    _gpioInit.gpio_pull= GETPP(mode);
    _gpioInit.gpio_out_type = GETOUTPUT(mode);
		_gpioInit.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
		
		gpio_init(port,&_gpioInit);    
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
		gpio_pin_mux_config(_port, (gpio_pins_source_type)id, (gpio_mux_sel_type)af_configration);
}
/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::set()
{
		_port->scr = _pin;
}

/**
 *@brief    GPIO����͵�ƽ
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::reset()
{
		_port->clr = _pin;
}

/**
 *@brief    GPIO�������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
void E_GPIO::write(uint8_t val)
{
    (val == 0) ? (_port->clr = _pin) : (_port->scr = _pin);
}

/**
 *@brief    ��ȡGPIO����״̬
 *@param    NONE
 *@retval   �������ŵ�ǰ�ĵ�ƽ״̬
*/
uint8_t E_GPIO::read(void)
{
    return ((_port->idt & _pin) == _pin) ? (1) : (0);
}

/**
 *@brief    GPIO�����ƽ��ת
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::toggle()
{
    _port->odt ^= _pin;
//	(_port->idt & _pin) == _pin ?(_port->clr = _pin):(_port->scr = _pin);
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
