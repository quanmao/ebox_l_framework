// gpios ��������
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
 * @brief ��������ʾ�˲��в������IO
 * 1. E_GPIOS������port,����pin��ϲ���,��������Ч�ʵ�
 * 2. E_PORT ��ͬ��port����pin��ϲ�����Ч�ʸߣ�����Բ���E_GPIOS 
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
