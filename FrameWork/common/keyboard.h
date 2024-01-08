#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "ebox_base.h"

/** ֧��3�ְ�����Ӧ������������������
  * ��Ҫ�Զ���һ������uint8_t���͵İ���ɨ�躯�����ް���ʱ����KEY_NUll,��0�����з�0�ķ��ؾ���Ϊ�а���
  * ���� key1�ļ�ֵΪ 0x01,�򵥻�Ϊ KEY_DOWN|0x01(0x401),���� KEY_LONG|0x01(0x201),����KEY_CONTINUE|0x01(0x101)
  */

//����˲�ʱ��ms
#define CONTINUE_COUNTS 200 //�������
#define FILTER_COUNTS   100   //Ĭ�ϲ�ʹ������˲�
#define LONG_PRESS_COUNTS   2000 ////�����¼�����ʱ��

// �ռ�ֵ
#define KEY_NULL    		0x1E

//���尴������ֵ״̬(����,����,����,�ͷ�)
#define KEY_DOWN        0x8400
#define KEY_LONG        0x8200
#define KEY_CONTINUE    0x8100


class keyBoard
{
	typedef uint8_t (*fptr)(void); //�޲κ���ָ��
public:
    fptr pScan;     // ָ��ɨ����򣬶�ȡӲ��ֵ
    void loop();    // ������������˲�������������������
    uint16_t getKeyVolue(); // ���ش���õļ�ֵ
private:
		uint16_t  _keyValue; // ���ռ�ֵ
};
#endif
