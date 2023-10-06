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
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_gpio.h"
#include "ebox_base.h"
#include "ebox_gpio.h"


// 获取Pin和port,pin返回值0x00-0x80,port部分f0和f1的获取方式不一样
#define GETPIN(A) 	 	  (uint16_t)(1<<(A&0x0f))
#define GETPORT(A)   (GPIO_TypeDef*)(((((A)&0xf0)+0x20)<<6)+APB2PERIPH_BASE)
// 解析pin mode
#define GETMODE(A)			((A&0xf0)>>4)		
#define GETOUTPUT(A)		((A&0x04))
#define GETPP(A)				((A&0x03))

// 此函数会被 parallel—gpio.cpp调用，请勿移除
void port_mode(GPIO_TypeDef *port, uint32_t pin, PinMode_t mode,uint8_t af_configration=0)
{
#if ENABLE_USESWD
  if (SWD_PORT == port)
  {
    pin = pin & ~SWD_PIN;
  }
#endif
  // 将pinpos转换为pin，参考stm32f1xx_ll_gpio.h中GPIO_LL_EC_PIN PIN定义
  pin = pin > 0x80 ? (pin << 8) | (0x04000000U | (pin >> 8)) : (pin << 8) | pin;
  eboxRccCmd(RccTableGPIO, (uint32_t)port, ENABLE);
  if ((GETMODE(mode) & M_AF) == M_AF)
  {
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  }

  LL_GPIO_InitTypeDef _gpioInit;

  _gpioInit.Mode = GETMODE(mode);
  _gpioInit.Pin = pin;
  _gpioInit.Pull = GETPP(mode);
  _gpioInit.OutputType = GETOUTPUT(mode);
  _gpioInit.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  LL_GPIO_Init(port, &_gpioInit);
}

/**
  *@brief    构造函数
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
  *@brief    GPIO模式设置
  *@param    mode: PIN_MODE枚举变量类型，af_configration 第二功能
  *@retval   None
  */
void E_GPIO::mode(PinMode_t mode, uint8_t af_configration)
{
		port_mode(_port, _pin, mode,af_configration);
}
/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::set()
{
    _port->BSRR = _pin;
}

/**
 *@brief    GPIO输出低电平
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::reset()
{
    _port->BRR = _pin;
}


/**
 *@brief    GPIO输出设置
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
void E_GPIO::write(uint8_t val)
{
    // val==0,执行_port->BRR = _pin，否则，_port->BSRR = _pin
    (val == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
}


/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
//void mcuGpio::read(uint8_t *val)
//{
//    *val = _port->IDR & _pin;
//}


/**
 *@brief    读取GPIO引脚状态
 *@param    NONE
 *@retval   返回引脚当前的电平状态
*/
uint8_t E_GPIO::read(void)
{
    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
}

/**
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void E_GPIO::toggle()
{
    _port->ODR ^= _pin;
}

//// 相当与read()
//int mcuGpio::operator =(mcuGpio &)
//{
//    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
//}
//// 相当与read()
//mcuGpio::operator int()
//{
//    return ((_port->IDR & _pin) == _pin) ? (1) : (0);
//}
//// 相当与write()
//mcuGpio mcuGpio::operator= ( int value)
//{
//    (value == 0) ? (_port->BRR = _pin) : (_port->BSRR = _pin);
//    return *this;
//}
