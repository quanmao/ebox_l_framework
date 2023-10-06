#ifndef __EBOX_PRINTF_H__
#define __EBOX_PRINTF_H__

#include <stdio.h>

class ebox_print 
{
private:
    /* data */
public:
    ebox_print (){};
		void printf(const char *fmt, ...);
    static uint16_t getString(char *buf,uint8_t size, const char *fmt, ...);
		virtual  size_t write(uint8_t *val,uint16_t len)=0;
};

#endif
