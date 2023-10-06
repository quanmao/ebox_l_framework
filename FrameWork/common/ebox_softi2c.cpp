/**
  ******************************************************************************
  * @file    softi2c.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
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
#include "ebox_base.h"
#include "mcu_define.h"
#include "ebox_i2c.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[SOFTI2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif


/**
  *@brief    softI2C构造函数
  *@param    scl_pin:  时钟Pin
  *          sda_pin:  数据Pin
  *@retval   None
  */
E_SOFTI2C::E_SOFTI2C(E_GPIO *scl, E_GPIO *sda)
{
    _scl = scl;
    _sda = sda;
    _busy = 0;
}

/**
  *@brief    根据i2c时钟和设置速率speed计算timing,默认80K
  *@param    speed:  速率 10,100,200,300,400 分别代表10k，100k，200k,300k,400k
  *@retval   None
  */
void E_SOFTI2C::begin(Speed_t speed)
{
    _sda->mode(OUTPUT_OD_PU);
    _scl->mode(OUTPUT_OD_PU);
    _sda->set();
    _scl->set();

    I2C_DEBUG("scl pin id 0x%x state is %d , sda pin id 0x%x  state is %d \r\n", _scl->id, _scl->read(), _sda->id, _sda->read());
    config(speed);
}

/**
 * @brief 设置i2c通信速度.
 *
 * @param[in] speed i2c速度，当其值为400K，300K，200K，100K时，i2c通信频率为该值.
 *
 * @return 0.
 */
void E_SOFTI2C::config(Speed_t speed)
{
    _speed = speed;
    _timing = 4; // 约80k
    if (speed >= K200) _timing = 1; // 约200k
    if (speed == K100) _timing = 3; // 约100k
}

/**
  *@brief    I2C写入一个字节. start->data->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t data:  要写入的数据
  *          uint16_t tOut: 超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t E_SOFTI2C::write(uint16_t slaveAddr, uint8_t data)
{
    uint8_t err = EOK;
    //  I2C_DEBUG("I2C Bus state,SCL is %d, SDA is %d \r\n",_scl->read(),_sda->read());
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(data);
    _stop();
    return err;
}

/**
  *@brief    I2C连续写 start->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t tOut:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t E_SOFTI2C::write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    while (nWrite--)
    {
        err += _sendByte(*data);
        data++;
    }
    _stop();
    return err;
}

/**
  *@brief    I2C读入一个字节. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t E_SOFTI2C::read(uint16_t slaveAddr)
{
    uint8_t data ;
    _start();
    _send7bitsAddress(slaveAddr, READ);
    _receiveByte(&data);
    _sendNack();
    _stop();
    return data;
}

/**
  *@brief    连续读取. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          uint16_t tOut: 超时
  *@retval   EOK，EWAIT
  */
uint8_t E_SOFTI2C::read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, READ);
    while (nRead--)
    {
        err += _receiveByte(data);
        data++;
        if (nRead == 0)
        {
            _sendNack();
            _stop();
            break;
        }
        _sendAck();
    }
    return err;
}

/**
 * @brief   等待从机空闲.
 * @param   [in] slave_address 7bit从机地址.
 * @return  从机状态.返回0表示从机空闲，返回-1表示从机忙.
 */
uint8_t E_SOFTI2C::check_busy(uint16_t slaveAddr)
{
    int8_t ret;
    uint8_t i = 0;
    do
    {
        _start();
        ret = _send7bitsAddress(slaveAddr, WRITE);
        _stop();
        if (i++ == 100)
        {
            return EBUSY;
        }
    } while (ret != 0); //如果返回值不是0，继续等待
    return EOK;
}

/**
  *@brief    获取I2C控制权
  *@param    timing:  时钟时序，通过readConfig获取
  *@retval   uint8_t: EOK,E_BUSY
  */
uint8_t E_SOFTI2C::take(Speed_t speed)
{
    uint32_t end = ENDTIME(timeout);
    while (_busy == 1)
    {
        delayMs(1);
        if (TIMEOUT(end, timeout)) return EWAIT;
    }
    if (_speed != speed) config(speed);
    _busy = 1;
    return EOK;
}

/**
 *@brief    释放I2C控制权
 *@param    none
 *@retval   none
*/
void E_SOFTI2C::release(void)
{
    _busy = 0;
}

/**
  * @brief 发送一个START信号.SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据
  * @param 无.
  * @return 无.
  */
int8_t E_SOFTI2C::_start()
{
    uint32_t end = ENDTIME(timeout);

    _scl->set();          // SCL高
    while (!(_sda->read() && _scl->read()))       // scl,sda 均为高电平，总线空闲
    {
        if (TIMEOUT(end,timeout))
        {
            I2C_DEBUG("start fail,bus busy SDA is %d，SCL is %d,time is %d us \r\n", _sda->read(), _scl->read(), _timing);
            return EWAIT;
        }
    }
    _sda->set();
    _scl->set();          // SCL高
    delayUs(_timing);
    _sda->reset();        // SDA拉低
    delayUs(_timing);
    _scl->reset();
    delayUs(_timing);
    return EOK;
}

/**
  * @brief 发送一个STOP信号. SCL为高电平时，SDA由低电平向高电平跳变，结束传送数据
  * @param 无.
  * @return 无.
  */
void E_SOFTI2C::_stop()
{
    _sda->reset(); // SDA低电平
    _scl->set();   // SCL拉高并保持
    delayUs(_timing);
    _sda->set(); // SDA拉高
    delayUs(_timing);
}

/**
 * @brief 等待一个ACK应答. 第9个clock，若从IC发ACK，SDA会被拉低
 * @param 无.
 * @return EOK,EWAIT.
 */
int8_t E_SOFTI2C::_waitAck()
{
    uint8_t re;
    _sda->set();
    delayUs(_timing);
    _scl->set();
    delayUs(_timing);
    if (!_sda->read()) //SDA为低则从设备返回Ack，否则没返回
    {
        re = 0;
    }
    else
    {
        re = 1;
    }
    _scl->reset();
    delayUs(_timing);
    return re;
}

/**
 * @brief 发送一个ACK应答. 第9个clock期间，拉低SDA
 * @param 无.
 * @return 0.
 */
int8_t E_SOFTI2C::_sendAck()
{
    //    _sda->mode(OUTPUT_PP);
    _sda->reset(); // 拉低SDA，
    delayUs(_timing);
    _scl->set(); // 给出一个时钟
    delayUs(_timing);
    _scl->reset();
    delayUs(_timing);
    return EOK;
}

/**
  * @brief 发送一个NACK应答,第9个clock期间，拉高SDA
  * @param 无
  * @return 0.
  */
int8_t E_SOFTI2C::_sendNack()
{
    _sda->set();
    delayUs(_timing);
    _scl->set();
    delayUs(_timing);
    _scl->reset();
    delayUs(_timing);
    return EOK;
}

/**
  * @brief 发送数据.
  * @param[in] byte 将被发送的数据.
  * @return 发送结果.EOK,EWAIT.
  */
int8_t E_SOFTI2C::_sendByte(uint8_t byte)
{
    int8_t ret = EOK;
    uint8_t ii = 8;
    _scl->reset();
    while (ii--)
    {
        _sda->write(byte & 0x80); // SCL低电平时将数据送到SDA
        delayUs(_timing);
        _scl->set(); // 产生一个时钟脉冲
        delayUs(_timing);
        byte = byte << 1;
        _scl->reset();
    }
    ret = _waitAck();
    return ret;
}

/**
 * @brief 发送7bit从机地址位.
 *
 * @param[in] slave_address 7bit从机地址位.
 *
 * @return 发送结果.返回0表示发送成功，返回-1表示发送失败.
 */
int8_t E_SOFTI2C::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
{
    int8_t ret = 0;
    // 写，从地址最低位置0；读，从地址最低位置1；
    slaveAddr = (WR == WRITE) ? (slaveAddr & 0xfe) : (slaveAddr | 0x01);
    ret = _sendByte(slaveAddr);
    return ret;
}

/**
  * @brief 接收数据.
  * @param 无.
  * @return 接收到的数据.
  */
int8_t E_SOFTI2C::_receiveByte(uint8_t *data)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    _sda->set();
    while (i--)
    {
        byte += byte;
        _scl->reset(); // 时钟高电平期间读SDA
        delayUs(_timing);
        _scl->set();
        delayUs(_timing);
        byte |= _sda->read();
    }
    _scl->reset();
    delayUs(_timing);
    *data = byte;
    return EOK;
}

#if USE_REGADDR
/**
  *@brief    指定位置写入一个字节. start->data->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t data:  要写入的数据
  *          uint16_t tOut: 超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t E_SOFTI2C::write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data)
{
    uint8_t err = EOK;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(regAddr);
    err += _sendByte(data);
    _stop();
//    delayUs(10);
    return err;
}

/**
  *@brief    读指定寄存器. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：   要读取的寄存器
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t E_SOFTI2C::read(uint16_t slaveAddr, uint16_t regAddr)
{
    uint8_t data ;
    _start();
    _send7bitsAddress(slaveAddr, WRITE);
    _sendByte(regAddr);
    _start();
    _send7bitsAddress(slaveAddr, READ);
    _receiveByte(&data);
    _sendNack();
    _stop();
    return data;
}

/**
  *@brief    在指定寄存器连续写 start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：要写入的寄存器地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t tOut:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t E_SOFTI2C::write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(regAddr);
    while (nWrite--)
    {
        err += _sendByte(*data);
        data++;
    }
    _stop();
    return err;
}

/**
  *@brief    指定寄存器连续读取. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr: 寄存器地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          uint16_t tOut: 超时
  *@retval   EOK，EWAIT
  */
uint8_t E_SOFTI2C::read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, WRITE);
    err += _sendByte(regAddr);
    err += _start();
    err += _send7bitsAddress(slaveAddr, READ);
    while (nRead--)
    {
        err += _receiveByte(data);
        data++;
        if (nRead == 0)
        {
            _sendNack();
            _stop();
            break;
        }
        _sendAck();
    }
    return err;
}

#endif
