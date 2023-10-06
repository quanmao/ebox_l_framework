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

    // 取最大值，最小值
#define min3v(v1, v2, v3) ((v1) > (v2) ? ((v2) > (v3) ? (v3) : (v2)) : ((v1) > (v3) ? (v3) : (v2)))
#define max3v(v1, v2, v3) ((v1) < (v2) ? ((v2) < (v3) ? (v3) : (v2)) : ((v1) < (v3) ? (v3) : (v1)))
#define min2v(v1, v2) ((v1) < (v2) ? (v1) : (v2))
#define max2v(v1, v2) ((v1) > (v2) ? (v1) : (v2))
#define min(v1, v2) ((v1) < (v2) ? (v1) : (v2))
#define max(v1, v2) ((v1) > (v2) ? (v1) : (v2))
    //#define abs(x)              ((x)>0?(x):-(x))
#define absDiff(x,y) ((x>y) ? (x-y) : (y-x))		// 取两个数差的绝对值
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define roundl(x) ((x) >= 0 ? (long)((x)-0.5) : (long)((x) + 0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define LSB_FIRST 0
#define MSB_FIRST 1

#define LOW 0
#define HIGH 1

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define EULER 2.718281828459045235360287471352662

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

typedef unsigned int word;

#define bit(b) (1UL << (b))

#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitsGet(x, m, n) ((((uint32)x) << (31 - (n))) >> ((31 - (n)) + (m)))
#define bitShift(shift) (1UL << (shift))
#define bitMaskShift(mask, shift) ((mask) << (shift))
#define isPow2(v) ((v) && !((v) & ((v)-1)))
#define getArraySize(__ARR) (sizeof(__ARR) / sizeof(__ARR[0])) // 获取数组大小

    typedef bool boolean;
    typedef uint8_t byte;

typedef struct
{
//    uint32_t pll_vco;
    uint32_t core;
    uint32_t hclk;
    uint32_t pclk2;
    uint32_t pclk1;
}Clock_t;

typedef struct
{
//    CpuType type;
//    uint8_t pins;
    uint16_t flash_size;
    uint32_t ability; //cpu calculate plus per second;
//    CpuClock_t clock;
    uint32_t chip_id[3];
//    char company[2];
} CpuInfo_t;

// 利用联合体特性，可以快速的长uint16拆分成两个uint8_t型，反过来也可以合成
typedef union {
    uint8_t byte[2];
    uint16_t value;
} DataU16_t;

typedef union {
    uint8_t byte[4];
    uint32_t value;
} DataU32_t;

typedef union {
    uint8_t byte[2];
    int16_t value;
} Data16_t;

typedef union {
    uint8_t byte[4];
    int32_t value;
} Data32_t;

typedef union {
    uint8_t byte[4];
    float value;
} DataFloat_t;

typedef union {
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

#define ALIGN(size, align) ((align + size - 1) & (~(align - 1)))
#define ALIGN_4BYTE(size) ((4 + size - 1) & (~(4 - 1)))
#define ALIGN_8BYTE(size) ((8 + size - 1) & (~(8 - 1)))

#define GETREALFLOAT(a)	(a/100.0)
#define GETREALUINT(a)	(a/100)

#if __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
//限制某个数的下界
template <typename T>
void limitLow(T &num, T limL)
{
    if (num < limL)
    {
        num = limL;
    }
}

template <class T>
void swap(T *a, T *b)
{
    T temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

#endif
#endif
