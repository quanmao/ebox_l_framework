/**
  ******************************************************************************
  * @file    example.cpp
  * @author  cat_li
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "ebox.h"
#include "bsp_ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"I2C serach address example"
#define EXAMPLE_DATE	"2018-08-08"

E_GPIO PF6(PF6_ID);
E_GPIO PF7(PF7_ID);

//#define I2CS &PF6,&PF7
#define I2CS &PB6,&PB7

//E_I2C I2C(I22C);
E_SOFTI2C I2C(I2CS);
//E_UART UART(USART1, &PA9, &PA10);

API_I2C *i2c = (API_I2C *)&I2C;

void setup()
{
    eboxInit();
    UART.begin(115200);
		i2c->begin(API_I2C::K10);
	
			PF0.mode(OUTPUT_PP_PU);
		PF1.mode(OUTPUT_PP_PD);	
		PF0.set();
		PF1.reset();
		PA1.mode(OUTPUT_PP);
		PA1.reset();
		delayMs(500);
		PA1.set();
		
//		PA0.mode(OUTPUT_PP);
//		PA0.set();
}

void test(void)
{
    uint8_t add = 0;
    uint16_t i = 0;
		uint8_t r[3];
//while(1){	
//		i2c->write(0x46,0x01);
//		delayMs(10);
//		i2c->write(0x46,0x11);
//		delayMs(150);
//		i2c->read_buf(0x46,r,2);
//		UART.printf("I2C read %d,%d.......\r\n",r[0],r[1]);
//		delayMs(4000);
//}
    UART.printf("I2C address scanner, Scaning.......\r\n");
    while (1)
    {
				
        // 7位地址,0-127
        for (; add <= 255; add++)
        {
            UART.printf("test address is 0x%2x  ", add);
            if (i2c->check_busy(add) == 0)
            {
                UART.printf("success! \r\n");
                delayMs(6000);
                //break;
            }
            else
            {
                UART.printf("fail! \r\n");
            }
        }
        if (add > 127)
        {
            UART.printf("请检查接线,并确认设备ok! \r\n");
        }
        delayMs(6000);
        i2c->write(0x4e, 0x00);
        add = 0x0;
    }

}




