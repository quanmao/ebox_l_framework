/**
  ******************************************************************************
  * @file    ebox_uart.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/15
  * @brief   此文件问串口相关，提供串口初始化，write，read，
  *         1   支持发送完成，接收中断，默认开启接收中断
  *         2   支持串口1-5，默认开启串口1-2
  *         3   已知问题，读取未作判断，不会清空数据
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __UARTX_H
#define __UARTX_H

#include "FunctionPointer.h"
#include "ebox_config.h"

#include "ebox_print.h"

// 配置是是否使用RX,TX中断，1 使用，0 不使用
#if USE_UART_IRQ
#define USE_RX_IRQ 1
#define USE_TX_IRQ 0
#endif


enum IrqType
{
#if USE_RX_IRQ
    C_UartRxIrq = 0,
#endif
#if USE_TX_IRQ
    C_UartTxIrq,
#endif
    Irqs
};

typedef enum
{
    C_UartParityNone = 0x0000,
    C_UartParityEven = 0x0400,
    C_UartParityOdd = 0x0600
} Parity_t;

typedef enum
{
    C_UartStop1 = 0x0000,
    C_UartStop2 = 0x2000
} Stop_t;

class E_UART : public ebox_print
{
public:
    E_UART(USART_TypeDef *USARTx, E_GPIO *tx_pin, E_GPIO *rx_pin);
    //initial uart
    void begin(uint32_t baud_rate, Parity_t parity = C_UartParityNone, Stop_t stop_bit = C_UartStop1);

    virtual size_t write(uint8_t val);
    virtual size_t write(uint8_t *val, uint16_t len);

    uint8_t readInRx(void);
    uint8_t read(void);
    void setTimeout(uint16_t timeout){_tOut = timeout;};

#if (USE_RX_IRQ | USE_TX_IRQ)
    // 开启/关闭中断
    void interruptEnable(IrqType type);
    void interruptDisable(IrqType type);

    // 绑定中断回调函数
    void attach(void (*fptr)(void), IrqType type);
    // 绑定类成员函数作为回调函数，类名::函数名

    template <typename T>
    void attach(T *tptr, void (T::*mptr)(void), IrqType type)
    {
        if ((mptr != NULL) && (tptr != NULL))
        {
            _irq[type].attach(tptr, mptr);
        }
    }
#endif

private:
    USART_TypeDef *_USARTx;
    E_GPIO *_txPin;
    E_GPIO *_rxPin;

    uint16_t _tOut; // 超时

#if (USE_RX_IRQ | USE_TX_IRQ)
    FunctionPointer _irq[Irqs];
    static void _irqHandle(uint32_t id, IrqType irq_type);
#endif
};



#endif
