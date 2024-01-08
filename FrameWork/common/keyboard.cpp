#include "keyboard.h"

#define KEY_STATE_INIT      0   // ��ʼ��
#define KEY_STATE_WOBBLE    1   // ����
#define KEY_STATE_PRESS     2   // ��������
#define KEY_STATE_LONG      3   // ����
#define KEY_STATE_CONTINUE  4   // ����   
#define KEY_STATE_RELEASE   5   // �����ͷ�

#define KEY_ISREAD 0x7FFF;

void keyBoard::loop()
{
    volatile static  uint8_t _machineState = KEY_STATE_INIT;
    volatile static  uint32_t _lastTime;
    volatile static  uint8_t _keybuf;
    uint8_t   _keyNum   = pScan();    // _pScan���ص�ԭʼ��ֵ
    switch (_machineState)
    {
    case KEY_STATE_INIT: // ��ʼ��
        if (_keyNum != KEY_NULL)
        {
            _keybuf = _keyNum;
            _lastTime = gMs;
            _machineState = KEY_STATE_WOBBLE;
        }
        break;
    case KEY_STATE_WOBBLE: // ����
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
    case KEY_STATE_LONG: // �����ж�
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
    case KEY_STATE_CONTINUE: // ����
        if (_keybuf == _keyNum)
        {
            if (_keyValue & KEY_LONG)
            {
                if (gMs - _lastTime > LONG_PRESS_COUNTS>>1) // ����LONG_PRESS_COUNTS/2����Ϊ����������������
                {
                    _lastTime = gMs;
                   _keyValue |= KEY_CONTINUE;
                }
            }
            else
            {
                if (gMs - _lastTime > CONTINUE_COUNTS)  // ÿ��continue_conts ms�󣬴���һ������
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
    case KEY_STATE_RELEASE: // �ͷŰ���ʱ���ɵ���������
        _keyValue = KEY_DOWN | _keybuf;
        _machineState = KEY_STATE_INIT;
        break;
    }
}

uint16_t keyBoard::getKeyVolue()
{	
	uint16_t re = KEY_NULL;
	if(_keyValue & 0x8000){
		_keyValue &= KEY_ISREAD;
		re =	_keyValue;
		//_keyValue = KEY_NULL;
	}
	return (re);
}
