/**
  ******************************************************************************
  * @file    spi.cpp
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
#include "stm32f1xx_ll_spi.h"
#include "ebox_gpio.h"
#include "ebox_base.h"
#include "mcu_define.h"
#include "ebox_spi.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#define  SPI_DEBUG(...) DBG("[SPI]  "),DBG(__VA_ARGS__)
#else
#define  SPI_DEBUG(...)
#endif

E_SPI::E_SPI(SPI_TypeDef *SPIx, E_GPIO *sck, E_GPIO *miso, E_GPIO *mosi)
{
    _busy = 0;
    _spi = SPIx;
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
}

void E_SPI::begin(C_SpiConfig_t *spi_config)
{
    uint8_t index = 0;

    index = getIndex(_sck->id, SPI_MAP);
    _sck->mode(SPI_MAP[index]._pinMode, SPI_MAP[index]._pinAf);
    index = getIndex(_miso->id, SPI_MAP);
    _miso->mode(SPI_MAP[index]._pinMode, SPI_MAP[index]._pinAf);
    index = getIndex(_mosi->id, SPI_MAP);
    _mosi->mode(SPI_MAP[index]._pinMode, SPI_MAP[index]._pinAf);

    eboxRccCmd(RccTableSPI, (uint32_t)_spi, ENABLE);
    config(spi_config);
}

static const uint32_t d_time[] = {LL_SPI_BAUDRATEPRESCALER_DIV2, LL_SPI_BAUDRATEPRESCALER_DIV4,
                                  LL_SPI_BAUDRATEPRESCALER_DIV8, LL_SPI_BAUDRATEPRESCALER_DIV16,
                                  LL_SPI_BAUDRATEPRESCALER_DIV32, LL_SPI_BAUDRATEPRESCALER_DIV64,
                                  LL_SPI_BAUDRATEPRESCALER_DIV128, LL_SPI_BAUDRATEPRESCALER_DIV256};

void E_SPI::config(C_SpiConfig_t *spi_config)
{
    _devID = spi_config->devID;
    SPI_DEBUG("dev num 0x%x \r\n", current_dev_num);
    LL_SPI_Disable(_spi);

    switch (spi_config->mode)
    {
    case C_SpiMode0:
			  LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        break;
    case C_SpiMode1:
			  LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        break;
    case C_SpiMode2:
			  LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        break;
    case C_SpiMode3:
			  LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        break;
    default:
        break;
    }

    /* Configure SPI1 communication */
    LL_SPI_SetBaudRatePrescaler(_spi, d_time[spi_config->prescaler]);
    LL_SPI_SetTransferBitOrder(_spi, (spi_config->bit_order)?LL_SPI_MSB_FIRST:LL_SPI_LSB_FIRST);

    LL_SPI_SetTransferDirection(_spi, LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(_spi, LL_SPI_DATAWIDTH_8BIT);
    LL_SPI_SetNSSMode(_spi, LL_SPI_NSS_SOFT);

    LL_SPI_SetMode(_spi, LL_SPI_MODE_MASTER);
    LL_SPI_Enable(_spi);
}

/**
  *@brief    读取配置信息
  *@param    void
  *@retval   返回当前设备id
  */
uint8_t E_SPI::readConfig(void)
{
    return _devID;
}

/**
  *@brief    transfer
  *@param    data 要发送的数据
  *@retval   uint8_t 接受到的数据
  */
uint8_t E_SPI::transfer(uint8_t data)
{
    while ((_spi->SR & LL_SPI_SR_TXE) == RESET);
    *((__IO uint8_t *)&_spi->DR) = data;
    while ((_spi->SR & LL_SPI_SR_RXNE) == RESET);
    return (uint8_t)_spi->DR;
}
/**
  *@brief    写数据
  *@param    data 要写入的数据
  *@retval   none
  */
int8_t E_SPI::write(uint8_t data)
{
    return transfer(data);
//	    while ((_spi->SR & LL_SPI_SR_TXE) == RESET);
//    *((__IO uint8_t *)&_spi->DR) = data;
//	return (uint8_t)_spi->DR;
}

/**
  *@brief    连续写数据
  *@param    data 要写入的数据，data_length 长度
  *@retval   none
  */
int8_t E_SPI::write(uint8_t *data, uint16_t data_length)
{
    __IO uint8_t dummyByte;
    if (data_length == 0)
        return -1;
    while (data_length--)
    {
        transfer(*data++);
    }
    return 0;
}

uint8_t E_SPI::read()
{
    return transfer(0xff);

}

/**
  *@brief    连续读取数据
  *@param    recv_data 读取到的数据，data_length 要读取的数据长度
  *@retval   返回当前设备id
  */
int8_t E_SPI::read(uint8_t *recv_data, uint16_t data_length)
{
    if (data_length == 0)
        return -1;
    while (data_length--)
    {
        *recv_data++ = transfer(0xff);
    }
    return 0;
}

/**
  *@brief    获取控制权
  *@param    none
  *@retval   none
  */
int8_t E_SPI::take(C_SpiConfig_t *spi_config)
{
    while((_busy == 1) && (spi_config->devID != _devID))
        delayMs(1);
    if(spi_config->devID == _devID)
    {
        _busy = 1;
        return 0;
    }
    config(spi_config);
    _busy = 1;
    return 0;
}

/**
  *@brief    释放控制权
  *@param    none
  *@retval   none
  */
int8_t E_SPI::release(void)
{
    _busy = 0;
    return 0;
}



