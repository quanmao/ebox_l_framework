/*
file   : *.cpp
author : shentq
version: V1.1
date   : 2016/03/26

Copyright 2016 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ebox_mem.h"
#include "wstring.h"
#include "bsp_ebox.h"
/**
 *	1	动态内存申请，释放例程
 */
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"动态内存申请，释放例程"
#define EXAMPLE_DATE	"2018-08-06"

// 内存申请，释放，比较，拷贝，以及内存占用情况等。
void mem_test()
{
    eboxMalloc(0);
    uart1.printf("总内存:%dByte\r\n",eboxGetFree());
    String str1;
    String str2;
    str1 = "123";
    str2 = "136";
    str1 += str2;
    uart1.printf("str1 = %s\r\n,str2 = %s", str1.c_str(),str2.c_str());

    char *ptr;

    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());
    ptr = (char *)eboxMalloc(128);
    uart1.printf("申请128字节addr = %p ,占用= %dByte,",ptr, eboxGetSizeofPtr(ptr));
    memcpy(ptr, str1.c_str(), 9);
    uart1.printf("内容：[%s]\r\n", ptr);
    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());
    ptr = (char *)eboxRealloc(ptr, 256);
    uart1.printf("修改为256字节addr = %p ,占用= %dByte,",ptr, eboxGetSizeofPtr(ptr));
    uart1.printf("内容：[%s]\r\n", ptr);
    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());
    eboxFree(ptr);
    uart1.printf("ptr释放\r\n");
    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());   
    uart1.printf("内存比较\r\n");
    uart1.printf("str1[%s]&str2[%s],cmp_num=%d,result = %d \r\n",str1.c_str(),str2.c_str(),3,eboxMemCmp(str1.c_str(),str2.c_str(),3));
    uart1.printf("str1[%s]&str2[%s],cmp_num=%d,result = %d \r\n",str1.c_str(),str2.c_str(),1,eboxMemCmp(str1.c_str(),str2.c_str(),1));
    uart1.printf("内存拷贝\r\n");
    ptr = (char *)eboxMalloc(50);
    eboxMemCpy(ptr, str1.c_str(), 9);
    uart1.printf("copy str1 to ptr 内容：[%s]\r\n", ptr);
    eboxMemCpy(ptr, str2.c_str(), 9);
    uart1.printf("copy str2 to ptr 内容：[%s]\r\n", ptr);
    eboxFree(ptr);
}

uint8_t *ptr;
uint8_t *ptx[100];
// 内存碎片测试
void memFragmentation()
{
    uart1.printf("\r\n");
    uart1.printf("----------------mem Fragmentation test--------------------\r\n");
    uart1.printf("mem :0x%x---0x%x(%dK%dbytes)\r\n", eboxGetHeapStartAddr(), eboxGetHeapEndAddr(), (eboxGetFree()) / 1024, (eboxGetFree()) % 1024);
    int i = 0;

    delayMs(100);
    uart1.printf("ptr ID:\t\tADDRE\t\t|free \r\n");
    for (i = 0; i < 100; i++)
    {
        ptx[i] = (uint8_t *)eboxMalloc(100);
        if (ptx[i] == NULL)
        {
            uart1.printf("game over\r\n");
            break;
        }
        uart1.printf("ptr %03d:\t\t@0X%X\t\t|%05d Byte\r\n", i, ptx[i], eboxGetFree());
    }
    uart1.printf("可用内存数%d \r\n", eboxGetFreeBlock());
    uart1.printf("\r\n===============================\r\n");
    uart1.printf("释放一半(只包含偶数)的指针后,");
    for (i = 0; i < 100; i += 2)
    {
        eboxFree(ptx[i]);
    }
    uart1.printf("可用内存数%d \r\n", eboxGetFreeBlock());

    uart1.printf("===============================\r\n");
    uart1.printf("释放另一半的指针后,");
    for (i = 1; i < 100; i += 2)
    {
        eboxFree(ptx[i]);
    }
    uart1.printf("可用内存数%d \r\n", eboxGetFreeBlock());
}

void setup()
{
    eboxInit();
    uart1.begin(115200);
}
int test(void)
{
    mem_test();
    memFragmentation();
    while(1)
    {
        delayMs(1000);
    }

}


