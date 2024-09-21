/**
  ******************************************************************************
  * @file    ebox_uart.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/15
  * @brief   ���ļ��ʴ�����أ��ṩ���ڳ�ʼ����write��read��
  *         1   ֧�ַ�����ɣ������жϣ�Ĭ�Ͽ��������ж�
  *         2   ֧�ִ���1-5��Ĭ�Ͽ�������1-2
  *         3   ��֪���⣬��ȡδ���жϣ������������
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __UARTX_H
#define __UARTX_H

#include "FunctionPointer.h"
#include "ebox_config.h"
#include "print.h"

// �������Ƿ�ʹ��RX,TX�жϣ�1 ʹ�ã�0 ��ʹ��
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
    C_UartParityNone = USART_PARITY_NONE,
    C_UartParityEven = USART_PARITY_EVEN,
    C_UartParityOdd = USART_PARITY_ODD
} Parity_t;

typedef enum
{
    C_UartStop1 = USART_STOP_1_BIT,
    C_UartStop2 = USART_STOP_2_BIT
} Stop_t;

class E_UART : public Print
{
public:
    E_UART(usart_type *USARTx, E_GPIO *tx_pin, E_GPIO *rx_pin);
    //initial uart
    void begin(uint32_t baud_rate, Parity_t parity = C_UartParityNone, Stop_t stop_bit = C_UartStop1);

    virtual size_t write(char val);
    //virtual size_t write(const char *val, size_t len);
		virtual size_t write(char *buffer, size_t size);
		//using Print::write;

    uint8_t readInRx(void);
    uint8_t read(void);
    void setTimeout(uint16_t timeout){_tOut = timeout;};

#if (USE_RX_IRQ | USE_TX_IRQ)
    // ����/�ر��ж�
    void interruptEnable(IrqType type);
    void interruptDisable(IrqType type);

    // ���жϻص�����
    void attach(void (*fptr)(void), IrqType type);
    // �����Ա������Ϊ�ص�����������::������

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
    usart_type *_USARTx;
    E_GPIO *_txPin;
    E_GPIO *_rxPin;

    uint16_t _tOut; // ��ʱ

#if (USE_RX_IRQ | USE_TX_IRQ)
    FunctionPointer _irq[Irqs];
    static void _irqHandle(uint32_t id, IrqType irq_type);
#endif
};
#endif


