#include "ebox.h"
#include "bsp_ebox.h"

E_UART uart(UART1);

void recive(){
	LED.toggle();
}

class timTest
{
public:
    void event()
    {
        LED.toggle();
    }
};
timTest tst;

void setup()
{
	eboxInit();
	uart.begin(115200);
	LED.mode(OUTPUT_PP);
//	LED1.mode(OUTPUT_PP);
//	LED1.set();
//	//uart.attach(recive,USE_TX_IRQ);
//	uart.attach(&tst,&timTest::event,C_UartRxIrq);
//	uart.interruptEnable(C_UartRxIrq);
}

uint8_t w[6] = {'1','2','3','4','5','6'};

void test(void)
{
	int16_t i;
	i = sizeof(w)/sizeof(uint8_t);
//	uart.write(w,6);
	while(1)
	{		
//			i = ebox_print::getString((char*)w,sizeof(w)/sizeof(w[0]),"%d \n\r",15);
//			uart.write(w,i);
			delayMs(1000);
			LED.toggle();
			uart.write('b');
		  uart.printf("test\r\n");
			delayMs(1000);
//			i = uart.read();
			uart.write(i);
			i = 0;
			delayMs(1000);

	}
}
