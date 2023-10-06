#include "ebox.h"
#include "bsp_ebox.h"

E_UART uart(UART1);


void setup()
{
	eboxInit();
	uart.begin(115200);

}

char buf[80]={1,2,3,4,5,6,7,8};

void test(void)
{
	while(1)
	{
#if USE_PRINTF
//	uart.printf("NO FORMAT\t DEC\t HEX\t OTC\t BIN\t \r\n");
//	for (int x = 0; x < 64; x++) {
//		uart.printf("%d \t\t %d \t %X\t %o\t  \t \t\r\n",x,x,x,x);
//		delayMs(200);
//	}		
	  int i =	ebox_sprintf(buf,"NO FORMAT\t DEC\t HEX\t OTC\t BIN\t \r\n");
		uart.write((uint8_t*)buf,i);
  	for (int x = 0; x < 64; x++) {
		i =	ebox_sprintf(buf,"%d \t\t %d \t %X\t %o\t  \t \t\r\n",x,x,x,x);
		uart.write((uint8_t*)buf,i);
		delayMs(200);
	}	

#else		
	uart.print("NO FORMAT");  // prints a label
  uart.print("\t");         // prints a tab

  uart.print("DEC");
  uart.print("\t");

  uart.print("HEX");
  uart.print("\t");

  uart.print("OCT");
  uart.print("\t");

  uart.print("BIN");
  uart.println();        // carriage return after the last label
		
	for (int x = 0; x < 64; x++) { // only part of the ASCII chart, change to suit
    // print it out in many formats:
    uart.print(x);       // print as an ASCII-encoded decimal - same as "DEC"
    uart.print("\t\t");  // prints two tabs to accomodate the label lenght

    uart.print(x, DEC);  // print as an ASCII-encoded decimal
    uart.print("\t");    // prints a tab

    uart.print(x, HEX);  // print as an ASCII-encoded hexadecimal
    uart.print("\t");    // prints a tab

    uart.print(x, OCT);  // print as an ASCII-encoded octal
    uart.print("\t");    // prints a tab

    uart.println(x, BIN);  // print as an ASCII-encoded binary

    // then adds the carriage return with "println"
    delayMs(2000);            // delay 200 milliseconds
  }
uart.println();
#endif
	}

}
