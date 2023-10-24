// labwork1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include <C:\Users\Renedito\Documents\str\my_interaction_functions\my_interaction_functions.h>

extern "C" {
	#include <FreeRTOS.h>
	#include <task.h>
	#include <timers.h>
	#include <semphr.h>
	#include <interface.h>	
	#include <interrupts.h>
}

xSemaphoreHandle xSemaphore;

#define mainREGION_1_SIZE   8201
#define mainREGION_2_SIZE   29905
#define mainREGION_3_SIZE   7607

// Initializations
void vAssertCalled(unsigned long ulLine, const char* const pcFileName);
static void initialiseHeap(void);
void inicializarPortos();
void myDaemonTaskStartupHook(void);

// Given Functions
void vAssertCalled(unsigned long ulLine, const char* const pcFileName)
{
	static BaseType_t xPrinted = pdFALSE;
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */
	/* Parameters are not used. */
	(void)ulLine;
	(void)pcFileName;
	printf("ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError());

	taskENTER_CRITICAL();
	{
		/* Cause debugger break point if being debugged. */
		__debugbreak();
		/* You can step out of this function to debug the assertion by using
		   the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		   value. */
		while (ulSetToNonZeroInDebuggerToContinue == 0)
		{
			__asm { NOP };
			__asm { NOP };
		}
	}
	taskEXIT_CRITICAL();
}
static void initialiseHeap(void)
{
	static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
	/* Just to prevent 'condition is always true' warnings in configASSERT(). */
	volatile uint32_t ulAdditionalOffset = 19;
	const HeapRegion_t xHeapRegions[] =
	{
		/* Start address with dummy offsetsSize */
		{ ucHeap + 1,mainREGION_1_SIZE },
		{ ucHeap + 15 + mainREGION_1_SIZE,mainREGION_2_SIZE },
		{ ucHeap + 19 + mainREGION_1_SIZE +
				mainREGION_2_SIZE,mainREGION_3_SIZE },
		{ NULL, 0 }
	};


	configASSERT((ulAdditionalOffset +
		mainREGION_1_SIZE +
		mainREGION_2_SIZE +
		mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);
	/* Prevent compiler warnings when configASSERT() is not defined. */
	(void)ulAdditionalOffset;
	vPortDefineHeapRegions(xHeapRegions);
}


// Tasks
TaskHandle_t taskCylinderStart;
TaskHandle_t taskCylinder1;
TaskHandle_t taskCylinder2;
TaskHandle_t taskCalibration;
TaskHandle_t taskMenu;
TaskHandle_t;
TaskHandle_t;
TaskHandle_t;
TaskHandle_t;

// Semaphores
xSemaphoreHandle xSemCylinderStart_start;
xSemaphoreHandle xSemCylinder1_start;
xSemaphoreHandle xSemCylinder2_start;
xSemaphoreHandle xSemCylinderStart_finished;
xSemaphoreHandle xSemCylinder1_finished;
xSemaphoreHandle xSemCylinder2_finished;
xSemaphoreHandle xSem_conveyor;

//Mailboxes
xQueueHandle mbx_CS;
xQueueHandle mbx_C1;
xQueueHandle mbx_C2;


// Cylinder Tasks
void vTaskCylinderStart(void* pvParameters) {
	while (TRUE) {
		xQueueSemaphoreTake(xSemCylinderStart_start, portMAX_DELAY);
		gotoCylinderStart(1);
		xSemaphoreGive(xSemCylinderStart_finished, portMAX_DELAY);
		gotoCylinderStart(0);
	}

}
void vTaskCylinder1(void* pvParameters) {
	while(TRUE) {
		xQueueSemaphoreTake(xSemCylinder1_start, portMAX_DELAY);
		gotoCylinder1(1);
		xSemaphoreGive(xSemCylinder1_finished, portMAX_DELAY);
		gotoCylinder1(0);
	}
}
void vTaskCylinder2(void* pvParameters) {
	xQueueSemaphoreTake(xSemCylinder2_start, portMAX_DELAY);
	gotoCylinder2(1);
	xSemaphoreGive(xSemCylinder2_finished, portMAX_DELAY);
	gotoCylinder2(0);
}
void vTaskMenu(void* pvParameters) {

}

void vTaskCalibration(void* pvParameters) {

}

void inicializarPortos() {
	// INPUT PORTS
	createDigitalInput(0);
	createDigitalInput(1);
	// OUTPUT PORTS
	createDigitalOutput(2);
}
void myDaemonTaskStartupHook(void) {
	inicializarPortos();

	xSemaphore = xSemaphoreCreateCounting(10, 0);

	xTaskCreate(vTaskCylinderStart, "vTaskCylinderStart", 100, NULL, 0, &taskCylinderStart);
	xTaskCreate(vTaskCylinder2, "vTaskCylinder2", 100, NULL, 0, &taskCylinder1);
	xTaskCreate(vTaskCylinder1, "vTaskCylinder1", 100, NULL, 0, &taskCylinder2);
	xTaskCreate(vTaskMenu, "vTaskMenu", 100, NULL, 0, &taskMenu);
	xTaskCreate(vTaskCalibration, "vTaskCalibration", 100, NULL, 0, &taskCalibration);
}

int main()
{
	inicializarPortos();
	initialiseHeap();
	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;
	vTaskStartScheduler();

	printf("\nCalibrate kit manually and press enter...");
	int tecla = 0;
	moveCylinderStartFront();
	while (tecla != 27) {
		tecla = _getch();
		printf("%c", tecla);
		if (tecla == 'q') {
			printf("Vai mover CylinderStart front\n");
			moveCylinderStartFront();
		}
		if (tecla == 'a') {
			printf("Vai mover CylinderStart back\n");
			moveCylinderStartBack();
		}	
		if (tecla == 'z') {
			printf("Parando CylinderStart\n");
			stopCylinderStart();
		}
		if (tecla == 'w') {
			printf("Vai mover Cylinder1 front");
		}
		if (tecla == 'x') {
			printf("Vai mover Cylinder2 back");
		}
			
		// to complete...
	}
	closeChannels();
		return 0;
}

void setBitValue(uInt8* variable, int n_bit, int new_value_bit)
// given a byte value, set the n bit to value
{
	uInt8 mask_on = (uInt8)(1 << n_bit);
	uInt8 mask_off = ~mask_on;
	if (new_value_bit) *variable |= mask_on;
	else *variable &= mask_off;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
