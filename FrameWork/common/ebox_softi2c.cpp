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
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[SOFTI2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif


/**
  *@brief    softI2C���캯��
  *@param    scl_pin:  ʱ��Pin
  *          sda_pin:  ����Pin
  *@retval   None
  */
E_SOFTI2C::E_SOFTI2C(E_GPIO *scl, E_GPIO *sda)
{
    _scl = scl;
    _sda = sda;
    _busy = 0;
}

/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing,Ĭ��80K
  *@param    speed:  ���� 10,100,200,300,400 �ֱ����10k��100k��200k,300k,400k
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
 * @brief ����i2cͨ���ٶ�.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return 0.
 */
void E_SOFTI2C::config(Speed_t speed)
{
    _speed = speed;
    _timing = 4; // Լ80k
    if (speed >= K200) _timing = 1; // Լ200k
    if (speed == K100) _timing = 3; // Լ100k
}

/**
  *@brief    I2Cд��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
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
  *@brief    I2C����д start->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
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
  *@brief    I2C����һ���ֽ�. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
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
  *@brief    ������ȡ. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
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
 * @brief   �ȴ��ӻ�����.
 * @param   [in] slave_address 7bit�ӻ���ַ.
 * @return  �ӻ�״̬.����0��ʾ�ӻ����У�����-1��ʾ�ӻ�æ.
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
    } while (ret != 0); //�������ֵ����0�������ȴ�
    return EOK;
}

/**
  *@brief    ��ȡI2C����Ȩ
  *@param    timing:  ʱ��ʱ��ͨ��readConfig��ȡ
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
 *@brief    �ͷ�I2C����Ȩ
 *@param    none
 *@retval   none
*/
void E_SOFTI2C::release(void)
{
    _busy = 0;
}

/**
  * @brief ����һ��START�ź�.SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬��ʼ��������
  * @param ��.
  * @return ��.
  */
int8_t E_SOFTI2C::_start()
{
    uint32_t end = ENDTIME(timeout);

    _scl->set();          // SCL��
    while (!(_sda->read() && _scl->read()))       // scl,sda ��Ϊ�ߵ�ƽ�����߿���
    {
        if (TIMEOUT(end,timeout))
        {
            I2C_DEBUG("start fail,bus busy SDA is %d��SCL is %d,time is %d us \r\n", _sda->read(), _scl->read(), _timing);
            return EWAIT;
        }
    }
    _sda->set();
    _scl->set();          // SCL��
    delayUs(_timing);
    _sda->reset();        // SDA����
    delayUs(_timing);
    _scl->reset();
    delayUs(_timing);
    return EOK;
}

/**
  * @brief ����һ��STOP�ź�. SCLΪ�ߵ�ƽʱ��SDA�ɵ͵�ƽ��ߵ�ƽ���䣬������������
  * @param ��.
  * @return ��.
  */
void E_SOFTI2C::_stop()
{
    _sda->reset(); // SDA�͵�ƽ
    _scl->set();   // SCL���߲�����
    delayUs(_timing);
    _sda->set(); // SDA����
    delayUs(_timing);
}

/**
 * @brief �ȴ�һ��ACKӦ��. ��9��clock������IC��ACK��SDA�ᱻ����
 * @param ��.
 * @return EOK,EWAIT.
 */
int8_t E_SOFTI2C::_waitAck()
{
    uint8_t re;
    _sda->set();
    delayUs(_timing);
    _scl->set();
    delayUs(_timing);
    if (!_sda->read()) //SDAΪ������豸����Ack������û����
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
 * @brief ����һ��ACKӦ��. ��9��clock�ڼ䣬����SDA
 * @param ��.
 * @return 0.
 */
int8_t E_SOFTI2C::_sendAck()
{
    //    _sda->mode(OUTPUT_PP);
    _sda->reset(); // ����SDA��
    delayUs(_timing);
    _scl->set(); // ����һ��ʱ��
    delayUs(_timing);
    _scl->reset();
    delayUs(_timing);
    return EOK;
}

/**
  * @brief ����һ��NACKӦ��,��9��clock�ڼ䣬����SDA
  * @param ��
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
  * @brief ��������.
  * @param[in] byte �������͵�����.
  * @return ���ͽ��.EOK,EWAIT.
  */
int8_t E_SOFTI2C::_sendByte(uint8_t byte)
{
    int8_t ret = EOK;
    uint8_t ii = 8;
    _scl->reset();
    while (ii--)
    {
        _sda->write(byte & 0x80); // SCL�͵�ƽʱ�������͵�SDA
        delayUs(_timing);
        _scl->set(); // ����һ��ʱ������
        delayUs(_timing);
        byte = byte << 1;
        _scl->reset();
    }
    ret = _waitAck();
    return ret;
}

/**
 * @brief ����7bit�ӻ���ַλ.
 *
 * @param[in] slave_address 7bit�ӻ���ַλ.
 *
 * @return ���ͽ��.����0��ʾ���ͳɹ�������-1��ʾ����ʧ��.
 */
int8_t E_SOFTI2C::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
{
    int8_t ret = 0;
    // д���ӵ�ַ���λ��0�������ӵ�ַ���λ��1��
    slaveAddr = (WR == WRITE) ? (slaveAddr & 0xfe) : (slaveAddr | 0x01);
    ret = _sendByte(slaveAddr);
    return ret;
}

/**
  * @brief ��������.
  * @param ��.
  * @return ���յ�������.
  */
int8_t E_SOFTI2C::_receiveByte(uint8_t *data)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    _sda->set();
    while (i--)
    {
        byte += byte;
        _scl->reset(); // ʱ�Ӹߵ�ƽ�ڼ��SDA
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
  *@brief    ָ��λ��д��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
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
  *@brief    ��ָ���Ĵ���. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��   Ҫ��ȡ�ļĴ���
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
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
  *@brief    ��ָ���Ĵ�������д start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��Ҫд��ļĴ�����ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
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
  *@brief    ָ���Ĵ���������ȡ. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr: �Ĵ�����ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
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
