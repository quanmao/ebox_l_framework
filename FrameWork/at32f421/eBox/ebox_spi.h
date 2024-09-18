/**
  ******************************************************************************
  * @file    spi.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   2021  ���ӵ���spiģʽ
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
	1.Ŀǰֻ������SPI1��SPI2��spi3�����Ѳ���
	2.��spi����ǿ������֧��ͬʱ���ز�ͬMODE ,SPEED,bit_oder���豸
	3.ÿһ��spi�豸Ӧ��һ���Լ���SPI_CONFIG�����ã���֧�ָ��豸�ĵĶ�д��
		�ڶ�дǰ��Ҫ���SPI�Ŀ���Ȩ�������ȡ������һֱ�ȴ�����Ҫ��Ϊ�˼��ݲ���ϵͳ��
		�����ʹ�ò���ϵͳҲ������ϻ�ÿ���Ȩ�Ĵ��룬��������SPI��һ��Ҫ�ͷ�SPI���ߣ�
		������ͷ����߻ᵼ�±��SPI�豸һֱ���ڵȴ���״̬
    4.������f0ϵ������pin���ã�����ʹ�ò�ͬ��io�����ʹ��spi��Դ��Ĭ��spi1ΪPA5��PA6��PA7
      Ҳ����ʹ��PB3��PA6��PA7��Ϊspi1����stm32f072_define�����ö��弴��
*/
//Ĭ������ �գ�ֻ�����ṹ��SPICONFIG����ʼ��

class E_SPI : public A_SPI
{
public:
    E_SPI(SPI_TypeDef *SPIx, E_GPIO *sck, E_GPIO *miso, E_GPIO *mosi);              // ��׼ģʽSPI,˫��ȫ˫��
    E_SPI(SPI_TypeDef *SPIx, E_GPIO *sck, E_GPIO *mosi, C_DIR_t wo = C_OneWrite);   // ����ģʽSPI, ʹ��MOSI�˿�,Ĭ��ֻд C_OneWrite; C_OneWriteRead ��д;

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

    uint8_t _dataWidth; // ���ݿ��
    uint8_t _writeOnly;
};

#endif
