#pragma once

extern "C" {
#include <interface.h>
}

int getBitValue(uInt8 value, uInt8 bit_n);
void setBitValue(uInt8* variable, int n_bit, int new_value_bit);

// CylinderStart related functions
// 
// Moves the Start Cylinder to the FRONT position
void moveCylinderStartFront();
// Moves the Start Cylinder to the BACK position
void moveCylinderStartBack();
// Stops the Start Cylinder
void stopCylinderStart();
// Gets the Start Cylinder position
// Returns:	
// 0 - Back Position
// 1 - Front Position
// -1 - Between or Outside
int  getCylinderStartPos();
// Moves the Start Cylinder to a specified position
// pos - Can be: 
// 0 - BACK or 
// 1 - FRONT
void gotoCylinderStart(int pos);

// Cylinder1 related functions

// Moves the Cylinder 1 to the FRONT position
void moveCylinder1Front();
// Moves the Cylinder 1 to the BACK position
void moveCylinder1Back();
// Stops the Cylinder 1
void stopCylinder1();
// Gets the Cylinder 1 position
// Returns:	
// 0 - Back Position
// 1 - Front Position
// -1 - Between or Outside
int  getCylinder1Pos();
// Moves the Cylinder 1 to a specified position
// pos - Can be: 
// 0 - BACK or 
// 1 - FRONT
void gotoCylinder1(int pos);

// Cylinder2 related functions

// Moves the Cylinder 2 to the FRONT position
void moveCylinder2Front();
// Moves the Cylinder 2 to the BACK position
void moveCylinder2Back();
// Stops the Cylinder 2
void stopCylinder2();
// Gets the Cylinder 2 position
// Returns:	
// 0 - Back Position
// 1 - Front Position
// -1 - Between or Outside
int  getCylinder2Pos();
// Moves the Cylinder 2 to a specified position
// pos - Can be: 
// 0 - BACK or 
// 1 - FRONT
void gotoCylinder2(int pos);

// Put here the other function headers!!!
