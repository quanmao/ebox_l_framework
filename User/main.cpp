/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
//#include "bsp_ebox.h"

extern void setup();
extern void test();

int main(void)
{
    setup();
		test();
    while(1)
    {		
			
    }
}




