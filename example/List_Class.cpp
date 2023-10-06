/*
	LinkedList Example
	Link: http://github.com/ivanseidel/LinkedList

	Example Created by
		Tom Stewart, github.com/tastewar

	Edited by:
		Ivan Seidel, github.com/ivanseidel
*/

#include "ebox.h"

// Let's define a new class
class Animal {
	public:
		char *name;
		bool isMammal;
};

char  catname[]="kitty";
char  dogname[]="doggie";
char  emuname[]="emu";

LinkedList<Animal*> myAnimalList = LinkedList<Animal*>();

void setup()
{

	eboxInit();
	uart1.begin(115200);
	uart1.printf("class list test\r\n");

	// Create a Cat
	Animal *cat = new Animal();
	cat->name = catname;
	cat->isMammal = true;

	// Create a dog
	Animal *dog = new Animal();
	dog->name = dogname;
	dog->isMammal = true;

	// Create a emu
	Animal *emu = new Animal();
	emu->name = emuname;
	emu->isMammal = false; // just an example; no offense to pig lovers

	// Add animals to list
	myAnimalList.add(cat);
	myAnimalList.add(emu);
	myAnimalList.add(dog);
}

void test() {

	uart1.printf("There are %d",myAnimalList.size());
	uart1.printf(" animals in the list. The mammals are: ");

	int current = 0;
	Animal *animal;
	for(int i = 0; i < myAnimalList.size(); i++){

		// Get animal from list
		animal = myAnimalList.get(i);

		// If its a mammal, then print it's name
		if(animal->isMammal){

			// Avoid printing spacer on the first element
			if(current++)
				uart1.printf(", ");

			// Print animal name
			uart1.printf(animal->name);
		}
	}
	uart1.printf(".\r\n");

	while (true); // nothing else to do, loop forever
}
