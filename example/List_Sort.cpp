/*
	LinkedList Example
	Link: http://github.com/PaulMurrayCbr/LinkedList
	Forked from: http://github.com/ivanseidel/LinkedList

	Example Created by
		Paul Murray, github.com/PaulMurrayCbr
*/

#include "ebox.h"

char testString[] = "Lorem ipsum dolor sit amet, \
consectetur adipiscing elit, sed do eiusmod tempor \
incididunt ut labore et dolore magna aliqua. Ut enim \
ad minim veniam";

LinkedList<char *> list;

int compare(char *&a, char *&b) {
  return strcmp(a, b);
}

void setup() {
  eboxInit();
	uart1.begin(115200);
	uart1.printf("list sort test\r\n");

  uart1.printf("Beginning sketch in ");
  for (int i = 3; i > 0; i--) {
    uart1.write(i+0x30);
    uart1.write(' ');
    delayMs(500);
  }
	
  uart1.printf("0.Loading some strings into the linked list\r\n");

  char *p;
  int col = 0;
  for (p = strtok(testString, " ,."); p; p = strtok(NULL, " ,.")) {
    uart1.printf("%s ",p);
    col += strlen(p) + 1;
    if (col >= 80) {
      uart1.printf("\r\n");
      col = 0;
    }
    list.add(p);
  }


  if (col) {
    uart1.printf("\r\n");
    col = 0;
  }
  
//  Serial.println();
//  Serial.println("Sorting");
//  Serial.println();
	uart1.printf("Sorting\r\n");
  list.sort(compare);

//  Serial.println();
//  Serial.println("Result");
//  Serial.println();
	uart1.printf("Result\r\n");

  while (p = list.shift()) {
    uart1.printf(p);
    uart1.write(' ');
    col += strlen(p) + 1;
    if (col >= 80) {
      uart1.printf("\r\n");
      col = 0;
    }
  }
  if (col) {
    uart1.printf("\r\n");
    col = 0;
  }

  uart1.printf("\r\nDone!");
}



void test() {
  // put your main code here, to run repeatedly:

}
