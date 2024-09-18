/**
  ******************************************************************************
  * @file    ebox_base.h
  * @author  cat_li
  * @version V1.0
  * @date    2020/1/8
  * @brief   ���ļ�Ϊebox�Ļ����ļ�����Ҫ�ṩʱ�ӳ�ʼ����systick����ʱ��mcu��Ϣ��ȡ��
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_BASE_H
#define __EBOX_BASE_H

//#include "stm32f0xx_ll_rcc.h"
//#include "stm32f0xx_ll_utils.h"
//#include "stm32f0xx_ll_cortex.h"
//#include "stm32f0xx_ll_system.h"
#include "mcu_config.h"
#include "mcu_define.h"
#include "ebox_type.h"

#define ENABLE TRUE


__STATIC_INLINE void nullFun(void) {}         // �պ����������κβ���������������ָ�븳ֵ����ֹ����ָ�� = nullʱ�����ã�����
typedef void (*fun_noPara_t)(void);          //�޲κ���ָ��
typedef void (*fun_onePara_t)(unsigned int); //����������ָ��
	
	

///////ȫ�ֱ���������///////////////////////////////////////////////
extern volatile uint32_t gMs; //�ṩһ��eboxGetMs()��Ч��ȫ�ֱ���������cpu���ÿ���

// ��ʼ��
extern void eboxInit(void); //eboxϵͳ��ʼ��
// MCU���
extern void eboxClockConfig(void);            //�ⲿʵ�ָú����������޸�ʱ��Դ�Լ�ʱ��Ƶ�ʡ�Ĭ�Ϲ�����48Mhz
extern void eboxGetClockInfo(Clock_t *clock); // ��ȡʱ����Ϣ
extern void eboxGetChipInfo(CpuInfo_t *cpu);  // ��ȡmcu��Ϣ,����flash size,chip_id
extern void eboxReset(void);
extern void eboxRccCmd(const DevToRcc_t *devtable, uint32_t dev, confirm_state state);
// �򿪹ر������ж�
#define eboxDisableAllIrq   __disable_irq   // �ر������ж�
#define eboxEnableAllIrq    __enable_irq    // ���������ж�
// ʱ����ؽӿ�
extern uint64_t eboxGetUs(void);  //��ȡSϵͳ��ǰ�����˶೤ʱ�䡣��λ��us
extern uint32_t eboxGetMs(void);  //��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��ms
extern void delayMs(uint32_t ms); //��ʱn���롣�������������ʹ�ã�������ֹ�����жϵ������
extern void delayUs(uint32_t us); //��ʱn΢�롣�������������ʹ�ã�������ֹ�����жϵ������
// systick�жϻص�����
extern void attachSystickCallBack(fun_noPara_t fun, uint16_t multiple = 1); // ���жϻص������������������� = multiple * 1ms; ���65535

#define ENDTIME(timeOut) (gMs + timeOut)
// ��ʱ,����1 ���򷵻�0   �������millis()��ȡmillis_seconds,���Է�ֹ��Ϊ�ر��жϵ��³���������ʱ������
#define TIMEOUT(endTime, delay) ((uint32_t)(endTime - eboxGetMs()) > delay)

/*----------------------------���²���Ϊ��Դ����----------------------------------*/

//// power manage
// typedef enum {
//  sleepOnExit,
//  sleepNow
// }sleep_t;

// typedef enum {
//  wfi,
//  wfe
// }entry_t;

// /* ���ر�CPUʱ�ӣ��������������У�����ͨ���κ��жϻ���,IO��������ǰ״̬ */
// extern void eboxSleep(sleep_t isSleepnow = sleepNow,entry_t entry = wfi);
// /* 1.8v������ʱ�ӹرգ�SRAM�ͼĴ������棬����ͨ���κ��ⲿ�ж�/�¼�����,
//    �ض�����(CEC,USART,I2C)�ж�(����Ϊ����ģʽ),IO��������ǰ״̬ */
// extern void eboxStop(sleep_t isSleepnow = sleepNow,entry_t entry = wfi); 
// /* ���ebox_sleep,ebox_stop����Ϊsleep_on_extiģʽ,���ж��е��øú��������˳�,������ѭ�� */
// __STATIC_INLINE void eboxExitSleepOnExti(){
//  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
//  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
// }

// /* wkup pin����,�ⲿ��λ,rtc alarm,�ڲ����Ź��ɻ���,Ĭ��PA0 */  
// extern void eboxStandby(uint16_t wakeUpPin = LL_PWR_WAKEUP_PIN1);
// /* �����ϴ��Ƿ�Ϊstandbyģʽ������ձ�־λ*/																			
// __STATIC_INLINE bool isWakeFromSB(){
// 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
// 		bool r = (READ_BIT(PWR->CSR, PWR_CSR_SBF) == (PWR_CSR_SBF));
// 		SET_BIT(PWR->CR, PWR_CR_CSBF);
//    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
// 		return r;
// }

// #define RST_LOWPOWER	0x80
// #define RST_WWDOG			0x40
// #define RST_IWDOG			0x20
// #define RST_SOFT			0x10
// #define RST_POWER			0x08
// #define RST_NRST			0x04
// #define RST_OBL				0x02

// // Ĭ�Ϸ���ֵΪ0x0c,��RST_POWER|RST_NRST
// __STATIC_INLINE uint8_t eboxResetType(){
// 		return (RCC->CSR)>>24;
// }

// // ���������ִ�к�λ�Ĵ�������ֵΪ0x1c
// __STATIC_INLINE void ebox_rst(){
// 		NVIC_SystemReset();
// }
#endif
