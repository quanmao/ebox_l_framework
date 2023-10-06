/**
  ******************************************************************************
  * @file    ebox_uart.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/15
  * @brief   ���ļ��ʴ�����أ��ṩ���ڳ�ʼ����write��read��֧���жϡ�
  *     2020/10/9   �޸��޸�ʼ�պ����ʴ����bug
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_usart.h"
#include "ebox_base.h"
#include "ebox_gpio.h"
#include "ebox_uart.h"
#include "ebox_nvic.h"


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
 *@brief    ���ڵĹ��캯��
 *@param    USARTx:  USART1,2,3��UART4,5
 *          tx_pin:  ��������Ӧ��tx����
 *          rx_pin:  ��������Ӧ��rx����
 *@retval   None
*/
E_UART::E_UART(USART_TypeDef *USARTx, E_GPIO *tx_pin, E_GPIO *rx_pin)
{
    _USARTx = USARTx;
    _txPin = tx_pin;
    _rxPin = rx_pin;
    _tOut = 300;    // Ĭ�ϳ�ʱʱ��300ms
}

/**
 *@name     void    begin(uint32_t baud_rate, uint8_t parity, uint8_t stop_bit);
 *@brief    ���ڳ�ʼ�������������и������ò���
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *          parity:     ����λ��0����У��λ��1��У�飬2żУ��
 *          stop_bit:   ֹͣλ��1,2 ������ѡ����
 *@retval   None
*/
void E_UART::begin(uint32_t baud_rate, Parity_t parity, Stop_t stop_bit)
{
    uint8_t index;
		Clock_t clock;
    eboxGetClockInfo(&clock);
	
    eboxRccCmd(RccTableUSART, (uint32_t)_USARTx, ENABLE);
    // ��ʼ��IO
    index = getIndex(_rxPin->id, UART_MAP);
    _rxPin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);
    index = getIndex(_txPin->id, UART_MAP);
    _txPin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);

    index = (_USARTx == USART1) ? 1 : 2;
    LL_USART_SetTransferDirection(_USARTx, LL_USART_DIRECTION_TX_RX);
    LL_USART_ConfigCharacter(_USARTx, LL_USART_DATAWIDTH_8B, parity, stop_bit);
    LL_USART_SetBaudRate(_USARTx, clock.core / index, baud_rate);

    LL_USART_Enable(_USARTx);

#if (USE_RX_IRQ | USE_TX_IRQ)
    index = eboxDevToIrq(IrqTableUSART, (uint32_t)_USARTx, 0);
    eboxNvicIrqSetPriority((IRQn_Type)index, 0);
    eboxNvicIrqEnable((IRQn_Type)index);

    switch ((uint32_t)_USARTx)
    {
#if (USE_UART1 && defined USART1_BASE)
    case (uint32_t)USART1:
        index = uart1;
        break;
#endif
#if (USE_UART2 && defined USART2_BASE)
    case (uint32_t)USART2:
        index = uart2;
        break;
#endif
#if (USE_UART3 && defined USART3_BASE)
    case (uint32_t)USART3:
        index = uart3;
        break;
#endif
#if (USE_UART4 && defined USART4_BASE)
    case (uint32_t)USART4:
        index = uart4;
        break;
#endif
#if (USE_UART5 && defined USART5_BASE)
    case (uint32_t)USART5:
        index = uart5;
        break;
#endif
    }
    uartIrqHandleAdd(index, E_UART::_irqHandle, (uint32_t)this);
#endif
}

/**
 *@brief    ��ȡRDR�е����ݲ�����,���ж�,�����
 *@param    NONE
 *@retval   ��ǰRDR����
*/
uint8_t E_UART::readInRx()
{
    return (uint8_t)(READ_BIT(_USARTx->DR, USART_DR_DR) & 0xFFU);
}

/**
 *@brief    ��ȡһ���ֽڣ��������ǰ����
 *@param    NONE
 *@retval   -1  �����ݣ���������ǰ����
*/
uint8_t E_UART::read()
{
    uint8_t ret = 0;
    uint32_t endtime = ENDTIME(_tOut);
    while (!TIMEOUT(endtime, _tOut))
    {
        if (LL_USART_IsActiveFlag_RXNE(_USARTx))
        {
		ret = (uint8_t)(READ_BIT(_USARTx->DR, USART_DR_DR));
             LL_USART_ClearFlag_RXNE(_USARTx);
        }        
    }
    return ret;
		
}

/**
 *@brief    ����һ���ֽ�
 *@param    c��Ҫ���͵��ַ�
 *@retval   1
*/
size_t E_UART::write(uint8_t val)
{
    while (READ_BIT(_USARTx->SR, USART_SR_TXE) != (USART_SR_TXE))
    {
    };
    _USARTx->DR = (uint8_t)val;
    return 1;
}

size_t E_UART::write(uint8_t *val, uint16_t len)
{
    uint16_t i = 0;
    for (; i < len; i++)
    {
        while (READ_BIT(_USARTx->SR, USART_SR_TXE) != (USART_SR_TXE))
        {
        }; // �ȴ����ͼĴ���Ϊ��
        _USARTx->DR = *val++;
    }
    return i;
}

/**********************************�ж���ز���************************************************/
#if (USE_RX_IRQ | USE_TX_IRQ)
// ���ж�
void E_UART::interruptEnable(IrqType type)
{
#if (USE_RX_IRQ)
    (type == C_UartRxIrq) ? LL_USART_EnableIT_RXNE(_USARTx) : (LL_USART_EnableIT_TC(_USARTx));
#else
    (LL_USART_EnableIT_TC(_USARTx));
#endif
}
// ���ж�
void E_UART::interruptDisable(IrqType type)
{
#if (USE_RX_IRQ)
    (type == C_UartRxIrq) ? LL_USART_DisableIT_RXNE(_USARTx) : (LL_USART_DisableIT_TC(_USARTx));
#else
    (LL_USART_DisableIT_TC(_USARTx));
#endif
}
// �󶨾�̬�ص�����
void E_UART::attach(void (*fptr)(void), IrqType type)
{
    if (fptr)
    {
        _irq[type].attach(fptr);
    }
}

// �жϵ������,ͨ������ڵ��ð󶨵Ļص�����
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
            LL_USART_ClearFlag_RXNE(USART1);
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
            LL_USART_ClearFlag_RXNE(USART2);
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
            LL_USART_ClearFlag_RXNE(USART3);
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
            LL_USART_ClearFlag_RXNE(USART4);
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
    void USART5_IRQHandler(void)
    {
#if USE_RX_IRQ
        if (LL_USART_IsActiveFlag_RXNE(USART5) && LL_USART_IsEnabledIT_RXNE(USART5))
        {
            pIrqHandler(irqIds[uart5], C_UartRxIrq);
            LL_USART_ClearFlag_RXNE(USART5);
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
