#include "ebox.h"
#include "math.h"
#include "bsp_ebox.h"
/**
	*	1	此例程临时验证用
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"TEMP example"
#define EXAMPLE_DATE	"2018-08-08"



E_UART UART(UART1);
void setup()
{
    eboxInit();
    UART.begin(115200);
    UART.printf("123456");
}
E_GPIO PA15(PA15_ID);
//E_SSPI spi(SSPI);
E_SPI spi(HSPI);
uint16_t id;
void test(void)
{
		C_SpiConfig_t cfg;
	
		cfg.mode = C_SpiMode0;
		cfg.bit_order = C_SpiMSB;
		cfg.prescaler = C_SpiDIV2;
		cfg.devID = PA15.id;
	
		spi.begin(&cfg);
			
		PA15.mode(OUTPUT_PP);
		PA15.reset();
	
		spi.write(0x90);
		spi.write(0x00);
		spi.write(0x00);
		spi.write(0x00);

		id |= spi.read()<<8;
	  id |= spi.read();
		UART.printf("\r\n %x",id);
    while(1)
    {
        delayMs(10);
    }
}
