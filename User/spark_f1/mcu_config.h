#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H

#include "stdint.h"
#include "stm32f1xx.h"

#if defined(STM32F103xB)
	//用户配置区域
	//请查阅ebox_cpu_type.h寻找特定信号CPU的详细配置
	#define STM32_TYPE STM32F103C8
	#define STM32_PINS 48
	#define STM32_FLASH 128
	#define STM32_RAM1 20
	#define STM32_COMPANY "ST\0"
#elif defined(STM32F103xE)
	#define STM32_TYPE STM32F103ZE
	#define STM32_PINS 112
	#define STM32_FLASH 512
	#define STM32_RAM1 64
	#define STM32_COMPANY "ST\0"
#else
#error "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
#endif

//------------------抽象层宏定义------------------------
#define MCU_TYPE STM32_TYPE
#define MCU_PINS STM32_PINS
#define MCU_COMPANY STM32_COMPANY

//RAM 区域定义
#define MCU_SRAM1_SIZE STM32_RAM1 * 1024
#define MCU_SRAM1_BEGIN 0x20000000
#define MCU_SRAM1_END (MCU_SRAM1_BEGIN + MCU_SRAM1_SIZE)

#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define MCU_HEAP_BEGIN ((uint32_t)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section = "HEAP"
#else
extern int __bss_end;
#endif

#define MCU_HEAP_END MCU_SRAM1_END
#define MCU_HEAP_SIZE (MCU_HEAP_END - MCU_HEAP_BEGIN)

#define MCU_SRAM1_USED (MCU_HEAP_BEGIN - MCU_SRAM1_BEGIN)
#define MCU_SRAM1_REMAIND (MCU_SRAM1_END - MCU_HEAP_BEGIN)

//FLASH 区域定义
#define MCU_FLASH_SIZE STM32_FLASH * 1024

#ifdef __CC_ARM
extern int SHT$$INIT_ARRAY$$Limit;
#define MCU_FLASH_PRG_END ((uint32_t)&SHT$$INIT_ARRAY$$Limit)
#endif

#define MCU_FLASH_BEGIN 0x8000000
#define MCU_FLASH_END (0x8000000 + MCU_FLASH_SIZE)

#define MCU_FLASH_USED (MCU_FLASH_PRG_END - MCU_FLASH_BEGIN)
#define MCU_FLASH_REMAIND (MCU_FLASH_END - MCU_FLASH_PRG_END)

#endif
