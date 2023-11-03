#pragma once

extern "C" {
#include <interface.h>
}

int getBitValue(uInt8 value, uInt8 bit_n);
void setBitValue(uInt8* variable, int n_bit, int new_value_bit);

// CylinderStart related functions
void moveCylinderStartFront();
void moveCylinderStartBack();
void stopCylinderStart();
int  getCylinderStartPos();
void gotoCylinderStart(int pos);

// Cylinder1 related functions
void moveCylinder1Front();
void moveCylinder1Back();
void stopCylinder1();
int  getCylinder1Pos();
void gotoCylinder1(int pos);

// Cylinder2 related functions
void moveCylinder2Front();
void moveCylinder2Back();
void stopCylinder2();
int  getCylinder2Pos();
void gotoCylinder2(int pos);

// Put here the other function headers!!!
void conveyorON();
void conveyorOFF();
int readSensor1();
int readSensor2();
void toggleLED();
int getLEDState();
int getCylinder1Sensor();
int getCylinder2Sensor();