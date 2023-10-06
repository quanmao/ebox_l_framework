#ifndef __EBOX_MEM_H
#define __EBOX_MEM_H

#include "ebox_type.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void    *eboxMalloc( size_t xWantedSize );  //�˺������ڷ���һ��ָ����С���ڴ�ռ䡣�ɹ�ָ������ڴ��ָ�룬���򷵻�NULL��
void    eboxFree( void *ptr ); //�˺��������ͷ���malloc��calloc����������ڴ档ptr��һ��ָ��Ҫ�ͷŵ��ڴ������ָ�롣���ͷ��ڴ��ptrָ���ڴ�sizeΪ0��
void    *eboxRealloc(void *ptr, size_t size);//�����ѷ����ڴ���С��sizeС���ڴ�飬����ptr,size�����ڴ�飬�����µ�ַptr,ʧ�ܷ���null;
size_t  eboxGetSizeofPtr(void *ptr);// get ptr mem size
size_t  eboxGetFree(void);// get free mem size

void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);

// �����ڴ�ʹ����
float   eboxMemUsage(void);
size_t   eboxMemUsed(void);
size_t  eboxGetHeapStartAddr(void);
size_t  eboxGetHeapEndAddr(void);
uint16_t eboxGetFreeBlock(void);    // ���ؿ����ڴ������������״̬Ϊ1��Խ��˵����ƬԽ�ࡣ

int eboxMemCmp(const void *cs, const void *ct, size_t count);// �Ƚ������ռ���ǰcount���ֽ����ݴ�С���������������0��ȣ�cs>ct:����0��cs<ct:С��0
void *eboxMemCpy(void *dst, const void *src, size_t count);// ��src�п���count���ֽڵ�dst�С�

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
