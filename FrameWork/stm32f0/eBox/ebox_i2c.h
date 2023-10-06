/**
  ******************************************************************************
  * @file    ebox_i2c.h
  * @author  cat_li
  * @brief   ����������ģʽ
		1    2017/5/30  ���ӳ�ʱ����ֹ������������д�������ӷ���״̬
		2    2017/7/24  �޸ĳ�ʱģʽ,I2C2��֤ok,���Ӷ��ֽ�д�ֽں���
  *	ע�⣺��ȡ�ͷ�Ȩ�޳���Ҫ�Ż���
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

#ifndef __EBOX_I2C_H_
#define __EBOX_I2C_H_

#include "ebox_config.h"

#define USE_REGADDR     1
#define MAXWRITE        0

class API_I2C
{
public:
    typedef enum
    {
        K10,
        K100,
        K200,
        K400
    } Speed_t;

public:
    API_I2C() : timeout(200){};

    virtual void begin(Speed_t speed) = 0;
    virtual void config(Speed_t speed) = 0;
    // ���ֽڶ�д
    virtual uint8_t write(uint16_t slaveAddr, uint8_t data) = 0;
    virtual uint8_t read(uint16_t slaveAddr) = 0;
    // ��дn�ֽ�
    virtual uint8_t write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t num_to_write) = 0;
    virtual uint8_t read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t num_to_read) = 0;
#if USE_REGADDR
    virtual uint8_t write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data) = 0;
    virtual uint8_t read(uint16_t slaveAddr, uint16_t regAddr) = 0;
    virtual uint8_t write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t num_to_write) = 0;
    virtual uint8_t read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t num_to_read) = 0;
#endif
    // �ȴ��豸��Ӧ
    virtual uint8_t check_busy(uint16_t slaveAddr) = 0;

    // ��ȡI2C����Ȩ,�ɹ�����E_OK,E_BUSY;��Ҫ��releaseRight�ɶ�ʹ��
    virtual uint8_t take(Speed_t speed) = 0;
    // �ͷ�I2C����Ȩ
    virtual void release(void) = 0;

protected:
    typedef enum
    {
        WRITE = 0,
        READ = 1
    } RW_t;
    uint16_t timeout;
};

class E_I2C : public API_I2C
{
public:
    E_I2C(I2C_TypeDef *I2Cx, E_GPIO *scl_pin, E_GPIO *sda_pin);
    // ����I2C, speed - 10,100,400 �ֱ����10k��100k��400k
    virtual void begin(Speed_t speed);
    virtual void config(Speed_t speed);
    // ���ֽڶ�д
    virtual uint8_t write(uint16_t slaveAddr, uint8_t data);
    virtual uint8_t read(uint16_t slaveAddr);
    // ��дn�ֽ�
    virtual uint8_t write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite);
    virtual uint8_t read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead);
#if USE_REGADDR
    virtual uint8_t write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data);
    virtual uint8_t read(uint16_t slaveAddr, uint16_t regAddr);
    virtual uint8_t write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite);
    virtual uint8_t read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead);
#endif
    // �ȴ��豸��Ӧ
    virtual uint8_t check_busy(uint16_t slaveAddr);
    // ��ȡI2C����Ȩ,�ɹ�����E_OK,E_BUSY;��Ҫ��releaseRight�ɶ�ʹ��
    virtual uint8_t take(Speed_t speed);
    // �ͷ�I2C����Ȩ
    virtual void release(void);

private:
    I2C_TypeDef *_i2cx; // i2c����
    Speed_t _speed;     // i2cʱ��
    uint32_t _timing;   // i2cʱ��
    uint8_t _busy;
    E_GPIO *_sda;
    E_GPIO *_scl;
};

/*
	1.֧���κ�IO���ţ�
	2.�����ӿں�Ӳ��I2C��ȫһ�����Ի������
	ע�⣺
				1.�����speed����delay_us��ʱ�������ơ����в�׼
				2.speed����ֻ��Ϊ100000��200000,300k,400k��������Ǵ�ֵ����Ὣspeed��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(speed);
				3.���ڵ���I2C�豸����ʹ��100k�����ߴ���10��ֵ
*/
class E_SOFTI2C : public API_I2C
{
public:
    E_SOFTI2C(E_GPIO *scl, E_GPIO *sda);
    // ����I2C, speed - 10,100,400 �ֱ����10k��100k��400k
    virtual void begin(Speed_t speed);
    virtual void config(Speed_t speed);
    // ���ֽڶ�д
    virtual uint8_t write(uint16_t slaveAddr, uint8_t data);
    virtual uint8_t read(uint16_t slaveAddr);
    // ��дn�ֽ�
    virtual uint8_t write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite);
    virtual uint8_t read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead);

#if USE_REGADDR
    virtual uint8_t write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data);
    virtual uint8_t read(uint16_t slaveAddr, uint16_t regAddr);
    virtual uint8_t write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite);
    virtual uint8_t read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead);
#endif
    // �ȴ��豸��Ӧ
    virtual uint8_t check_busy(uint16_t slaveAddr);
    // ��ȡI2C����Ȩ,�ɹ�����E_OK,E_BUSY;��Ҫ��releaseRight�ɶ�ʹ��
    virtual uint8_t take(Speed_t speed);
    // �ͷ�I2C����Ȩ
    virtual void release(void);

private:
    int8_t _start();
    void _stop();
    int8_t _sendAck();
    int8_t _sendNack();

    int8_t _sendByte(uint8_t byte);
    int8_t _send7bitsAddress(uint8_t slaveAddr, uint8_t WR);
    int8_t _receiveByte(uint8_t *data);

    int8_t _waitAck();

private:
    E_GPIO *_sda;
    E_GPIO *_scl;
    Speed_t _speed;   // i2cʱ��
    uint32_t _timing; // i2cʱ��
    uint8_t _busy;
};

#endif
