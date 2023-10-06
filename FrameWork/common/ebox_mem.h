#ifndef __EBOX_MEM_H
#define __EBOX_MEM_H

#include "ebox_type.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void    *eboxMalloc( size_t xWantedSize );  //此函数用于分配一块指定大小的内存空间。成功指向分配内存的指针，否则返回NULL。
void    eboxFree( void *ptr ); //此函数用于释放由malloc或calloc函数分配的内存。ptr是一个指向要释放的内存区域的指针。在释放内存后，ptr指向内存size为0。
void    *eboxRealloc(void *ptr, size_t size);//调整已分配内存块大小，size小于内存块，返回ptr,size大于内存块，返回新地址ptr,失败返回null;
size_t  eboxGetSizeofPtr(void *ptr);// get ptr mem size
size_t  eboxGetFree(void);// get free mem size

void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);

// 计算内存使用量
float   eboxMemUsage(void);
size_t   eboxMemUsed(void);
size_t  eboxGetHeapStartAddr(void);
size_t  eboxGetHeapEndAddr(void);
uint16_t eboxGetFreeBlock(void);    // 返回可用内存块数量，理想状态为1，越大说明碎片越多。

int eboxMemCmp(const void *cs, const void *ct, size_t count);// 比较两个空间中前count个字节内容大小，结果返回整数：0相等，cs>ct:大于0，cs<ct:小于0
void *eboxMemCpy(void *dst, const void *src, size_t count);// 从src中拷贝count个字节到dst中。

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
