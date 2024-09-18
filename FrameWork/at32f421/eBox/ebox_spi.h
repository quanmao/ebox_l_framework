/**
  ******************************************************************************
  * @file    spi.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   2021  增加单线spi模式
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

#ifndef __SPI_H
#define __SPI_H

#include "ebox_config.h"
#include "ebox_spi_api.h"

/*
	1.目前只测试了SPI1、SPI2，spi3望网友测试
	2.该spi功能强大，总线支持同时挂载不同MODE ,SPEED,bit_oder的设备
	3.每一个spi设备应有一个自己的SPI_CONFIG的配置，以支持该设备的的读写，
		在读写前需要获得SPI的控制权，如果获取不到则一直等待！主要是为了兼容操作系统，
		如果不使用操作系统也必须加上获得控制权的代码，在是用完SPI后一定要释放SPI总线，
		如果不释放总线会导致别的SPI设备一直处于等待的状态
    4.得益于f0系列灵活的pin配置，可以使用不同的io组合来使用spi资源，默认spi1为PA5，PA6，PA7
      也可以使用PB3，PA6，PA7作为spi1，在stm32f072_define中做好定义即可
*/
//默认配置 空，只依靠结构体SPICONFIG来初始化

class E_SPI : public A_SPI
{
public:
    E_SPI(SPI_TypeDef *SPIx, E_GPIO *sck, E_GPIO *miso, E_GPIO *mosi);              // 标准模式SPI,双线全双工
    E_SPI(SPI_TypeDef *SPIx, E_GPIO *sck, E_GPIO *mosi, C_DIR_t wo = C_OneWrite);   // 单线模式SPI, 使用MOSI端口,默认只写 C_OneWrite; C_OneWriteRead 读写;

    virtual void begin(C_SpiConfig_t *spi_config);
    virtual void config(C_SpiConfig_t *spi_config);
    virtual uint8_t readConfig(void);

    virtual DATA_T transfer(DATA_T data);

    virtual int8_t write(DATA_T data);
    virtual DATA_T read();

    virtual int8_t write(uint8_t *data, uint16_t len);
    virtual int8_t read(uint8_t *recv_data, uint16_t len);

    virtual int8_t take(C_SpiConfig_t *spi_config);
    virtual int8_t release(void);

private:
    SPI_TypeDef *_spi;
    E_GPIO *_sck;
    E_GPIO *_miso;
    E_GPIO *_mosi;

    uint8_t _dataWidth; // 数据宽度
    uint8_t _writeOnly;
};

#endif
