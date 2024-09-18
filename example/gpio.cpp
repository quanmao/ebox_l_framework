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


/**
 * @brief 此例程演示了并行操作多个IO
 * 1. E_GPIOS将任意port,任意pin组合操作,操作灵活，但效率低
 * 2. E_PORT 将同意port连续pin组合操作，效率高，灵活性不如E_GPIOS 
 */


void setup(){
    eboxInit();
		PA5.mode(OUTPUT_PP);
}

void test(){
    while(1){
			PA5.toggle();
			delayMs(1000);
    }
}
