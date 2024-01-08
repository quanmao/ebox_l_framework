/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

// STM32 RUN IN eBox

/*
按键演示程序
*/
#include "ebox.h"
#include "bsp_ebox.h"
#include "keyboard.h"
#include "ebox_gpios.h"  

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME "keyboard example"
#define EXAMPLE_DATE "2024-01-08"

E_GPIO PB7(PB7_ID);
E_GPIO PB8(PB8_ID);
E_GPIO PB9(PB9_ID);
E_GPIO PB13(PB13_ID);
E_GPIO PB14(PB14_ID);

keyBoard key;
E_GPIO *pins[] = {&PB14,&PB13,&PB7,&PB8,&PB9};
E_GPIOS key5(pins,5);


uint8_t scan()

{
		uint8_t i = key5.read();
    return i;
}

void setup()
{
    eboxInit();
    UART.begin(115200);
    UART.printf("example:%s\r\n",EXAMPLE_NAME);
    UART.printf("Date:%s\r\n",__DATE__);
    key5.mode(OUTPUT_OD);
		key5.write(0xff);
    key.pScan = &scan;
    UART.printf("vol %d\r\n",scan());
    
}


void test(void)

{ 
	uint16_t rs = 0;
		while(1){
    key.loop();
		rs = key.getKeyVolue();
    if (rs != KEY_NULL)
    {
        UART.printf("key = %d\r\n",rs);
        delayMs(100);
    }
	}
}
