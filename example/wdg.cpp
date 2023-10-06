/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

// STM32 RUN IN eBox

/*
一个独立看门狗的示例程序
*/
#include "ebox.h"
#include "bsp_ebox.h"
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME "independent wdg example"
#define EXAMPLE_DATE "2018-08-08"

E_IWWDG dog;
E_WWDG wdg;

void setup()
{
    eboxInit();
    UART.begin(115200);
    UART.printf("reset %d!!!\r\n",wdg.isWWDGRst());
    LED.mode(OUTPUT_PP_PU);
    //		LED.set();
    delayMs(1000);
    LED.reset();
//    dog.begin(26208);
}
// uint32_t current;
// void normal()
// {
//     uint8_t i = 8;
//     current = gMs;
//     UART.printf("喂狗演示,喂狗时间不超过26208ms\r\n");
//     while (i--)
//     {
//         dog.feed(); // 在常见的应用中喂狗应该在定时器中断中定时喂狗
//         UART.printf("feed %d running %.02fs interval %.02f s!\r\n", i++, (gMs / 1000.0), (gMs - current) / 1000.0);
//         current = eboxGetMs();
//         delayMs(26020);
//     }
// }

void  normal(){
    UART.printf("窗口看门狗测试\r\n");
    uint16_t min,max;
    wdg.getMaxMin(3,max,min);
    UART.printf("最大值 %d 最小值 %d\r\n",max,min);
    wdg.begin(5000,10000);
    uint8_t i = 8;
    while(i--){
        delayMs(4);
        wdg.feed();
        UART.printf("feed %d\r\n", i);
    }
}



void test(void)
{       
    normal();
    UART.printf("超时喂狗，系统将复位重启\r\n");
}
