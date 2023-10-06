/**
  ******************************************************************************
  * @file    ebox_i2c.cpp
  * @author  cat_li
  * @brief   ����������ģʽ
		1  2017/5/30  ���ӳ�ʱ����ֹ������������д�������ӷ���״̬
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
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_i2c.h"
#include "ebox_gpio.h"
#include "ebox_base.h"
#include "mcu_define.h"
#include "ebox_i2c.h"


#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[I2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

// ��Ƶ�����������֣��ߵ�ƽ���͵�ƽ
#define	C16M10K	 	__LL_I2C_CONVERT_TIMINGS(0x3, 0xc7, 0xc3, 0x02, 0x04)		//10k@16M
#define	C16M100K	__LL_I2C_CONVERT_TIMINGS(0x3, 0x13, 0xf, 0x02, 0x04)		//100k@16M
#define	C16M400K	__LL_I2C_CONVERT_TIMINGS(0x1, 0x9, 0x3, 0x2, 0x3)			//400k@16M

#define	C48M10K	 	0xA010B2FF		//10k@48M,stand mode
#define	C48M100K	0x10805E89		//100k@48M,stand mode
#define C48M200K    0x00905E82      //200K@48M,Fast mode
#define	C48M400K	0x00901850   	//400k@48M,Fast mode

uint32_t const TIMING[] = {C48M10K,C48M100K,C48M200K,C48M400K};
#define	C8M10K	 	0x101098F3		//10k@8M,stand mode
#define	C8M100K	 	0x00201D2B		//100k@8M,stand mode

/**
 *@name     I2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
 *@brief      I2C���캯��
 *@param    I2Cx:  I2C1,I2C2
 *          scl_pin:  ʱ��Pin
 *          sda_pin:  ����Pin
 *@retval   None
*/
E_I2C::E_I2C(I2C_TypeDef *I2Cx, E_GPIO *scl_pin, E_GPIO *sda_pin)
{
    _busy = 0;
    _i2cx = I2Cx;
    _scl = scl_pin;
    _sda = sda_pin;
}

/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing��8M��48M��ƵĬ��100k������Ƶ��Ĭ��10k @8M
  *@param    speed:  ���� 10,100��200,400 �ֱ����10k��100k��200k��400k
  *@retval   None
  */
void  E_I2C::begin(Speed_t speed)
{
    uint8_t index = 0;
    index = getIndex(_scl->id, I2C_MAP);
    _scl->mode(I2C_MAP[index]._pinMode, I2C_MAP[index]._pinAf);
    index = getIndex(_sda->id, I2C_MAP);
    _sda->mode(I2C_MAP[index]._pinMode, I2C_MAP[index]._pinAf);
    config(speed);
}

void E_I2C::config(Speed_t speed)
{
    Clock_t clock;
    eboxGetClockInfo(&clock);
    _speed = speed;
    if (clock.pclk1 == 48000000)
    {
        _timing = TIMING[speed];
    }
    else
    { // ��48M��Ƶ��һ�ɰ�8M��Ƶ����
        _timing = (speed == K10) ? (C8M10K) : (C8M100K);
    }

    eboxRccCmd(RccTableI2C, (uint32_t)_i2cx, ENABLE);
    // I2C1 ��Ҫѡ���ض���ʱ��
    (_i2cx == I2C1) ? (LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK)) : (void());

    LL_I2C_Disable(_i2cx);
    LL_I2C_SetTiming(_i2cx, _timing);
    LL_I2C_Enable(_i2cx);
}

/**
  *@brief    I2Cд��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t E_I2C::write(uint16_t slaveAddr, uint8_t data)
{
 #if USE_TIMEOUT
     uint32_t end = ENDTIME(timeout);
 #endif
     LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

     while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
     {
         if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
         {
             LL_I2C_TransmitData8(_i2cx, data);
         }
 #if USE_TIMEOUT
         if (TIMEOUT(end, timeout)) return EWAIT;
 #endif
     }
     LL_I2C_ClearFlag_STOP(_i2cx);
		return EOK;
//    return write_buf(slaveAddr,&data,1);
}

/**
  *@brief    I2C����д start->data....->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data:  Ҫд�������
  *          uint16_t nWrite  Ҫд������ݳ���
  *          uint16_t tOut:  ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t E_I2C::write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite)
{
#if (USE_TIMEOUT != 0)
    uint32_t end = ENDTIME(tOut);
#endif

#if MAXWRITE
    if (nWrite > 255)
    {
        // num_to_writ>255: RELOAD,NBYTE=0xFF,START
        LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 0xFF, LL_I2C_MODE_RELOAD, LL_I2C_GENERATE_START_WRITE);

        while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx, *data++);
            }
#if (USE_TIMEOUT != 0)
            if (TIMEOUT(end, tOut))
                return EWAIT;
#endif
        }
        nWrite = nWrite - 255;
        // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART

        while (nWrite > 255)
        {
            // ���͵�ַ�Ĵ���
            LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 0xFF, LL_I2C_MODE_RELOAD, LL_I2C_GENERATE_NOSTARTSTOP);
#if (USE_TIMEOUT != 0)
            end = ENDTIME(tOut);
#endif
            while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
            {
                if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
                {
                    LL_I2C_TransmitData8(_i2cx, *data++);
                }
#if (USE_TIMEOUT != 0)
                if (TIMEOUT(end, tOut))
                    return EWAIT;
#endif
            }
            nWrite = nWrite - 255;
        }
        // num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
        LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, nWrite, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_NOSTARTSTOP);
#if (USE_TIMEOUT != 0)
        end = ENDTIME(tOut);
#endif
        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx, *data++);
            }
#if (USE_TIMEOUT != 0)
            if (TIMEOUT(end, tOut))
                return EWAIT;
#endif
        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }
    else
#endif
    {
        // д���ַ�Ĵ���������
        LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, nWrite, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
#if (USE_TIMEOUT != 0)
        end = ENDTIME(tOut);
#endif
        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx, *data++);
            }
#if (USE_TIMEOUT != 0)
            if (TIMEOUT(end, tOut))
                return EWAIT;
#endif
        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }

    return EOK;
}

/**
  *@brief    I2C����һ���ֽ�. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
  */
uint8_t E_I2C::read(uint16_t slaveAddr)
{
    uint8_t ret = 0;
    read_buf(slaveAddr, &ret, 1);
    return ret;
}

/**
  *@brief    ������ȡ. start->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          uint16_t tOut: ��ʱ
  *@retval   EOK��EWAIT
  */
uint8_t E_I2C::read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead)
{
#if USE_TIMEOUT
    uint32_t end = ENDTIME(timeout);
#endif
    // ���Ͷ�ָ��ӵ�ǰ��ַ��ʼ��ȡ����
    LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, nRead, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
        {
            *data++ = LL_I2C_ReceiveData8(_i2cx);
        }
#if USE_TIMEOUT
        if (TIMEOUT(end, timeout)) return EWAIT;
#endif
    }
    LL_I2C_ClearFlag_STOP(_i2cx);
    return EOK;
}




/**
  *@brief    �ȴ��豸��Ӧ����ָ���豸����startָ�����豸æ���򷵻�NACK,���򷵻�ACK,���豸����stopָ��
  *@param    slaveAddr:  �豸��ַ
  *@retval   uint8_t: EOK,EWAIT
  */
uint8_t E_I2C::check_busy(uint16_t slaveAddr)
{
    uint8_t tmp = 0;
    uint32_t end = ENDTIME(timeout);
    do
    {
        // clear STOP & NACK flag
        SET_BIT(_i2cx->ICR, LL_I2C_ICR_NACKCF | LL_I2C_ICR_STOPCF);
        LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 0, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
        // �ȴ�����ֹͣλ�������
        while (!LL_I2C_IsActiveFlag_STOP(_i2cx));
        LL_I2C_ClearFlag_STOP(_i2cx);
        if (TIMEOUT(end, timeout))
        {
            I2C_DEBUG("fail,ISR reg is %d \r\n", _i2cx->ISR);
            return EWAIT;
        }
        tmp = LL_I2C_IsActiveFlag_NACK(_i2cx);
    } while (tmp == 1); //�������Ӧ��������ȴ�
    return EOK;
}

/**
  *@brief    ��ȡI2C����Ȩ
  *@param    timing:  ʱ��ʱ��ͨ��readConfig��ȡ
  *@retval   uint8_t: EOK,E_BUSY
  */
uint8_t E_I2C::take(Speed_t speed)
{
    uint32_t end = ENDTIME(timeout);
    while (_busy == 1)
    {
        delayMs(1);
        if (TIMEOUT(end, timeout)) return EWAIT;
    }
    if (_speed != speed)  config(speed);
    _busy = 1;
    return EOK;
}
/**
 *@brief    �ͷ�I2C����Ȩ
 *@param    none
 *@retval   none
*/
void E_I2C::release(void)
{
    _busy = 0;
}

#if USE_REGADDR
/**
  *@brief    ָ��λ��д��һ���ֽ�. start->data->stop
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t data:  Ҫд�������
  *          uint16_t tOut: ��ʱ
  *@retval   ״̬ EOK �ɹ��� EWAIT ��ʱ
  */
uint8_t E_I2C::write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data)
{
    uint32_t end = ENDTIME(timeout);

    LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx, regAddr);
        }
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx, data);
        }
        if (TIMEOUT(end, timeout)) return EWAIT;
    }
    LL_I2C_ClearFlag_STOP(_i2cx);

    return EOK;
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
uint8_t E_I2C::write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite)
{
    uint32_t end = ENDTIME(timeout);

    // ���͵�ַ�Ĵ���
    LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 1, LL_I2C_MODE_RELOAD, LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx, regAddr);
        }
        if (TIMEOUT(end, timeout)) return EWAIT;

    }
    // ��������
    if (nWrite > 255)
    {
        // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART
        while (nWrite > 255)
        {
            // ���͵�ַ�Ĵ���
            LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 0xFF, LL_I2C_MODE_RELOAD, LL_I2C_GENERATE_NOSTARTSTOP);
            end = ENDTIME(timeout);

            while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
            {
                if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
                {
                    LL_I2C_TransmitData8(_i2cx, *data++);
                }
                if (TIMEOUT(end, timeout)) return EWAIT;
            }
            nWrite = nWrite - 255;
        }
        // num_to_writ<=255: AUTOEND,NBYTE=num_to_writ,NOSTART
        LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, nWrite, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_NOSTARTSTOP);
        end = ENDTIME(timeout);

        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx, *data++);
            }
            if (TIMEOUT(end, timeout)) return EWAIT;

        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }
    else
    {
        // д���ַ�Ĵ���������
        LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, nWrite, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_NOSTARTSTOP);
        
        end = ENDTIME(timeout);
        while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
        {
            if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
            {
                LL_I2C_TransmitData8(_i2cx, *data++);
            }
            if (TIMEOUT(end, timeout)) return EWAIT;
        }
        LL_I2C_ClearFlag_STOP(_i2cx);
    }
    return EOK;
}

/**
  *@brief    ��ָ���Ĵ���. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint8_t regAddr��   Ҫ��ȡ�ļĴ���
  *          uint16_t tOut: ��ʱ
  *@retval   ��ȡ��������
  */
uint8_t E_I2C::read(uint16_t slaveAddr, uint16_t regAddr)
{
    uint8_t ret = 0;
    read_buf(slaveAddr, regAddr, &ret, 1);
    return ret;
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
uint8_t E_I2C::read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead)
{
    uint32_t end = ENDTIME(timeout);

    //   while(LL_I2C_IsActiveFlag_STOP(_i2cx)) I2C_DEBUG("ISR reg is %d \r\n",_i2cx->ISR);;
    // ���͵�ַ�Ĵ���
    LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    while (!LL_I2C_IsActiveFlag_TC(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx, regAddr);
        }
        if (TIMEOUT(end, timeout))
        {
            return EWAIT;
        }

    }
    // ���Ͷ�ָ��ӵ�ǰ��ַ��ʼ��ȡ����
    LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, nRead, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    end = ENDTIME(timeout);

    while (!LL_I2C_IsActiveFlag_STOP(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_RXNE(_i2cx))
        {
            *data++ = LL_I2C_ReceiveData8(_i2cx);
        }
        if (TIMEOUT(end, timeout)) return EWAIT;

    }
    LL_I2C_ClearFlag_STOP(_i2cx);
    return EOK;
}
#endif
