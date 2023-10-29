#include "C:/str/my_interaction_functions/my_interaction_functions.h"
#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <interface.h>
#include <interrupts.h>
}

// #include <stdio.h>
int getBitValue(uInt8 value, uInt8 bit_n)
// given a byte value, returns the value of its bit n
{
	return(value & (1 << bit_n));
}

void setBitValue(uInt8* variable, int n_bit, int new_value_bit)
// given a byte value, set the n bit to value
{
	uInt8 mask_on = (uInt8)(1 << n_bit);
	uInt8 mask_off = ~mask_on;
	if (new_value_bit) *variable |= mask_on;
	else *variable &= mask_off;
}

// Cylinder Start
void moveCylinderStartFront() {
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 0, 0); // set bit 0 to low level
	setBitValue(&p, 1, 1); // set bit 1 to high level
	
	taskENTER_CRITICAL();
	writeDigitalU8(2, p); // update port 2
	taskEXIT_CRITICAL();
}
void moveCylinderStartBack() {
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 1, 0); // set bit 1 to low level
	setBitValue(&p, 0, 1); // set bit 0 to high level
	
	taskENTER_CRITICAL();
	writeDigitalU8(2, p); // update port 2
	taskEXIT_CRITICAL();
}
void stopCylinderStart() {
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 0, 0); // set bit 0 to low level
	setBitValue(&p, 1, 0); // set bit 1 to low level

	taskENTER_CRITICAL();
	writeDigitalU8(2, p); // update port 2
	taskEXIT_CRITICAL();
}
int getCylinderStartPos() {
	uInt8 p0 = readDigitalU8(0);
	if (getBitValue(p0, 6))
		return 0;
	if (getBitValue(p0, 5))
		return 1;
	return -1;
}
void gotoCylinderStart(int pos) { // Moves the cylinder into a position
	if (pos) {                    // pos = 1, front
		moveCylinderStartFront();
		printf("test3\n");            //DEBUG
		while (getCylinderStartPos() != 1) {}
	}
	else {                        // pos = 0, back
		printf("test4\n");            //DEBUG
		moveCylinderStartBack();
		printf("Cylinder Start started moving back!\n");            //DEBUG
		while (getCylinderStartPos() != 0) {}
	}
	printf("Cylinder Start Finished Moving\n");            //DEBUG
	stopCylinderStart();
	return;
}

// Cylinder 1
void moveCylinder1Front() {
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 3, 0); // set bit 0 to low level
	setBitValue(&p, 4, 1); // set bit 1 to high level
	writeDigitalU8(2, p); // update port 2
}
void moveCylinder1Back() {
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 4, 0); // set bit 1 to low level
	setBitValue(&p, 3, 1); // set bit 0 to high level
	writeDigitalU8(2, p); // update port 2
}
void stopCylinder1() {
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 3, 0); // set bit 0 to low level
	setBitValue(&p, 4, 0); // set bit 1 to low level
	writeDigitalU8(2, p); // update port 2
}
int getCylinder1Pos() {
	uInt8 p0 = readDigitalU8(0);
	if (!getBitValue(p0, 4))
		return 0;
	if (!getBitValue(p0, 3))
		return 1;
	return -1;
}
void gotoCylinder1(int pos) { // Moves the cylinder into a position
	if (pos) {                    // pos = 1, front
		moveCylinder1Front();
		while (getCylinder1Pos() != 1) {}
	}
	else {                        // pos = 0, back
		moveCylinder1Back();
		while (getCylinder1Pos() != 0) {}
	}
	stopCylinder1();
}

// Cylinder 2
void moveCylinder2Front() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 5, 0); // set bit 0 to low level
	setBitValue(&p, 6, 1); // set bit 1 to high level
	writeDigitalU8(2, p); // update port 2
}
void moveCylinder2Back() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 6, 0); // set bit 1 to low level
	setBitValue(&p, 5, 1); // set bit 0 to high level
	writeDigitalU8(2, p); // update port 2
}
void stopCylinder2() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 5, 0); // set bit 0 to low level
	setBitValue(&p, 6, 0); // set bit 1 to low level
	writeDigitalU8(2, p); // update port 2
}
int getCylinder2Pos() {
	uInt8 p0 = readDigitalU8(0);
	if (!getBitValue(p0, 2))
		return 0;
	if (!getBitValue(p0, 1))
		return 1;
	return -1;
}
void gotoCylinder2(int pos) { // Moves the cylinder into a position
	if (pos) {                    // pos = 1, front
		moveCylinder2Front();
		while (getCylinder2Pos() != 1) {}
	}
	else {                        // pos = 0, back
		moveCylinder2Back();
		while (getCylinder2Pos() != 0) {}
	}
	stopCylinder2();
	return;
}

// Conveyor
void toggleConveyor() {
	uInt8 p2 = readDigitalU8(2);
	if (getBitValue(p2, 2))
		setBitValue(&p2, 2, 0);
	else
		setBitValue(&p2, 2, 1);
	taskENTER_CRITICAL();
	writeDigitalU8(2, p2);
	taskEXIT_CRITICAL();
}
// Start Sensors
int readSensor1() {
	uInt8 p1 = readDigitalU8(1);
	if (getBitValue(p1, 6))
		return 1;
	else
		return 0;
}
int readSensor2() {
	uInt8 p1 = readDigitalU8(1);
	if (getBitValue(p1, 5))
		return 1;
	else
		return 0;
}

// LED
void toggleLED() {
	uInt8 p2 = readDigitalU8(2);
	if (getBitValue(p2, 7))
		setBitValue(&p2, 7, 0);
	else
		setBitValue(&p2, 7, 1);
	writeDigitalU8(2, p2);
}
int getLEDState() {
	uInt8 p2 = readDigitalU8(2);
	if (getBitValue(p2, 7))
		return 1;
	else
		return 0;
}

int getCylinder1Sensor() {
	uInt8 p = readDigitalU8(0);
	if (getBitValue(p, 0)){ 
		return 1;
	} else { 
		return 0; 
	}
}
int getCylinder2Sensor() {
	uInt8 p = readDigitalU8(1);
	if (getBitValue(p, 7)) {
		return 1;
	} else {
		return 0;
	}
}