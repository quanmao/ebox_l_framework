/**
  ******************************************************************************
  * @file    E_SSPI.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   2020/10/10 软件spi改名为E_SSPI,修改底层读写逻辑
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
#include "ebox_spi.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 1
#endif

#if debug
#define  SPI_DEBUG(...) DBG("[SPI]  "),DBG(__VA_ARGS__)
#else
#define  SPI_DEBUG(...)
#endif

/*
     *CPOL,时钟极性，空闲时时钟信号的电平，CPHA时钟相位，即MISO在SCK的第几个边沿采样（0，第一个，1第二个）
     *mode0: CPOL := 0, CPHA := 0 => INIT = 0, PRE = Z|0, MID = 1, POST =  0
     *mode1: CPOL := 0, CPHA := 1 => INIT = 0, PRE =  1 , MID = 0, POST = Z|0
     *mode2: CPOL := 1, CPHA := 0 => INIT = 1, PRE = Z|1, MID = 0, POST =  1
     *mode3: CPOL := 1, CPHA := 1 => INIT = 1, PRE =  0 , MID = 1, POST = Z|1
     */

E_SSPI::E_SSPI(E_GPIO *sck, E_GPIO *miso, E_GPIO *mosi)
{
    _sck =  sck;
    _miso = miso;
    _mosi = mosi;
    _writeOnly = C_Norm;
}

E_SSPI::E_SSPI(E_GPIO *sck, E_GPIO *mosi, C_DIR_t wo)
{
    _sck = sck;
    _mosi = mosi;
    _miso = mosi;
    _writeOnly = wo;
}

void E_SSPI::begin(C_SpiConfig_t *spi_config)
{
    _sck->mode(OUTPUT_PP);
    if(!_writeOnly) _miso->mode(INPUT);
    _mosi->mode(OUTPUT_PP);

    config(spi_config);
}

static const uint8_t d_time[] = {1,1,2,4,8,16,32,64};

void E_SSPI::config(C_SpiConfig_t *spi_config)
{
    _devID = spi_config->devID;
    _bitOrder = spi_config->bit_order;
    spi_delay = d_time[spi_config->prescaler];

    if (4 <= spi_config->dataWidth && spi_config->dataWidth <= 16)
        _dataWidth = spi_config->dataWidth;
    else
        _dataWidth = 8;
    
    switch (spi_config->mode)
    {
    case C_SpiModeLowRise0:
        _cpol = 0;
        _cpha = 0;
        break;
    case C_SpiModeLowFall1:
        _cpol = 0;
        _cpha = 1;
        break;
    case C_SpiModeHighFall2:
        _cpol = 1;
        _cpha = 0;
        break;
    case C_SpiModeHighRise3:
        _cpol = 1;
        _cpha = 1;
        break;
    }
    _sck->write(_cpol); // 空闲时钟
}
uint8_t E_SSPI::readConfig(void)
{
    return _devID;
}

DATA_T E_SSPI::_lsb(DATA_T data){
    DATA_T RcvData = 0, i;
    if (!_cpha) // 第1个边沿发送数据
    {
        for (i = 0; i < _dataWidth; i++)
        {
            _mosi->write(data & (1 << i)); // 发送数据，并延时一段时间，确保从机接收
            delayUs(spi_delay);
            _sck->toggle(); // 产生第一个边沿，并延时一段时间，等待从机准备数据
            delayUs(spi_delay);
            RcvData |= _miso->read() << i; // 读取数据
            _sck->toggle();                // 产生第二个边沿
        }
    }
    else
    {
        for (i = 0; i < _dataWidth; i++)
        {
            _sck->toggle();                // 产生第一个边沿
            _mosi->write(data & (1 << i)); // 发送数据
            delayUs(spi_delay);
            _sck->toggle(); // 产生第二个边沿
            delayUs(spi_delay);
            RcvData |= _miso->read() << i; // 读取数据
        }
    }
    return RcvData;
}

DATA_T E_SSPI::_msb(DATA_T data)
{
    DATA_T RcvData = 0;
    int8_t  i;
    if (!_cpha) // 第1个边沿发送数据
    {
        for (i = _dataWidth-1; i >= 0; i--)
        {
            _mosi->write(data & (1 << i)); // 发送数据，并延时一段时间，确保从机接收
            delayUs(spi_delay);
            _sck->toggle(); // 产生第一个边沿，并延时一段时间，等待从机准备数据
            delayUs(spi_delay);
            RcvData |= _miso->read() << i; // 读取数据
            _sck->toggle();                // 产生第二个边沿
        }
    }
    else
    {
        for (i = _dataWidth-1; i >= 0; i--)
        {
            _sck->toggle();                // 产生第一个边沿
            _mosi->write(data & (1 << i)); // 发送数据
            delayUs(spi_delay);
            _sck->toggle(); // 产生第二个边沿
            delayUs(spi_delay);
            RcvData |= _miso->read() << i; // 读取数据
        }
    }
    return RcvData;
}

DATA_T E_SSPI::transfer(DATA_T data)
{
    if (_bitOrder == MSB_FIRST)
    return _msb(data);
    else
    return _lsb(data);
}

// void E_SSPI::_format(DATA_T &data){
//     DATA_T val = data;
//     data = ((val & 0x01) << 7) |
//         ((val & 0x02) << 5) |
//         ((val & 0x04) << 3) |
//         ((val & 0x08) << 1) |
//         ((val & 0x10) >> 1) |
//         ((val & 0x20) >> 3) |
//         ((val & 0x40) >> 5) |
//         ((val & 0x80) >> 7);
// }

int8_t E_SSPI::write(DATA_T data)
{
    if (_writeOnly == C_OneWriteRead)
        _mosi->mode(OUTPUT_PP);
    return transfer(data);
}
int8_t  E_SSPI::write(uint8_t *data, uint16_t len)
{
    if(len == 0)
        return -1;
    if (_writeOnly == C_OneWriteRead)
        _mosi->mode(OUTPUT_PP);
    while(len--)
    {
        transfer(*data++);
    }
    return len;
}

DATA_T E_SSPI::read()
{
    if (_writeOnly == C_OneWriteRead)
        _miso->mode(INPUT);
#if USE_SPI_WORD
    return transfer(0xffff);
#else
    return transfer(0xff);
#endif
}

int8_t  E_SSPI::read(uint8_t *rcvdata, uint16_t len)
{
    if(len == 0)
        return -1;
    if (_writeOnly == C_OneWriteRead)
        _miso->mode(INPUT);
    while(len--)
    {
        *rcvdata++ = transfer(0xff);
    }
    return len;
}

int8_t E_SSPI::take(C_SpiConfig_t *spi_config)
{
    while((busy == 1) && (spi_config->devID != _devID))
        delayMs(1);
    // if(spi_config->devID == _devID)
    // {
    //     busy = 1;
    //     return 0;
    // }
    config(spi_config);
    busy = 1;
    return 0;
}

int8_t E_SSPI::release(void)
{
    busy = 0;
    return 0;
}




