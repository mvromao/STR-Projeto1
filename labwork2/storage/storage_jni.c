#include <jni.h>
#include <storage.h>

/*
 * Class:     Storage
 * Method:    initializeHardwarePorts
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_initializeHardwarePorts
(JNIEnv* jniEnv, jclass jclass) {
	initializeHardwarePorts();
}

/*
 * Class:     Storage
 * Method:    moveXRight
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_moveXRight
(JNIEnv* jniEnv, jclass jclass) {
	moveXRight();
}
/*
 * Class:     Storage
 * Method:    moveXLeft
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_moveXLeft
(JNIEnv* jniEnv, jclass jclass) {
	moveXLeft();
}

/*
 * Class:     Storage
 * Method:    stopX
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_stopX
(JNIEnv* jniEnv, jclass jclass) {
	stopX();
}

/*
 * Class:     Storage
 * Method:    getXPos
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Storage_getXPos
(JNIEnv* jniEnv, jclass jclass) {
	getXPos();
}

/*
 * Class:     Storage
 * Method:    moveZUp
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_moveZUp
(JNIEnv* jniEnv, jclass jclass) {
	moveZUp();
}

/*
 * Class:     Storage
 * Method:    moveZDown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_moveZDown
(JNIEnv* jniEnv, jclass jclass) {
	moveZDown();
}

/*
 * Class:     Storage
 * Method:    stopZ
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_stopZ
(JNIEnv* jniEnv, jclass jclass) {
	stopZ();
}

/*
 * Class:     Storage
 * Method:    getZPos
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Storage_getZPos
(JNIEnv* jniEnv, jclass jclass) {
	getZPos();
}

/*
 * Class:     Storage
 * Method:    moveYInside
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_moveYInside
(JNIEnv* jniEnv, jclass jclass) {
	moveYInside();
}

/*
 * Class:     Storage
 * Method:    moveYOutside
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_moveYOutside
(JNIEnv* jniEnv, jclass jclass) {
	moveYOutside();
}

/*
 * Class:     Storage
 * Method:    stopY
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_stopY
(JNIEnv* jniEnv, jclass jclass) {
	stopY();
}

/*
 * Class:     Storage
 * Method:    getYPos
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Storage_getYPos
(JNIEnv* jniEnv, jclass jclass) {
	getYPos();
}

/*
 * Class:     Storage
 * Method:    getSwitch1
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Storage_getSwitch1
(JNIEnv* jniEnv, jclass jclass) {
	getSwitch1();
}

/*
 * Class:     Storage
 * Method:    getSwitch2
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Storage_getSwitch2
(JNIEnv* jniEnv, jclass jclass) {
	getSwitch2();
}

/*
 * Class:     Storage
 * Method:    getSwitch1_2
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Storage_getSwitch1_12
(JNIEnv* jniEnv, jclass jclass) {
	getSwitch1_2();
}

/*
 * Class:     Storage
 * Method:    ledOn
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_Storage_ledOn
(JNIEnv* jniEnv, jclass jclass, jint n) {
	ledOn(n);
}

/*
 * Class:     Storage
 * Method:    ledsOff
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Storage_ledsOff
(JNIEnv* jniEnv, jclass jclass) {
	ledsOff();
}
