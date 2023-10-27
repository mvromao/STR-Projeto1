#include <stdio.h>
#include <conio.h>
#include <stdio.h>
extern "C" {
	// observe your project contents. We are mixing C files with cpp ones.
	// Therefore, inside cpp files, we need to tell which functions are written in C.
	// That is why we use extern "C" directive
#include <interface.h>
}
int main()
{
	printf("Welcome to the Compact Storage Application\n");
	printf("press key");
	createDigitalInput(0);
	createDigitalInput(1);
	createDigitalOutput(2);
	printf("\nPress a key to start");
	_getch();
	writeDigitalU8(2, 0x04);
	printf("\nPress a key to finish");
	_getch();
	closeChannels();
	return 0;
}