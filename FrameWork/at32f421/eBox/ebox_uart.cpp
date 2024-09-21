/**
 ******************************************************************************
 * @file    ebox_uart.cpp
 * @author  cat_li
 * @version V1.0
 * @date    2020/1/15
 * @brief   ���ļ��ʴ�����أ��ṩ���ڳ�ʼ����write��read��֧���жϡ�
 *      20240917-��ֲ��at32f421ϵ��
 ******************************************************************************
 * @attention
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
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
 *@brief    ���ڵĹ��캯��
 *@param    USARTx:  USART1,2,3��UART4,5
 *          tx_pin:  ��������Ӧ��tx����
 *          rx_pin:  ��������Ӧ��rx����
 *@retval   None
 */
E_UART::E_UART(usart_type *USARTx, E_GPIO *tx_pin, E_GPIO *rx_pin)
{
    _USARTx = USARTx;
    _txPin = tx_pin;
    _rxPin = rx_pin;
    _tOut = 300; // Ĭ�ϳ�ʱʱ��300ms
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
    eboxRccCmd(RccTableUSART, (uint32_t)_USARTx, ENABLE);
    // ��ʼ��IO
    index = getIndex(_rxPin->id, UART_MAP);
    _rxPin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);
    index = getIndex(_txPin->id, UART_MAP);
    _txPin->mode(UART_MAP[index]._pinMode, UART_MAP[index]._pinAf);

    usart_init(_USARTx, baud_rate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_parity_selection_config(_USARTx, (usart_parity_selection_type)parity);
    usart_transmitter_enable(_USARTx, TRUE);
		usart_receiver_enable(_USARTx,TRUE);
    usart_enable(_USARTx, TRUE);

#if (USE_RX_IRQ | USE_TX_IRQ)
    eboxNvicIrqEnable(eboxDevToIrq(IrqTableUSART, (uint32_t)_USARTx, 0));
//		eboxNvicIrqEnable(USART1_IRQn);
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
 *@brief    ��ȡRDR�е����ݲ�����,���ж�,�����
 *@param    NONE
 *@retval   ��ǰRDR����
 */
uint8_t E_UART::readInRx()
{
    return (uint8_t)(_USARTx->dt);
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
        if ((_USARTx->sts & USART_RDBF_FLAG) != 0)
        {
            ret = (uint8_t)(_USARTx->dt);
            //                   LL_USART_RequestRxDataFlush(_USARTx);
        }
    }
    return ret;
}

/**
 *@brief    ����һ���ֽ�
 *@param    c��Ҫ���͵��ַ�
 *@retval   1
 */
size_t E_UART::write(char val)
{
    while ((_USARTx->sts & USART_TDBE_FLAG) == 0)
    {
    }; // �ȴ����ͼĴ���Ϊ��
    _USARTx->dt = val;
    return 1;
}

size_t E_UART::write(char *val, size_t len)
{
    uint16_t i = 0;
    for (; i < len; i++)
    {
        while ((_USARTx->sts & USART_TDBE_FLAG) == 0)
        {
        }; // �ȴ����ͼĴ���Ϊ��
        _USARTx->dt = *val++;
    }
    return i;
}

/**********************************�ж���ز���************************************************/
#if (USE_RX_IRQ | USE_TX_IRQ)
// ���ж�
void E_UART::interruptEnable(IrqType type)
{
#if (USE_RX_IRQ)
		//usart_interrupt_enable(USART1, USART_RDBF_INT, TRUE);
    (type == C_UartRxIrq) ? (PERIPH_REG((uint32_t)_USARTx, USART_RDBF_INT) |= PERIPH_REG_BIT(USART_RDBF_INT)) : (PERIPH_REG((uint32_t)_USARTx, USART_TDBE_INT) |= PERIPH_REG_BIT(USART_TDBE_INT));
#else
    (PERIPH_REG((uint32_t)_USARTx, USART_TDBE_INT) |= PERIPH_REG_BIT(USART_TDBE_INT));
#endif
}
// ���ж�
void E_UART::interruptDisable(IrqType type)
{
#if (USE_RX_IRQ)
    (type == C_UartRxIrq) ? (PERIPH_REG((uint32_t)_USARTx, USART_RDBF_INT) &= ~PERIPH_REG_BIT(USART_RDBF_INT)) : (PERIPH_REG((uint32_t)_USARTx, USART_TDBE_INT) &= ~PERIPH_REG_BIT(USART_TDBE_INT));
#else
    (PERIPH_REG((uint32_t)_USARTx, USART_TDBE_INT) &= ~PERIPH_REG_BIT(USART_TDBE_INT));
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
        
        if( USART1->sts_bit.rdbf & USART1->ctrl1_bit.rdbfien )	// �����жϿ��ҽ��ջ������ǿ�
        {
            pIrqHandler(irqIds[uart1], C_UartRxIrq);
            USART1->sts = ~USART_RDBF_FLAG; // �����־λ,�����һֱ�����ж�.
        }
#endif
#if USE_TX_IRQ
        if (USART1->sts_bit1.tdc & USART1->ctrl1_bit.tdcien )   // ��������жϿ����ѷ������
        {
            pIrqHandler(irqIds[uart1], C_UartTxIrq);
            USART1->sts = ~USART_TDC_FLAG; // �����־λ,�����һֱ�����ж�.
        }
#endif
    }
#endif

#if (USE_UART2 && defined USART2_BASE)
    void USART2_IRQHandler(void)
    {
#if USE_RX_IRQ
        
        if( USART2->sts_bit.rdbf & USART2->ctrl1_bit.rdbfien )	// �����жϿ��ҽ��ջ������ǿ�
        {
            pIrqHandler(irqIds[uart2], C_UartRxIrq);
            USART2->sts = ~USART_RDBF_FLAG; // �����־λ,�����һֱ�����ж�.
        }
#endif
#if USE_TX_IRQ
        if (USART2->sts_bit1.tdc & USART2->ctrl1_bit.tdcien )   // ��������жϿ����ѷ������
        {
            pIrqHandler(irqIds[uart2], C_UartTxIrq);
            USART2->sts = ~USART_TDC_FLAG; // �����־λ,�����һֱ�����ж�.
        }
#endif
    }
#endif
#endif

#ifdef __cplusplus
}
#endif
