/**
  ******************************************************************************
  * @file    ebox_uart.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/15
  * @brief   此文件问串口相关，提供串口初始化，write，read，支持中断。
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_ll_usart.h"
#include "ebox_base.h"
#include "ebox_gpio.h"
#include "ebox_nvic.h"
#include "ebox_uart.h"

enum Uart_It_Index
{
#if (USE_UART1 && defined USART1_BASE)
    uart1 = 0,
#endif
#if (USE_UART2 && defined USART2_BASE)
    uart2,
#endif
#if (USE_UART3 && defined USART3_BASE)
    uart3,
#endif
#if (USE_UART4 && defined USART4_BASE)
    uart4,
#endif
#if (USE_UART5 && defined USART5_BASE)
    uart5,
#endif
    uarts
};

#ifdef __cplusplus
extern "C"
{
#endif
#if (USE_RX_IRQ | USE_TX_IRQ)
    typedef void (*uartIrqHandler)(uint32_t id, IrqType type);
    static uint32_t irqIds[uarts];
    static uartIrqHandler pIrqHandler;
    void uartIrqHandleAdd(uint8_t index, uartIrqHandler handler, uint32_t id)
    {
        pIrqHandler = handler;
        irqIds[index] = id;
    }
#endif
#ifdef __cplusplus
}
#endif

/**
 *@brief    串口的构造函数
 *@param    USARTx:  USART1,2,3和UART4,5
 *          tx_pin:  外设所对应的tx引脚
 *          rx_pin:  外设所对应的rx引脚
 *@retval   None
*/
E_UART::E_UART(USART_TypeDef *USARTx, E_GPIO *tx_pin, E_GPIO *rx_pin)
{
    _USARTx = USARTx;
    _txPin = tx_pin;
    _rxPin = rx_pin;
    _tOut = 300;    // 默认超时时间300ms
}

/**
 *@name     void    begin(uint32_t baud_rate, uint8_t parity, uint8_t stop_bit);
 *@brief    串口初始化函数，并带有更多配置参数
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *          parity:     检验位；0：无校验位，1奇校验，2偶校验
 *          stop_bit:   停止位；1,2 两个可选参数
 *@retval   None
*/
void E_UART::begin(uint32_t baud_rate, Parity_t parity, Stop_t stop_bit)
{
    uint8_t index;
    eboxRccCmd(RccTableUSART, (uint32_t)_USARTx, ENABLE);
    // 初始化IO
    index = getIndex(_rxPin->id, UART_MAP);
    _rxPin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);
    index = getIndex(_txPin->id, UART_MAP);
    _txPin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);

    LL_USART_SetTransferDirection(_USARTx, LL_USART_DIRECTION_TX_RX);
    LL_USART_ConfigCharacter(_USARTx, LL_USART_DATAWIDTH_8B, parity, stop_bit);
    LL_USART_SetBaudRate(_USARTx, SystemCoreClock, LL_USART_OVERSAMPLING_16, baud_rate);
    //	禁止溢出检测
    LL_USART_DisableOverrunDetect(_USARTx);
    LL_USART_Enable(_USARTx);
    /* Polling USART initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(_USARTx))) || (!(LL_USART_IsActiveFlag_REACK(_USARTx))))
    {
    }
#if (USE_RX_IRQ | USE_TX_IRQ)
    eboxNvicIrqEnable(eboxDevToIrq(IrqTableUSART, (uint32_t)_USARTx, 0));

    switch ((uint32_t)_USARTx)
    {
#if (USE_UART1 && defined USART1_BASE)
    case (uint32_t)USART1_BASE:
        index = uart1;
        break;
#endif
#if (USE_UART2 && defined USART2_BASE)
    case (uint32_t)USART2_BASE:
        index = uart2;
        break;
#endif
#if (USE_UART3 && defined USART3_BASE)
    case (uint32_t)USART3_BASE:
        index = uart3;
        break;
#endif
#if (USE_UART4 && defined USART4_BASE)
    case (uint32_t)USART4_BASE:
        index = uart4;
        break;
#endif
#if (USE_UART5 && defined USART5_BASE)
    case (uint32_t)USART5_BASE:
        index = uart5;
        break;
#endif
		default:
			break;
    }
    uartIrqHandleAdd(index, E_UART::_irqHandle, (uint32_t)this);
#endif
}

/**
 *@brief    读取RDR中的数据并返回,不判断,不清除
 *@param    NONE
 *@retval   当前RDR数据
*/
uint8_t E_UART::readInRx()
{
    return (uint8_t)(READ_BIT(_USARTx->RDR, USART_RDR_RDR) & 0xFFU);
}

/**
 *@brief    读取一个字节，并清除当前数据
 *@param    NONE
 *@retval   -1  无数据，其他，当前数据
*/
uint8_t E_UART::read()
{
    uint8_t ret = 0;
    uint32_t endtime = ENDTIME(_tOut);
    while (!TIMEOUT(endtime, _tOut))
    {
        if (LL_USART_IsActiveFlag_RXNE(_USARTx))
        {
            ret = (uint8_t)(READ_BIT(_USARTx->RDR, USART_RDR_RDR) & 0xFFU);
            LL_USART_RequestRxDataFlush(_USARTx);
        }        
    }
    return ret;
}

/**
 *@brief    发送一个字节
 *@param    c：要发送的字符
 *@retval   1
*/
size_t E_UART::write(char val)
{
    while (READ_BIT(_USARTx->ISR, USART_ISR_TXE) != (USART_ISR_TXE))
    {
    };
    _USARTx->TDR = val;
    return 1;
}

size_t E_UART::write(char *val, size_t len)
{
    uint16_t i = 0;
    for (; i < len; i++)
    {
        while (READ_BIT(_USARTx->ISR, USART_ISR_TXE) != (USART_ISR_TXE))
        {
        }; // 等待发送寄存器为空
        _USARTx->TDR = *val++;
    }
    return i;
}

/**********************************中断相关操作************************************************/
#if (USE_RX_IRQ | USE_TX_IRQ)
// 开中断
void E_UART::interruptEnable(IrqType type)
{
#if (USE_RX_IRQ)
    (type == C_UartRxIrq) ? LL_USART_EnableIT_RXNE(_USARTx) : (LL_USART_EnableIT_TC(_USARTx));
#else
    (LL_USART_EnableIT_TC(_USARTx));
#endif
}
// 关中断
void E_UART::interruptDisable(IrqType type)
{
#if (USE_RX_IRQ)
    (type == C_UartRxIrq) ? LL_USART_DisableIT_RXNE(_USARTx) : (LL_USART_DisableIT_TC(_USARTx));
#else
    (LL_USART_DisableIT_TC(_USARTx));
#endif
}
// 绑定静态回调函数
void E_UART::attach(void (*fptr)(void), IrqType type)
{
    if (fptr)
    {
        _irq[type].attach(fptr);
    }
}

// 中断调用入口,通过该入口调用绑定的回调函数
void E_UART::_irqHandle(uint32_t id, IrqType irq_type)
{
    E_UART *handler = (E_UART *)id;
    handler->_irq[irq_type].call();
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if (USE_RX_IRQ | USE_TX_IRQ)

#if (USE_UART1 && defined USART1_BASE)
    void USART1_IRQHandler(void)
    {
#if USE_RX_IRQ
        if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
        {
            pIrqHandler(irqIds[uart1], C_UartRxIrq);
            LL_USART_RequestRxDataFlush(USART1);
        }
#endif
#if USE_TX_IRQ
        if (LL_USART_IsActiveFlag_TC(USART1) && LL_USART_IsEnabledIT_TC(USART1))
        {
            pIrqHandler(irqIds[uart1], C_UartTxIrq);
            LL_USART_ClearFlag_TC(USART1);
        }
#endif
    }
#endif

#if (USE_UART2 && defined USART2_BASE)
    void USART2_IRQHandler(void)
    {
#if USE_RX_IRQ
        if (LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2))
        {
            pIrqHandler(irqIds[uart2], C_UartRxIrq);
            LL_USART_RequestRxDataFlush(USART2);
        }
#endif
#if USE_TX_IRQ
        if (LL_USART_IsActiveFlag_TC(USART2) && LL_USART_IsEnabledIT_TC(USART2))
        {
            pIrqHandler(irqIds[uart2], C_UartTxIrq);
            LL_USART_ClearFlag_TC(USART2);
        }
#endif
    }
#endif

#if (USE_UART3 && defined USART3_BASE)
    void USART3_IRQHandler(void)
    {
#if USE_RX_IRQ
        if (LL_USART_IsActiveFlag_RXNE(USART3) && LL_USART_IsEnabledIT_RXNE(USART3))
        {
            pIrqHandler(irqIds[uart3], C_UartRxIrq);
            LL_USART_RequestRxDataFlush(USART3);
        }
#endif
#if USE_TX_IRQ
        if (LL_USART_IsActiveFlag_TC(USART3) && LL_USART_IsEnabledIT_TC(USART3))
        {
            pIrqHandler(irqIds[uart3], C_UartTxIrq);
            LL_USART_ClearFlag_TC(USART3);
        }
#endif
    }
#endif

#if (USE_UART4 && defined USART4_BASE)
    void USART4_IRQHandler(void)
    {
#if USE_RX_IRQ
        if (LL_USART_IsActiveFlag_RXNE(USART4) && LL_USART_IsEnabledIT_RXNE(USART4))
        {
            pIrqHandler(irqIds[uart4], C_UartRxIrq);
            LL_USART_RequestRxDataFlush(USART4);
        }
#endif
#if USE_TX_IRQ
        if (LL_USART_IsActiveFlag_TC(USART4) && LL_USART_IsEnabledIT_TC(USART4))
        {
            pIrqHandler(irqIds[uart4], C_UartTxIrq);
            LL_USART_ClearFlag_TC(USART4);
        }
#endif
    }
#endif

#if (USE_UART5 && defined USART5_BASE)
    void USART2_IRQHandler(void)
    {
#if USE_RX_IRQ
        if (LL_USART_IsActiveFlag_RXNE(USART5) && LL_USART_IsEnabledIT_RXNE(USART5))
        {
            pIrqHandler(irqIds[uart5], C_UartRxIrq);
            LL_USART_RequestRxDataFlush(USART5);
        }
#endif
#if USE_TX_IRQ
        if (LL_USART_IsActiveFlag_TC(USART5) && LL_USART_IsEnabledIT_TC(USART5))
        {
            pIrqHandler(irqIds[uart5], C_UartTxIrq);
            LL_USART_ClearFlag_TC(USART5);
        }
#endif
    }
#endif
#endif

#ifdef __cplusplus
}
#endif
