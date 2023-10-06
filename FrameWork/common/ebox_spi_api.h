/**
  ******************************************************************************
  * @file    ebox_interface_spi.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/202
  * @brief   spi����ӿ�
  *          2021/7/7  �޸�mode����
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_SPI_API_H
#define __EBOX_SPI_API_H

#include "mcu_config.h"
#include "mcu_define.h"

#ifndef DATA_T
	#define DATA_T uint8_t
#endif

typedef enum
{
  C_SpiModeLowRise0 = 0,    // CPOL0,CPHA1  �͵�ƽ��������
  C_SpiModeLowFall1 = 1,    // CPOL0,CPHA2  �͵�ƽ���½���
  C_SpiModeHighFall2 = 2,   // CPOL1,CPHA1  �ߵ�ƽ���½���
  C_SpiModeHighRise3 = 3    // CPOL1,CPHA2  �ߵ�ƽ��������
} C_SpiMode_t;
// define ���㲻ͬ�ķ�ʽ����
#define C_SPIMODE0 C_SpiModeLowRise0
#define C_SPIMODE1 C_SpiModeLowFall1
#define C_SPIMODE2 C_SpiModeHighFall2
#define C_SPIMODE3 C_SpiModeHighRise3

typedef enum
{
  C_SpiDIV2 = 0,
  C_SpiDIV4 = 1,
  C_SpiDIV8 = 2,
  C_SpiDIV16 = 3,
  C_SpiDIV32 = 4,
  C_SpiDIV64 = 5,
  C_SpiDIV128 = 6,
  C_SpiDIV256 = 7
} C_SpiClock_t;

typedef enum
{
  C_SpiLSB = 0,
  C_SpiMSB = 1
} C_SpiBitOrder_t;

typedef struct
{
  uint8_t devID;
  C_SpiMode_t mode;
  C_SpiClock_t prescaler;
  C_SpiBitOrder_t bit_order;
  uint8_t dataWidth;    // 4-16
} C_SpiConfig_t;

typedef enum
{
  C_Norm = 0,         // ��ͨģʽ��ʹ��mosi & miso
  C_OneWrite = 1,     // ����ģʽ��ʹ��mosi,ֻд
  C_OneWriteRead = 2, // ����ģʽ��ʹ��mosi,��д
} C_DIR_t;

class A_SPI
{
protected:
  uint8_t _devID;     // �豸ID����������spi�����Ϲ��صĲ�ͬ�豸������ʹ��cs pinid ��Ϊ�豸id
  uint8_t _busy;      // ��ǰ����״̬��1 ����æ��0 ����
  uint8_t _writeOnly; // spiͨѶģʽ���ã�ȡֵ�ο�C_DIR_t
public:
  virtual void begin(C_SpiConfig_t *cfg) = 0;
  virtual void config(C_SpiConfig_t *cfg) = 0;
  virtual uint8_t readConfig(void) = 0;

  virtual DATA_T transfer(DATA_T data) = 0;
  // ���ֽڶ�д
  virtual int8_t write(DATA_T data) = 0;
  virtual DATA_T read() = 0;
  // buf��д
  virtual int8_t write(uint8_t *data, uint16_t data_length) = 0;
  virtual int8_t read(uint8_t *recv_data, uint16_t data_length) = 0;

  //    virtual int8_t      dma_write(uint8_t data){ return 0;};
  //    virtual uint8_t     dma_read(){ return 0;};
  //    virtual uint16_t    dma_write_buf(uint8_t *data, uint16_t len){ return 0;};
  //    virtual uint16_t    dma_read_buf(uint8_t *recv_data, uint16_t len){ return 0;};
  //    virtual void        dma_wait(){};

  virtual int8_t take(C_SpiConfig_t *spi_config) = 0;
  virtual int8_t release(void) = 0;
};

/*
	ע�⣺1.�����SPI_CLOCK_DIV����delay_us��ʱ�������ơ����в�׼����Ӳ��SPI�����ܶ�
				2.speed����ֻ��ΪSPI_CLOCK_DIVx��������Ǵ�ֵ����ὫSPI_CLOCK_DIV��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(SPI_CONFIG_TYPE->prescaler);
				3.���ڵ���I2C�豸����ʹ��SPI_CLOCK_DIV256��
				4.�����ӿں�Ӳ��SPI��ȫһ�����Ի����滻��
*/
class E_SSPI : public A_SPI
{
public:
  E_SSPI(E_GPIO *sck, E_GPIO *miso, E_GPIO *mosi);               // ��׼spi
  E_SSPI(E_GPIO *sck, E_GPIO *mosi, C_DIR_t wo = C_OneWrite);    // ����spi��Ĭ��ֻд

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
  E_GPIO *_sck;
  E_GPIO *_miso;
  E_GPIO *_mosi;

  uint8_t _cpol, _cpha;

  uint8_t _bitOrder;  // �ֽ�˳��
  uint8_t spi_delay;  // spi clk ʱ��
  uint8_t _dataWidth; // ���ݿ��

  uint8_t busy;

  // void _format(DATA_T &data);
  DATA_T _msb(DATA_T data);
  DATA_T _lsb(DATA_T data);
};
#endif
