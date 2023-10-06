/**
  ******************************************************************************
  * @file    ebox_interface_spi.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/202
  * @brief   spi外设接口
  *          2021/7/7  修改mode描述
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
  C_SpiModeLowRise0 = 0,    // CPOL0,CPHA1  低电平，上升沿
  C_SpiModeLowFall1 = 1,    // CPOL0,CPHA2  低电平，下降沿
  C_SpiModeHighFall2 = 2,   // CPOL1,CPHA1  高电平，下降沿
  C_SpiModeHighRise3 = 3    // CPOL1,CPHA2  高电平，上升沿
} C_SpiMode_t;
// define 方便不同的方式调用
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
  C_Norm = 0,         // 普通模式，使用mosi & miso
  C_OneWrite = 1,     // 单线模式，使用mosi,只写
  C_OneWriteRead = 2, // 单线模式，使用mosi,读写
} C_DIR_t;

class A_SPI
{
protected:
  uint8_t _devID;     // 设备ID，用来区分spi总线上挂载的不同设备，建议使用cs pinid 作为设备id
  uint8_t _busy;      // 当前总线状态，1 总线忙，0 空闲
  uint8_t _writeOnly; // spi通讯模式设置，取值参考C_DIR_t
public:
  virtual void begin(C_SpiConfig_t *cfg) = 0;
  virtual void config(C_SpiConfig_t *cfg) = 0;
  virtual uint8_t readConfig(void) = 0;

  virtual DATA_T transfer(DATA_T data) = 0;
  // 单字节读写
  virtual int8_t write(DATA_T data) = 0;
  virtual DATA_T read() = 0;
  // buf读写
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
	注意：1.该类的SPI_CLOCK_DIV是由delay_us延时函数控制。略有不准，比硬件SPI会慢很多
				2.speed设置只能为SPI_CLOCK_DIVx。如果不是此值，则会将SPI_CLOCK_DIV的值直接传递给delay_us.即delay_us(SPI_CONFIG_TYPE->prescaler);
				3.初期调试I2C设备建议使用SPI_CLOCK_DIV256。
				4.函数接口和硬件SPI完全一样可以互相替换。
*/
class E_SSPI : public A_SPI
{
public:
  E_SSPI(E_GPIO *sck, E_GPIO *miso, E_GPIO *mosi);               // 标准spi
  E_SSPI(E_GPIO *sck, E_GPIO *mosi, C_DIR_t wo = C_OneWrite);    // 单线spi，默认只写

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

  uint8_t _bitOrder;  // 字节顺序
  uint8_t spi_delay;  // spi clk 时序
  uint8_t _dataWidth; // 数据宽度

  uint8_t busy;

  // void _format(DATA_T &data);
  DATA_T _msb(DATA_T data);
  DATA_T _lsb(DATA_T data);
};
#endif
