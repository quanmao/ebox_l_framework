/*
	LinkedList Example
	Link: http://github.com/ivanseidel/LinkedList

	Example Created by
		Tom Stewart, github.com/tastewar

	Edited by:
		Ivan Seidel, github.com/ivanseidel
*/
#include "ebox.h"

LinkedList<int> myList = LinkedList<int>();

void setup()
{
	
	eboxInit();
	uart1.begin(115200);
	uart1.printf("SimpleInteger list test\r\n");

	// Add some stuff to the list
	int k = -240,
		l = 123,
		m = -2,
		n = 222;
	myList.add(n);
	myList.add(0);
	myList.add(l);
	myList.add(17);
	myList.add(k);
	myList.add(m);
}

void test() {

	int listSize = myList.size();

	uart1.printf("There are %d",listSize);
	uart1.printf(" integers in the list. The negative ones are: ");

	// Print Negative numbers
	for (int h = 0; h < listSize; h++) {

		// Get value from list
		int val = myList.get(h);

		// If the value is negative, print it
		if (val < 0) {
			uart1.printf(" %d",val);
			// Serial.print(" ");
			// Serial.print(val);
		}
	}

	while (true); // nothing else to do, loop forever
}


