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
 *	1	��̬�ڴ����룬�ͷ�����
 */
/* ���������������̷������� */
#define EXAMPLE_NAME	"��̬�ڴ����룬�ͷ�����"
#define EXAMPLE_DATE	"2018-08-06"

// �ڴ����룬�ͷţ��Ƚϣ��������Լ��ڴ�ռ������ȡ�
void mem_test()
{
    eboxMalloc(0);
    uart1.printf("���ڴ�:%dByte\r\n",eboxGetFree());
    String str1;
    String str2;
    str1 = "123";
    str2 = "136";
    str1 += str2;
    uart1.printf("str1 = %s\r\n,str2 = %s", str1.c_str(),str2.c_str());

    char *ptr;

    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());
    ptr = (char *)eboxMalloc(128);
    uart1.printf("����128�ֽ�addr = %p ,ռ��= %dByte,",ptr, eboxGetSizeofPtr(ptr));
    memcpy(ptr, str1.c_str(), 9);
    uart1.printf("���ݣ�[%s]\r\n", ptr);
    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());
    ptr = (char *)eboxRealloc(ptr, 256);
    uart1.printf("�޸�Ϊ256�ֽ�addr = %p ,ռ��= %dByte,",ptr, eboxGetSizeofPtr(ptr));
    uart1.printf("���ݣ�[%s]\r\n", ptr);
    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());
    eboxFree(ptr);
    uart1.printf("ptr�ͷ�\r\n");
    uart1.printf("free = %dByte used =%dByte, %.2f%% \r\n", eboxGetFree(),eboxMemUsed(),eboxMemUsage());   
    uart1.printf("�ڴ�Ƚ�\r\n");
    uart1.printf("str1[%s]&str2[%s],cmp_num=%d,result = %d \r\n",str1.c_str(),str2.c_str(),3,eboxMemCmp(str1.c_str(),str2.c_str(),3));
    uart1.printf("str1[%s]&str2[%s],cmp_num=%d,result = %d \r\n",str1.c_str(),str2.c_str(),1,eboxMemCmp(str1.c_str(),str2.c_str(),1));
    uart1.printf("�ڴ濽��\r\n");
    ptr = (char *)eboxMalloc(50);
    eboxMemCpy(ptr, str1.c_str(), 9);
    uart1.printf("copy str1 to ptr ���ݣ�[%s]\r\n", ptr);
    eboxMemCpy(ptr, str2.c_str(), 9);
    uart1.printf("copy str2 to ptr ���ݣ�[%s]\r\n", ptr);
    eboxFree(ptr);
}

uint8_t *ptr;
uint8_t *ptx[100];
// �ڴ���Ƭ����
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
    uart1.printf("�����ڴ���%d \r\n", eboxGetFreeBlock());
    uart1.printf("\r\n===============================\r\n");
    uart1.printf("�ͷ�һ��(ֻ����ż��)��ָ���,");
    for (i = 0; i < 100; i += 2)
    {
        eboxFree(ptx[i]);
    }
    uart1.printf("�����ڴ���%d \r\n", eboxGetFreeBlock());

    uart1.printf("===============================\r\n");
    uart1.printf("�ͷ���һ���ָ���,");
    for (i = 1; i < 100; i += 2)
    {
        eboxFree(ptx[i]);
    }
    uart1.printf("�����ڴ���%d \r\n", eboxGetFreeBlock());
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


