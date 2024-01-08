#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "ebox_base.h"

/** 支持3种按键响应，单击，长按，连击
  * 需要自定义一个返回uint8_t类型的按键扫描函数，无按键时返回KEY_NUll,即0，所有非0的返回均视为有按键
  * 假设 key1的键值为 0x01,则单击为 KEY_DOWN|0x01(0x401),长按 KEY_LONG|0x01(0x201),连击KEY_CONTINUE|0x01(0x101)
  */

//软件滤波时间ms
#define CONTINUE_COUNTS 200 //连击间隔
#define FILTER_COUNTS   100   //默认不使用软件滤波
#define LONG_PRESS_COUNTS   2000 ////长按事件触发时间

// 空键值
#define KEY_NULL    		0x1E

//定义按键返回值状态(按下,长按,连发,释放)
#define KEY_DOWN        0x8400
#define KEY_LONG        0x8200
#define KEY_CONTINUE    0x8100


class keyBoard
{
	typedef uint8_t (*fptr)(void); //无参函数指针
public:
    fptr pScan;     // 指向扫面程序，读取硬件值
    void loop();    // 按键处理程序，滤波，单击，长按，连击
    uint16_t getKeyVolue(); // 返回处理好的键值
private:
		uint16_t  _keyValue; // 最终键值
};
#endif
