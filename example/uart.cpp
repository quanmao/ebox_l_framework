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
        //LED.toggle();
				uart1.write( uart1.readInRx());
    }
};
timTest tst;

void setup()
{
	eboxInit();
	uart.begin(115200);
	LED.mode(OUTPUT_PP);

	uart.attach(recive,C_UartRxIrq);
//	uart.attach(&tst,&timTest::event,C_UartRxIrq);
	uart.interruptEnable(C_UartRxIrq);
}

char w[6] = {'1','2','3','4','5','6'};

void test(void)
{
	int i;
	i = sizeof(w)/sizeof(uint8_t);
	uart.write(w,6);
	while(1)
	{		
//			LED.toggle();
//			uart.write('b');
//		  uart.printf("test\r\n");
//			delayMs(1000);
//			i = uart.read();
//			uart.write(i);
//			i = 0;
//			delayMs(1000);
	}
}
