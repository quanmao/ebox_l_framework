// gpios 测试用例
/**
 * @file gpios_.cpp
 * @author cat_li   
 * @brief ebox application example
 * @version 0.1
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_gpios.h"  

/**
 * @brief 此例程演示了并行操作多个IO
 * 1. E_GPIOS将任意port,任意pin组合操作,操作灵活，但效率低
 * 2. E_PORT 将同意port连续pin组合操作，效率高，灵活性不如E_GPIOS 
 */

// 从左到右对应bit0-bitx，不连续IO或非同一port推荐使用
E_GPIO *pins[] = {&PA0,&PA1,&PA2,&PB1};
E_GPIOS gpios(pins, sizeof(pins)/sizeof(E_GPIO *));

// 操做port连续IO推荐使用方式
E_PORT PortPin0(GPIOA_BASE,3,2);	// PA2,PA3,PA4
E_PORT PortPin1(GPIOA_BASE,3);		// PA0,PA1,PA2

API_GPIOS   * gpio_api;

uint8_t gpio_state = 0;
void operationPortPin0(){
    gpio_api = &PortPin0;
    gpio_api->mode(OUTPUT_PP);
    gpio_api->write(0x00); // PA2,PA3,PA4 = 0
    gpio_api->write(0xff); // PA2,PA3,PA4 = 1
    gpio_api->write(0x05); // PA2,PA4=1; PA3 = 0;
    gpio_state = gpio_api->read();
    uart1.print("Port test 0(PA2,PA3,PA4) = ");
    uart1.println(gpio_state,BIN);
    gpio_api->write(0x00); //

}

void operationPortPin1(){
    gpio_api = &PortPin1;  
    gpio_api->mode(OUTPUT_PP);
    gpio_api->write(0x00); // PA0,PA1,PA2 = 0
    gpio_api->write(0xff); // PA0,PA1,PA2 = 1
    gpio_api->write(0x02); // PA0,PA2=0; PA1 = 1;
    gpio_state = gpio_api->read();
    uart1.print("Port test 1(PA0,PA1,PA2) = ");
    uart1.println(gpio_state,BIN);
    gpio_api->write(0x00); //
}

void operationGpios(){
    gpio_api = &gpios;
    gpio_api->mode(OUTPUT_PP);
    gpio_api->write(0x00); // PA0,PA1,PA2,PB1 = 0
    gpio_api->write(0xff); // PA0,PA1,PA2,PB1 = 1
    gpio_api->write(0x09); // PA0,PB1=1; PA2,PA1 = 0;
    gpio_state = gpio_api->read();
    uart1.print("gpios test(PA0,PA1,PA2,PB1) = ");
    uart1.println(gpio_state,BIN);
    gpio_api->write(0x00); //
}

void setup(){
    eboxInit();
    uart1.begin(115200);
    uart1.println("gpios test");
}

void test(){
    while(1){
        operationGpios();
        operationPortPin0();
        operationPortPin1();
        delayMs(1000);
    }
}
