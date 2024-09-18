#ifndef __EBOX_TYPE_H
#define __EBOX_TYPE_H

#include <stdlib.h>
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "stdbool.h"

#if __cplusplus
extern "C"
{
#endif

#define LSB_FIRST 0
#define MSB_FIRST 1


#define LOW 0
#define HIGH 1
//常用常数定义
#define PI 3.1415926535897932384626433832795//π，圆周率
#define HALF_PI 1.5707963267948966192313216916398// π/2, 半圆周律
#define TWO_PI 6.283185307179586476925286766559// 2倍圆周率
#define DEG_TO_RAD 0.017453292519943295769236907684886//角度转弧度
#define RAD_TO_DEG 57.295779513082320876798154814105//弧度转角度
#define EULER 2.718281828459045235360287471352662//欧拉常数

#define EOK 0
#define EWAIT 1
#define EBUSY 2
#define ETIMEOUT 3
#define EPARA 4
#define E2LONG 5
#define ENG 6
#define EOTHER 128

#define MAX_CHAR +127 // (1 Byte)
#define MIN_CHAR -128

#define MAX_SHORT +32767 // (2 Byte)
#define MIN_SHORT -32768

#define MAX_USHORT +65535 // (2 Byte)
#define MIN_USHORT -0

#define MAX_INT16 +32767 // (2 Byte)
#define MIN_INT16 -32768

#define MAX_UINT16 +65535 // (2 Byte)
#define MIN_UINT16 -0

#define MAX_INT +2147483647 // (4 Byte)
#define MIN_INT -2147483648

#define MAX_UINT +4294967295 // (4 Byte)
#define MIN_UINT -0

#define MAX_INT32 +2147483647 // (4 Byte)
#define MIN_INT32 -2147483648

#define MAX_UINT32 +4294967295 // (4 Byte)
#define MIN_UINT32 -0

#define MAX_LONG +2147483647 // (4 Byte)
#define MIN_LONG -2147483648

#define MAX_LONGLONG +9223372036854775807 // (8 Byte)
#define MIN_LONGLONG -9223372036854775808

#define MAX_ULONGLONG +1844674407370955161 // (8 Byte)
#define MIN_ULONGLONG -9223372036854775808

/**常用计算宏定义**/
// 取最大值，最小值
#define min3v(v1, v2, v3) ((v1) > (v2) ? ((v2) > (v3) ? (v3) : (v2)) : ((v1) > (v3) ? (v3) : (v2)))
#define max3v(v1, v2, v3) ((v1) < (v2) ? ((v2) < (v3) ? (v3) : (v2)) : ((v1) < (v3) ? (v3) : (v1)))
#define min2v(v1, v2) ((v1) < (v2) ? (v1) : (v2))
#define max2v(v1, v2) ((v1) > (v2) ? (v1) : (v2))
#define min(v1, v2) ((v1) < (v2) ? (v1) : (v2))
#define max(v1, v2) ((v1) > (v2) ? (v1) : (v2))
    //#define abs(x)              ((x)>0?(x):-(x))
#define absDiff(x,y) ((x>y) ? (x-y) : (y-x))		// 取两个数差的绝对值
//限制数的大小在特定范围内.amt是要限制的数，low是允许的最小值，high是允许的最大值。如果amt小于low，则返回low；如果amt大于high，则返回high；否则返回amt。
#define constrain(amt, low, high) ((amt) < (low)? (low) : ((amt) > (high)? (high) : (amt)))
#define round(x) ((x) >= 0? (long)((x) + 0.5) : (long)((x)-0.5))    // 4舍5入
//#define roundl(x) ((x) >= 0? (long)((x)-0.5) : (long)((x) + 0.5))   // 向下取整，即小数位全部舍掉
#define radians(deg) ((deg)*DEG_TO_RAD)// 角度到弧度
#define degrees(rad) ((rad)*RAD_TO_DEG)// 弧度到角度
#define sq(x) ((x) * (x))// 平方


#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)//用于从整数value中读取指定位的值。bit是要读取的位的索引（从0开始）。
#define bitSet(value, bit) ((value) |= (1UL << (bit)))//用于将指定位的值设置为1。bit是要设置的位的索引（从0开始）
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))//于将指定位的值设置为0。bit是要清除的位的索引（从0开始）。
#define bitWrite(value, bit, bitvalue) (bitvalue? bitSet(value, bit) : bitClear(value, bit))//用于设置指定位的值。bit是要设置的位的索引（从0开始），bitvalue是要设置的值。
#define bitsGet(x, m, n) ((((uint32)x) << (31 - (n))) >> ((31 - (n)) + (m)))// 用于从整数x中获取从第m位到第n位的值。m和n必须是0到31之间的整数。
#define bitShift(shift) (1UL << (shift))//用于计算一个位移量。shift是要位移的位数。
#define bitMaskShift(mask, shift) ((mask) << (shift))//用于计算一个位掩码，该位掩码从第shift位开始，并在其后mask个位上设置为1。
#define isPow2(v) ((v) &&!((v) & ((v)-1)))//用于检查一个整数是否为2的幂次方。如果v为非零且不能被其自身整除，则isPow2(v)为真。
#define getArraySize(__ARR) (sizeof(__ARR) / sizeof(__ARR[0])) // 获取数组大小

typedef bool boolean;
typedef uint8_t byte;
typedef unsigned int word;


typedef struct
{
    uint32_t core;// 内核时钟    
    uint32_t hclk;// HCLK时钟    
    uint32_t pclk2;// PCLK2时钟    
    uint32_t pclk1;// PCLK1时钟
} Clock_t;

typedef struct
{
    uint16_t flash_size;
    uint32_t ability; // cpu calculate plus per second;
                      //     CpuClock_t clock;
    uint32_t chip_id[3];
} CpuInfo_t;

// 利用联合体特性，可以快速的长uint16拆分成两个uint8_t型，反过来也可以合成
typedef union
{
    uint8_t byte[2];
    uint16_t value;
} DataU16_t;

typedef union
{
    uint8_t byte[4];
    uint32_t value;
} DataU32_t;

typedef union
{
    uint8_t byte[2];
    int16_t value;
} Data16_t;

typedef union
{
    uint8_t byte[4];
    int32_t value;
} Data32_t;

typedef union
{
    uint8_t byte[4];
    float value;
} DataFloat_t;

typedef union
{
    uint8_t byte[4];
    double value;
} DataDouble_t;

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t date;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t week;
} DateTime_t;
// 用于计算满足指定对齐要求的大小。size 要对齐的内存大小，align是所需对齐的大小，eg. ALIGN(4,8),返回大小为8的内存块，且确保在4字节对齐的地址上正确存储。
#define ALIGN(size, align) ((align + size - 1) & (~(align - 1)))
#define ALIGN_4BYTE(size) ((4 + size - 1) & (~(4 - 1)))
#define ALIGN_8BYTE(size) ((8 + size - 1) & (~(8 - 1)))

#define GETREALFLOAT(a) (a / 100.0) // 整数a转换成浮点数，2位小数点
//#define GETREALUINT(a) (a / 100)    // a转换成无符号整数。 

#if __cplusplus
} // extern "C"
#endif

/**以下代码只能在C++中使用**/
#if __cplusplus
//限制某个数的下界
template <typename T>
void limitLow(T &num, T limL)
{
    //如果num小于limL，则将num设置为limL
    if (num < limL)
    {
        num = limL;
    }
}

template <class T>
void swap(T *a, T *b)
{
    //交换两个数的值
    T temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

#endif
#endif
