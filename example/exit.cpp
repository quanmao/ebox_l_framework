
#include "ebox.h"
#include "bsp_ebox.h"


void recive(){
	LED.toggle();
}

void setup()
{
	eboxInit();
	BTN.begin();
	LED.mode(OUTPUT_PP);
	LED.set();
	BTN.attach(recive,FALL);
	BTN.enable(FALL);
}

void test(void)
{

	while(1)
	{		

	}
}
