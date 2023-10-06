#include "stdint.h"
#include "ebox_config.h"
#include "ebox_print.h"
#define buffer 64

uint16_t ebox_print::getString(char *buf,uint8_t size,const char *fmt, ...){

  uint16_t size1;
#if USE_PRINTF  
	va_list va_params;
    va_start(va_params, fmt);
		size1 = sizeof(buf);
    size1 = ebox_vsnprintf(buf, size, fmt, va_params);
    va_end(va_params);
#endif
    return size1;

}

void ebox_print::printf(const char *fmt,...){
#if USE_PRINTF
	char buf[buffer];
	uint16_t size1; 
		va_list va_params;
    va_start(va_params, fmt);
    size1 = ebox_vsnprintf(buf, buffer, fmt, va_params);
    va_end(va_params);
		write((uint8_t *)buf,size1);	
#endif
}

/**
 *@brief    定义一个ebox_printf的输出函数，其中的输出设备必须是可用的
 *@param    NONE
 *@retval   NONE
*/
size_t ebox_printf(const char *fmt, ...)
{

    int size1 = 0;
//#if USE_PRINTF
//    size_t size2 = 64;
//    char *p;

//    va_list va_params;
//    va_start(va_params, fmt);

//    do
//    {
//        p = (char *)ebox_malloc(size2);
//        if (p == NULL)
//            return 0;

//        size1 = ebox_vsnprintf(p, size2, fmt, va_params);

//        if (size1 == -1 || size1 >= size2)
//        {
//            size2 += 64;
//            size1 = -1;
//            ebox_free(p);
//        }
//    } while (size1 == -1);
//    va_end(va_params);
//    DBG_UART.write(p, size1);
//    DBG_UART.flush();
//    ebox_free(p);
//	char buf[buffer];
//		va_list va_params;
//    va_start(va_params, fmt);
//    size1 = ebox_vsnprintf(buf, buffer, fmt, va_params);
//    va_end(va_params);
//		DBG_UART.write((uint8_t *)buf,size1);	
//#endif
    return size1;
}
