/******************************************************************************
�� �� ���� stm32_define.h��
�� д �ˣ� cat_li
��дʱ�䣺 2017��3��21��
���ƽ̨�� STM32F0xx, ebox
�ӿ�˵���� stm32��ض���
�޸���־������
	NO.1-  ��ʼ�汾
	NO.2-  2017/5/16 typedef �����struct��������ͳһ�޸�Ϊt��׺,�����շ�������
				   typedef �����enum�������ͽ�β����t��׺,�����շ�������
******************************************************************************/

#ifndef __STM32_DEFINE_H_
#define __STM32_DEFINE_H_

#include "mcu_config.h"

#define M_IN			0x00
#define M_OUT			0x10
#define M_AF			0x20	// at32ϵ�� GPIO_MODE_MUX
#define M_ANALOG	0x30

#define	O_PP			0x00
#define O_OD			0x04	// at32ϵ�� OD Ϊ01

#define P_NO			0x00
#define P_UP			0x01
#define P_DOWN		0x02

#define GIO_A			0x00
#define GIO_B			0x10
#define GIO_C			0x20
#define GIO_F			0x50

typedef enum
{
    OUTPUT_PP			= M_OUT|O_PP,
    OUTPUT_PP_PU	= M_OUT|O_PP|P_UP,
    OUTPUT_PP_PD	= M_OUT|O_PP|P_DOWN,

    OUTPUT_OD			= M_OUT|O_OD,
    OUTPUT_OD_PU	= M_OUT|O_OD|P_UP,
    OUTPUT_OD_PD	= M_OUT|O_OD|P_DOWN,

    AIN						= M_ANALOG,

    INPUT					= M_IN,
    INPUT_PU			= M_IN|P_UP,
    INPUT_PD			= M_IN|P_DOWN,

    AF_OD					= M_AF|O_OD,
    AF_OD_PU			= M_AF|O_OD|P_UP,
    AF_OD_PD			= M_AF|O_OD|P_DOWN,

    AF_PP					= M_AF|O_PP,
    AF_PP_PU			= M_AF|O_PP|P_UP,
    AF_PP_PD			= M_AF|O_PP|P_DOWN,
} PinMode_t;

typedef enum
{
    PA0_ID = GIO_A,     PA1_ID,     PA2_ID,     PA3_ID,     PA4_ID,     PA5_ID,     PA6_ID,     PA7_ID,
    PA8_ID,         PA9_ID,     PA10_ID,    PA11_ID,    PA12_ID,    PA13_ID,    PA14_ID,    PA15_ID,

    PB0_ID = GIO_B,  PB1_ID,     PB2_ID,     PB3_ID,     PB4_ID,     PB5_ID,     PB6_ID,     PB7_ID,
    PB8_ID,         PB9_ID,     PB10_ID,    PB11_ID,    PB12_ID,    PB13_ID,    PB14_ID,    PB15_ID,

    PC0_ID = GIO_C,  PC1_ID,     PC2_ID,     PC3_ID,     PC4_ID,     PC5_ID,     PC6_ID,     PC7_ID,
    PC8_ID,         PC9_ID,     PC10_ID,    PC11_ID,    PC12_ID,    PC13_ID,    PC14_ID,    PC15_ID,

    PF0_ID = GIO_F,  PF1_ID,     PF2_ID,     PF3_ID,     PF4_ID,     PF5_ID,     PF6_ID,     PF7_ID,
    PF8_ID,         PF9_ID,     PF10_ID,    PF11_ID,    PF12_ID,    PF13_ID,    PF14_ID,    PF15_ID,

	P_NC = 0xff
} PinId_t; ///<���ŵ�����

// ����ʱ��Դ
typedef struct
{
    uint32_t dev;
    uint32_t rcc;
} DevToRcc_t;
// �����ж�Դ
typedef struct
{
    uint32_t dev;
    uint8_t irqn[2];
} DevToIRQn_t;
// ���� pin ��Ϣ
typedef struct
{
    PinId_t	_pinId;		//pin_id
    PinMode_t	_pinMode;	//pin ������ mode��outputtyper,updown
    uint8_t		_pinAf;		//af����
    uint32_t	_periph;	//��������ͨ����
}AF_FUN_S;

#if defined(AT32F421C8T7)
#include "at32f421c8t7_define.h"
#elif defined(STM32F030x8)
#include "stm32f030x8_define.h"
#elif defined(STM32F072xB)
  #include "stm32f072_define.h"
#else
#error "Please add and include the target STM32F0xx device define (in mcu_define.h file)"
#endif


/**
 *@brief    ����Pin_id��periph��ȡ����
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(PinId_t pin_id, const AF_FUN_S *emap, uint32_t periph)
{
    uint8_t i = 0;
    while (!((0xffffff00 & (emap + i)->_periph) == periph) || !((emap + i)->_pinId == pin_id))
    {
        if ((emap + i)->_pinId == 0xff)
        {
            return (uint8_t)0xff;
        }
        i++;
    }
    return i;
}

/**
 *@brief    ����Pin_id��ȡ��Ӧ��������
 *@param    pin_id��pin_id     *emap  �������б�
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(PinId_t pin_id, const AF_FUN_S *emap)
{
    uint8_t i = 0;
    while (!((emap + i)->_pinId  == pin_id ))
    {
        if ((emap + i)->_pinId == 0xff)
        {
            return (uint8_t)0xff;
        }
        i++;
    }
    return i;
}

#endif
