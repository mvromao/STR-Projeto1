#include "my_interaction_functions.h"
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
//
// Acho que não devia estar a fazer um while para verificar
// enquanto os cilindros não estão nas posições corretas no
// gotoCylinder####
// Outra ideia que tinha era fazer outra função para verificar
// se o cilindro estava na posição correta e aí para-lo, de
// maneira a não prender o programa no while 
// 

// Cylinder Start
void moveCylinderStartFront() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 0, 0); // set bit 0 to low level
    setBitValue(&p, 1, 1); // set bit 1 to high level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}
void moveCylinderStartBack() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 1, 0); // set bit 1 to low level
    setBitValue(&p, 0, 1); // set bit 0 to high level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}
void stopCylinderStart() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 0, 0); // set bit 0 to low level
    setBitValue(&p, 1, 0); // set bit 1 to low level
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
        while (getCylinderStartPos() != 1) {}
    }
    else {                        // pos = 0, back
        moveCylinderStartBack();
        while (getCylinderStartPos() != 0) {}
        //printf("test3");            //DEBUG
    }
    stopCylinderStart();
    return;
}

// Cylinder 1
void moveCylinder1Front() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 3, 0); // set bit 0 to low level
    setBitValue(&p, 4, 1); // set bit 1 to high level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}
void moveCylinder1Back() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 4, 0); // set bit 1 to low level
    setBitValue(&p, 3, 1); // set bit 0 to high level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}
void stopCylinder1() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 3, 0); // set bit 0 to low level
    setBitValue(&p, 4, 0); // set bit 1 to low level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
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
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 5, 0); // set bit 0 to low level
    setBitValue(&p, 6, 1); // set bit 1 to high level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}
void moveCylinder2Back() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 6, 0); // set bit 1 to low level
    setBitValue(&p, 5, 1); // set bit 0 to high level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}
void stopCylinder2() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 5, 0); // set bit 0 to low level
    setBitValue(&p, 6, 0); // set bit 1 to low level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
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
void conveyorON() {
    taskENTER_CRITICAL();
    uInt8 p2 = readDigitalU8(2);
    setBitValue(&p2, 2, 1); // set bit 1 to high level
    writeDigitalU8(2, p2); // update port 2
    taskEXIT_CRITICAL();
}
void conveyorOFF() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2);
    setBitValue(&p, 2, 0); // set bit 0 to high level
    writeDigitalU8(2, p); // update port 2
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