/**
  ******************************************************************************
  * @file    ebox_i2c.cpp
  * @author  cat_li
  * @brief   仅工作在主模式
		1  2017/5/30  增加超时，防止程序死掉。读写函数增加返回状态
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
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_i2c.h"
#include "ebox_gpio.h"
#include "ebox_base.h"
#include "mcu_define.h"
#include "ebox_i2c.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#define  I2C_DEBUG(...) DBG("[I2C]  "),DBG(__VA_ARGS__)
#else
#define  I2C_DEBUG(...)
#endif

uint32_t const TIMING[] = {10000,100000,200000,400000};
/**
 *@name     I2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin)
 *@brief      I2C构造函数
 *@param    I2Cx:  I2C1,I2C2
 *          scl_pin:  时钟Pin
 *          sda_pin:  数据Pin
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
  *@brief    根据i2c时钟和设置速率speed计算timing。8M，48M主频默认100k，其他频率默认10k @8M
  *@param    speed:  速率 10,100，200,400 分别代表10k，100k，200k，400k
  *@retval   None
  */
void E_I2C::begin(Speed_t speed)
{
    eboxRccCmd(RccTableI2C, (uint32_t)_i2cx, ENABLE);

    _scl->mode(AF_OD_PU);
    _sda->mode(AF_OD_PU);

    config(speed);
}

void E_I2C::config(Speed_t speed)
{
    Clock_t clock;
    eboxGetClockInfo(&clock);
    _speed = speed;
    _timing = TIMING[speed];
    LL_I2C_Disable(_i2cx);
    LL_I2C_ConfigSpeed(I2C1, clock.pclk1, _timing, LL_I2C_DUTYCYCLE_2);
    LL_I2C_Enable(_i2cx);
}

/**
  *@brief    I2C写入一个字节. start->data->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t data:  要写入的数据
  *          uint16_t tOut: 超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t E_I2C::write(uint16_t slaveAddr, uint8_t data)
{
    uint8_t err = EOK;
    err = _start();
    err = _send7bitsAddress(slaveAddr, WRITE);
    err = _sendByte(data);
    LL_I2C_GenerateStopCondition(I2C1);
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
uint8_t E_I2C::write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite)
{
    uint8_t err = EOK;
    err = _start();
    err = _send7bitsAddress(slaveAddr, WRITE);
    while(nWrite){
        err = _sendByte(*data++);
    }    
    LL_I2C_GenerateStopCondition(I2C1);
    return err;
}

/**
  *@brief    I2C读入一个字节. start->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t E_I2C::read(uint16_t slaveAddr)
{
    uint8_t data;
    _start();
    _send7bitsAddress(slaveAddr, READ);
    LL_I2C_AcknowledgeNextData(_i2cx, LL_I2C_NACK);
    LL_I2C_GenerateStopCondition(_i2cx);
    _receiveByte(&data);
    LL_I2C_AcknowledgeNextData(_i2cx, LL_I2C_ACK);
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
uint8_t E_I2C::read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead)
{
    uint8_t err = 0;
    err += _start();
    err += _send7bitsAddress(slaveAddr, READ);
    while (nRead--)
    {
        if (nRead == 0)
        {
            LL_I2C_AcknowledgeNextData(_i2cx, LL_I2C_NACK);
            LL_I2C_GenerateStopCondition(_i2cx);
        }
        err += _receiveByte(data);
        data++;
    }
    LL_I2C_AcknowledgeNextData(_i2cx, LL_I2C_ACK);
    return err;
}

/**
  *@brief    等待设备响应。向指定设备发送start指令，如果设备忙，则返回NACK,否则返回ACK,主设备发送stop指令
  *@param    slaveAddr:  设备地址
  *@retval   uint8_t: EOK,EWAIT
  */
uint8_t E_I2C::check_busy(uint16_t slaveAddr)
{
    uint8_t tmp = 0;
    uint32_t end = ENDTIME(timeout);
    do
    {
        _start();
        tmp = _send7bitsAddress(slaveAddr, WRITE);
        LL_I2C_ClearFlag_STOP(_i2cx);
        LL_I2C_GenerateStopCondition(I2C1);
        if (TIMEOUT(end, timeout))
        {
            return EWAIT;
        }
    } while (tmp == EWAIT);
    return EOK;
}

/**
  *@brief    获取I2C控制权
  *@param    timing:  时钟时序，通过readConfig获取
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
 *@brief    释放I2C控制权
 *@param    none
 *@retval   none
*/
void E_I2C::release(void)
{
    _busy = 0;
}

int8_t E_I2C::_start()
{
//#if USE_TIMEOUT
    uint32_t end = ENDTIME(timeout);
//#endif
    /* (1) Prepare acknowledge for Master data reception ************************/
    LL_I2C_AcknowledgeNextData(_i2cx, LL_I2C_ACK);
    /* (2) Initiate a Start condition to the Slave device ***********************/
    /* Master Generate Start condition */
    LL_I2C_GenerateStartCondition(_i2cx);
    /* (3) Loop until Start Bit transmitted (SB flag raised) ********************/
    /* Loop until SB flag is raised  */
    while (!LL_I2C_IsActiveFlag_SB(_i2cx))
    {
//#if USE_TIMEOUT
        if (TIMEOUT(end, timeout))
            return EWAIT;
//#endif
    }
    return EOK;
}

int8_t E_I2C::_sendByte(uint8_t byte)
{
#if USE_TIMEOUT
    uint32_t end = ENDTIME(timeout);
#endif
    /* (6.1) Transmit data (TXE flag raised) **********************************/
    /* Check TXE flag value in ISR register */
    while (!LL_I2C_IsActiveFlag_TXE(_i2cx))
    {
#if USE_TIMEOUT
        if (TIMEOUT(end, timeout))
            return EWAIT
#endif
    }
    /* Write data in Transmit Data register.
      TXE flag is cleared by writing data in TXDR register */
    LL_I2C_TransmitData8(_i2cx, byte);
    return EOK;
}

int8_t E_I2C::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
{
//#if USE_TIMEOUT
    uint32_t end = ENDTIME(timeout);
//#endif
    /* (4) Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a write request */
    LL_I2C_TransmitData8(_i2cx, (slaveAddr&0xfe) | WR);
    /* (5) Loop until Address Acknowledgement received (ADDR flag raised) *******/
    /* Loop until ADDR flag is raised  */
    while (!LL_I2C_IsActiveFlag_ADDR(_i2cx))
    {
//#if USE_TIMEOUT
        if (TIMEOUT(end, timeout))
            return EWAIT;
//#endif
    }
    /* (6) Clear ADDR flag and loop until end of transfer (ubNbDataToTransmit == 0) */
    LL_I2C_ClearFlag_ADDR(_i2cx);
    return EOK;
}

int8_t E_I2C::_receiveByte(uint8_t *data)
{
    uint32_t end = ENDTIME(timeout);
    while (!LL_I2C_IsActiveFlag_RXNE(_i2cx))
    {
        if (TIMEOUT(end, timeout))
        {
            I2C_DEBUG("read data fail, state reg SR2 = %d£¬SR1 = %d \r\n", _i2cx->SR2, _i2cx->SR1);
            return EWAIT;
        }
    }
    *data = (uint8_t)(READ_BIT(_i2cx->DR, I2C_DR_DR));
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
  *@brief    在指定寄存器连续写 start->regAddr->data....->stop
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：要写入的寄存器地址
  *          uint8_t *data:  要写入的数据
  *          uint16_t nWrite  要写入的数据长度
  *          uint16_t tOut:  超时
  *@retval   状态 EOK 成功； EWAIT 超时
  */
uint8_t E_I2C::write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite)
{
    uint32_t end = ENDTIME(timeout);

    // 发送地址寄存器
    LL_I2C_HandleTransfer(_i2cx, slaveAddr, LL_I2C_ADDRESSING_MODE_7BIT, 1, LL_I2C_MODE_RELOAD, LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_TCR(_i2cx))
    {
        if (LL_I2C_IsActiveFlag_TXIS(_i2cx))
        {
            LL_I2C_TransmitData8(_i2cx, regAddr);
        }
        if (TIMEOUT(end, timeout)) return EWAIT;

    }
    // 发送数据
    if (nWrite > 255)
    {
        // num_to_writ>255: RELOAD,NBYTE=0xFF,NOSTART
        while (nWrite > 255)
        {
            // 发送地址寄存器
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
        // 写入地址寄存器和数据
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
  *@brief    读指定寄存器. start->WslaveAddr->regAddr->RslaveAddr->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint8_t regAddr：   要读取的寄存器
  *          uint16_t tOut: 超时
  *@retval   读取到的数据
  */
uint8_t E_I2C::read(uint16_t slaveAddr, uint16_t regAddr)
{
    uint8_t ret = 0;
    read_buf(slaveAddr, regAddr, &ret, 1);
    return ret;
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
uint8_t E_I2C::read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead)
{
    uint32_t end = ENDTIME(timeout);

    //   while(LL_I2C_IsActiveFlag_STOP(_i2cx)) I2C_DEBUG("ISR reg is %d \r\n",_i2cx->ISR);;
    // 发送地址寄存器
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
    // 发送读指令，从当前地址开始读取数据
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
