#include "keyboard.h"

#define KEY_STATE_INIT      0
#define KEY_STATE_WOBBLE    1
#define KEY_STATE_PRESS     2
#define KEY_STATE_LONG      3
#define KEY_STATE_CONTINUE  4
#define KEY_STATE_RELEASE   5

void keyBoard::loop()
{
    volatile static  uint8_t _machineState = KEY_STATE_INIT;
    static volatile uint32_t _lastTime;
    uint8_t   _keyNum   = pScan();    // _pScan返回的原始键值
    switch (_machineState)
    {
    case KEY_STATE_INIT: // 初始
        if (_keyNum != KEY_NULL)
        {
            _keybuf = _keyNum;
            _lastTime = gMs;
            _machineState = KEY_STATE_WOBBLE;
        }
        break;
    case KEY_STATE_WOBBLE: // 消抖
        if (_keyNum == _keybuf)
        {
#if (FILTER_COUNTS)
            if (gMs - _lastTime > FILTER_COUNTS)
            {
#endif
                _lastTime = gMs;
                _machineState = KEY_STATE_LONG;
#if (FILTER_COUNTS)
            }
#endif
        }
        else
        {
            _machineState = KEY_STATE_INIT;
        }
        break;
    case KEY_STATE_LONG: // 长按
        if (_keybuf == _keyNum)
        {
            if (gMs - _lastTime >= LONG_PRESS_COUNTS)
            {
                _lastTime = gMs;
                _keyValue = KEY_LONG | _keybuf;
                _machineState = KEY_STATE_CONTINUE;
            }
        }
        else
        {
            _machineState = KEY_STATE_RELEASE;					
        }
        break;
    case KEY_STATE_CONTINUE: // 连击
        if (_keybuf == _keyNum)
        {
            if (_keyValue & KEY_LONG)
            {
                if (gMs - _lastTime > LONG_PRESS_COUNTS>>1)
                {
                    _lastTime = gMs;
                   _keyValue |= KEY_CONTINUE;
                }
            }
            else
            {
                if (gMs - _lastTime > CONTINUE_COUNTS)
                {
                    _lastTime = gMs;
                    _keyValue = KEY_CONTINUE | _keybuf;
                }
            }
        }
        else
        {
            _machineState = KEY_STATE_INIT;
        }
        break;
    case KEY_STATE_RELEASE: // 释放
//			if (_keyValue >> 8 == KEY_NULL){
//        _keyValue = KEY_DOWN | _keybuf;
//			}
				_keyValue = ((~_keyValue) & KEY_DOWN) | _keybuf;
        _machineState = KEY_STATE_INIT;
        break;
    }
}

uint16_t keyBoard::getKeyVolue()
{	
	uint16_t re = KEY_NULL;
	if(_keyValue >> 8){
		re =	_keyValue;
		_keyValue = KEY_NULL;
	}
	return (re);
}
