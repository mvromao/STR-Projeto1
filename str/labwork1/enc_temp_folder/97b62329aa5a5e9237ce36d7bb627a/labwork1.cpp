#include <iostream>
#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include <C:/str/my_interaction_functions/my_interaction_functions.h>
extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <interface.h>	
#include <interrupts.h>
}

#define mainREGION_1_SIZE   8201
#define mainREGION_2_SIZE   29905
#define mainREGION_3_SIZE   7607

TaskHandle_t taskMenu;
TaskHandle_t taskCalibration;
TaskHandle_t taskCylinderStart;
TaskHandle_t taskCylinder1;
TaskHandle_t taskCylinder2;
TaskHandle_t taskConveyor;
TaskHandle_t taskBrickManager;
TaskHandle_t taskDock1;
TaskHandle_t taskDock2;
TaskHandle_t taskDock3;
TaskHandle_t taskEnterBrick;
TaskHandle_t taskCheckBrick;
TaskHandle_t taskP2;

xSemaphoreHandle xSemCalibrationStarted;
xSemaphoreHandle xSemCalibrationFinished;
xSemaphoreHandle xSemCylinderStartStart;
xSemaphoreHandle xSemCylinder1Start;
xSemaphoreHandle xSemCylinder2Start;
xSemaphoreHandle xSemCylinderStartFinished;
xSemaphoreHandle xSemCylinder1Finished;
xSemaphoreHandle xSemCylinder2Finished;
xSemaphoreHandle xSemCheckBrickStart;
xSemaphoreHandle xSemBrickManagerStart;
xSemaphoreHandle xSemBrickManagerFinished;
xSemaphoreHandle xSemDock1SensorFinished;
xSemaphoreHandle xSemDock2SensorFinished;
xSemaphoreHandle xSemDock3SensorFinished;
xSemaphoreHandle xSemConveyor;
xSemaphoreHandle xSemBrickEntry;



xQueueHandle mbx_P2;
xQueueHandle mbx_checkedBricks;

xQueueHandle mbx_CS;
xQueueHandle mbx_C1;
xQueueHandle mbx_C2;

xQueueHandle mbx_D1;
xQueueHandle mbx_D2;
xSemaphoreHandle xSemD3;

int Front = 1;
int Back = 0;
int blocksRemaining = 0;

// Tasks

void vTaskP2(void* pvParameters) {
	char cmd;
	while (TRUE) {
		xQueueReceive(mbx_P2, &cmd, portMAX_DELAY);
		printf("Received %c\n", cmd);
		switch (cmd) {
		case 0:
			printf("Case 0!\n");
			xQueueSend(mbx_CS, &Back, portMAX_DELAY);
			break;
		case 1:
			printf("Case 1!\n");
			xQueueSend(mbx_CS, &Front, portMAX_DELAY);
			break;
		case 2:
			xSemaphoreGive(xSemConveyor);
			break;
		case 3:
			xQueueSend(mbx_C1, &Back, portMAX_DELAY);
			break;
		case 4:
			xQueueSend(mbx_C1, &Front, portMAX_DELAY);
			break;
		case 5:
			xQueueSend(mbx_C2, &Back, portMAX_DELAY);
			break;
		case 6:
			xQueueSend(mbx_C2, &Front, portMAX_DELAY);
			break;
		case 7:
			// LED
			break;
		}
	}
}

void vTaskCylinderStart(void* pvParameters) {
	int pos = 0;
	while (TRUE) {
		xQueueReceive(mbx_CS, &pos, portMAX_DELAY);
		printf("Order to go to %d received!\n", pos);

		gotoCylinderStart(pos);
		printf("Cylinder stopped moving!\n");
		if (pos == 1) {
			printf("Cylinder Start going to the front!\n");
			xSemaphoreGive(xSemCylinderStartFinished);
		}
		if (pos == 0) {
			printf("Cylinder Start going back!\n");
			xSemaphoreGive(xSemCylinderStartStart);
		}
	}
}
void vTaskCylinder1(void* pvParameters) {
	int pos;
	while (TRUE) {
		xQueueReceive(mbx_C1, &pos, portMAX_DELAY);
		gotoCylinder1(pos);
		if (pos == 1) {
			xSemaphoreGive(xSemCylinder1Finished);
		}
		if (pos == 0) {
			xSemaphoreGive(xSemCylinder1Start);
		}
	}
}
void vTaskCylinder2(void* pvParameters) {
	int pos;
	while (TRUE) {
		xQueueReceive(mbx_C2, &pos, portMAX_DELAY);
		gotoCylinder2(pos);
		if (pos == 1) {
			xSemaphoreGive(xSemCylinder2Finished);
		}
		if (pos == 0) {
			xSemaphoreGive(xSemCylinder2Start);
		}
	}
}
void vTaskConveyor(void* pvParameters) {
	while (TRUE) {
		xQueueSemaphoreTake(xSemConveyor, portMAX_DELAY);
		printf("Conveyor toggled!\n");
		toggleConveyor();
	}
}

void vTaskDock1(void* pvParameters) {
	int brickType;
	while (TRUE) {
		xQueueReceive(mbx_D1, &brickType, portMAX_DELAY);
		while (getCylinder1Sensor() != 1) {}
		if (brickType == 1) {
			printf("Brick of type %d received!", brickType);
			xSemaphoreGive(xSemConveyor);
			printf("Convenyor off!\n");

			xQueueSend(mbx_C1, &Front, portMAX_DELAY);
			xQueueSend(mbx_C1, &Back, portMAX_DELAY);

			xQueueSemaphoreTake(xSemCylinder1Start, portMAX_DELAY);
			blocksRemaining--;
			if (blocksRemaining > 0)
				xSemaphoreGive(xSemConveyor);
			printf("Convenyor on!\n");
		}
		xSemaphoreGive(xSemDock1SensorFinished);
	}
}
void vTaskDock2(void* pvParameters) {
	int brickType;
	while (TRUE) {
		xQueueReceive(mbx_D2, &brickType, portMAX_DELAY);
		printf("Entered dock2 task!\n");
		while (getCylinder1Sensor() != 1) {}
		xSemaphoreGive(xSemDock1SensorFinished);
		while (getCylinder2Sensor() != 1) {}
		if (brickType == 2) {
			xSemaphoreGive(xSemConveyor);

			xQueueSend(mbx_C2, &Front, portMAX_DELAY);
			xQueueSend(mbx_C2, &Back, portMAX_DELAY);

			xQueueSemaphoreTake(xSemCylinder2Start, portMAX_DELAY);
			blocksRemaining--;
			printf("Blocks Remaining: %d", blocksRemaining);
			if (blocksRemaining > 0)
				xSemaphoreGive(xSemConveyor);

		}
		else {
			printf("Sending to dock3\n");
			xSemaphoreGive(xSemD3);
		}
	}
}
void vTaskDock3(void* pvParameters) {
	while (TRUE) {
		xQueueSemaphoreTake(xSemD3, portMAX_DELAY);
		printf("Brick of type 3 detected\n");
		vTaskDelay(3000);
		// ativar LED

		blocksRemaining--;
		printf("Blocks Remaining: %d", blocksRemaining);
		if (blocksRemaining == 0)
			xSemaphoreGive(xSemConveyor);
	}
	xSemaphoreGive(xSemDock1SensorFinished);
}

void vTaskCheckBrick(void* pvParameters) {

	while (TRUE) {
		int packageType = 1, sensor1 = 0, sensor2 = 0;
		printf("CheckBrick: Waiting for function to start!\n");
		xQueueSemaphoreTake(xSemCheckBrickStart, portMAX_DELAY);
		printf("CheckBrick: Reading the Brick\n");
		while (getCylinderStartPos() != 1) {
			if (readSensor1() && !sensor1) {
				packageType++;
				sensor1++;
				printf("CheckBrick: Sensor1 Read\n");								//DEBUG
			}
			if (readSensor2() && !sensor2) {
				packageType++;
				sensor2++;
				printf("CheckBrick: Sensor2 Read\n");								//DEBUG
			}
		}
		printf("Brick of type %d read!", packageType);
		xQueueSend(mbx_checkedBricks, &packageType, portMAX_DELAY);
	}
}
//	while (TRUE) {
//	int packageType = 1;
//	int sensor1 = 0, sensor2 = 0;
//
//	xQueueSemaphoreTake(xSemCheckPackageStart, portMAX_DELAY);
//
//	while (getCylinderStartPos() != 1) {
//
//		//			printf("a%d sensor1 %d sensor2 %d\n", packageType, sensor1, sensor2);
//		if (readSensor1() && !sensor1) {
//			packageType++;
//			sensor1++;
//			printf("Sensor1 Read\n");								//DEBUG
//		}
//		if (readSensor2() && !sensor2) {
//			packageType++;
//			sensor2++;
//			printf("Sensor2 Read\n");								//DEBUG
//		}
//	}
//	//printf("b%d\n", packageType);
//	xQueueSend(mbx_CheckPackage, &packageType, portMAX_DELAY);
//}
//

void vTaskEnterBrick(void* pvParameters) {
	while (TRUE) {
		int brickType = 1;
		xQueueSemaphoreTake(xSemBrickEntry, portMAX_DELAY);
		printf("Starting the checkBrick process!\n");
		xSemaphoreGive(xSemCheckBrickStart);
		xQueueSend(mbx_P2, &Front, portMAX_DELAY);
		//xQueueSend(mbx_CS, &Front, portMAX_DELAY);
		vTaskDelay(1000);
		blocksRemaining++;
		if (blocksRemaining == 1) {
			xSemaphoreGive(xSemConveyor);
			printf("Brick remaining was 1\n");
		}
		printf("Stop!\n");
		xQueueReceive(mbx_checkedBricks, &brickType, portMAX_DELAY);
		xQueueSend(mbx_P2, &Back, portMAX_DELAY);
		// Check user type with read type to indicate error
		printf("-----\nBrick of type %d detected!\n", brickType);

		//xQueueSend(mbx_CS, &Back, portMAX_DELAY);
		printf("Cylinder stopped!\n");
	}
}

void vTaskCalibration(void* pvParameters) {
	int CSCalibrated = 0, C1Calibrated = 0, C2Calibrated = 0;
	char cmd;
	while (TRUE) {
		printf("Waiting for calibration\n");
		xQueueSemaphoreTake(xSemCalibrationStarted, portMAX_DELAY);
		printf("Calibration received\n");

		while (!(CSCalibrated == 2) || !(C1Calibrated == 2) || !(C2Calibrated == 2)) {
			printf("\n\n\n*********************\n");
			printf("\n\n\nCalibration_Menu\n");
			printf("\n'q' CS in\n");
			printf("\n'a' CS out\n");
			printf("\n'w' C1 in\n");
			printf("\n's' C1 out\n");
			printf("\n'e' C2 in\n");
			printf("\n'd' C2 out\n");
			printf("\n'z' Try Auto-Calibration\n");
			printf("\n'x' Exit Menu\n");
			printf("\n*********************\n");
			while ((cmd = getchar()) == '\n') {}

			switch (cmd) {
			case 'q':
				printf("Cylinder Start to the front!\n");
				xQueueSend(mbx_CS, &Front, portMAX_DELAY);
				CSCalibrated++;
				break;
			case 'a':
				printf("Cylinder Start to the back!\n");
				xQueueSend(mbx_CS, &Back, portMAX_DELAY);
				CSCalibrated++;
				break;
			case 'w':
				printf("Cylinder 1 to the front!\n");
				xQueueSend(mbx_C1, &Front, portMAX_DELAY);
				C1Calibrated++;
				break;
			case 's':
				if (!C1Calibrated) {
					printf("Cylinder Uncalibrated! Please send it to the front.\n");
					break;
				}
				printf("Cylinder 1 to the back!\n");
				xQueueSend(mbx_C1, &Back, portMAX_DELAY);
				C1Calibrated++;
				break;
			case 'e':
				printf("Cylinder 2 to the front!\n");
				xQueueSend(mbx_C2, &Front, portMAX_DELAY);
				C2Calibrated++;
				break;
			case 'd':
				printf("Cylinder 2 to the back!\n");
				xQueueSend(mbx_C2, &Back, portMAX_DELAY);
				C2Calibrated++;
				break;
			}

			if (cmd == 'x') {
				printf("Leaving Calibration Menu Uncalibrated!");
				break;
			}
		}
		xSemaphoreGive(xSemCalibrationFinished);

	}
}
void vTaskMenu(void* pvParameters) {
	char cmd;

	while (TRUE) {
		do {
			printf("\n\n\n*********************\n");
			printf("******** MENU *******\n");
			printf("*********************\n");


			printf("\n(0) Calibration\n");
			printf("\n(1) Insert a brick\n");
			printf("\n(2) Show a list of stored bricks\n");
			printf("\n(3) Show a list by type of brick\n");
			printf("\n(4) Show the record of all bricks rejected.\n");
			printf("\n(5) Clean all Records\n");

			while ((cmd = getchar()) == '\n') {}

			switch (cmd) {
			case '0':
				xSemaphoreGive(xSemCalibrationStarted);
				printf("Calibration sent!\n");
				xQueueSemaphoreTake(xSemCalibrationFinished, portMAX_DELAY);
				printf("Calibration complete!\n");
				break;
			case '1':
				printf("Brick Manager request sent!\n");
				xSemaphoreGive(xSemBrickManagerStart);
				xQueueSemaphoreTake(xSemBrickManagerFinished, portMAX_DELAY);
				printf("Brick Manager complete!\n");
				break;
			}
		} while (cmd != 'x');
		printf("\n\nEnding Program. Bye bye!");
		break;
		//printf("\n(6) NºBrick on conveyor real time\n");
		//printf("\n(q) Conveyor ON\n");
		//printf("\n(w) Conveyor OFF\n");
		//printf("\n(e) Task_GOTO_CS\n");
		//printf("\n(r) Task_GOTO_C1\n");
		//printf("\n(t) Task_GOTO_C2\n");
		//printf("\n(y) Check LED\n");
		//printf("\n(x) End program.\n");
		//printf("\n*********************\n");
	}
}
void vTaskBrickManager(void* pvParameters) {
	char cmd;
	int buff;
	while (TRUE) {
		xQueueSemaphoreTake(xSemBrickManagerStart, portMAX_DELAY);
		do {
			printf("\n\n\n*********************\n");
			printf("\n\n\n**** Brick Manager ****\n");
			printf("'1' Type 1 (Brick 1) ->D1\n");
			printf("'2' Type 2 (Brick 2) ->D2\n");
			printf("'3' Type 3 (Brick 3) ->D3\n");
			printf("'x' Exit Menu\n");
			printf("***********************\n");

			while ((cmd = getchar()) == '\n') {}
			printf("Command: %c\n", cmd);
			if (cmd != 'x')
				switch (cmd) {
				case '1':
					printf("Sending order to Dock1\n");
					buff = cmd - '0';
					xQueueSend(mbx_D1, &buff, portMAX_DELAY);
					xSemaphoreGive(xSemBrickEntry);
					break;

				case '2':
					printf("Sending order to Dock2\n");
					buff = cmd - '0';
					xQueueSend(mbx_D2, &buff, portMAX_DELAY);
					xSemaphoreGive(xSemBrickEntry);
					break;
				case '3':
					printf("Sending order to Dock3\n");
					buff = cmd - '0';
					xQueueSend(mbx_D2, &buff, portMAX_DELAY);
					xSemaphoreGive(xSemBrickEntry);
					break;
				}
			xQueueSemaphoreTake(xSemDock1SensorFinished, portMAX_DELAY);
		} while (cmd != 'x');
		printf("Exiting Brick Menu!\n");
		xSemaphoreGive(xSemBrickManagerFinished);
	}
}

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
void initialisePorts() {
	printf("Initializing Ports...\n");
	createDigitalInput(0);
	createDigitalInput(1);
	createDigitalOutput(2);
	printf("Ports Initialized\n");
	writeDigitalU8(2, 0);
}
void myDaemonTaskStartupHook(void) {
	initialisePorts();

	xSemCalibrationStarted = xSemaphoreCreateCounting(100, 0);
	xSemCalibrationFinished = xSemaphoreCreateCounting(100, 0);

	mbx_P2 = xQueueCreate(20, sizeof(int));
	mbx_checkedBricks = xQueueCreate(20, sizeof(int));
	mbx_CS = xQueueCreate(10, sizeof(int));
	mbx_C1 = xQueueCreate(10, sizeof(int));
	mbx_C2 = xQueueCreate(10, sizeof(int));
	mbx_D1 = xQueueCreate(10, sizeof(int));
	mbx_D2 = xQueueCreate(10, sizeof(int));

	xSemCylinderStartStart = xSemaphoreCreateCounting(100, 0);
	xSemCylinder1Start = xSemaphoreCreateCounting(100, 0);
	xSemCylinder2Start = xSemaphoreCreateCounting(100, 0);
	xSemCylinderStartFinished = xSemaphoreCreateCounting(100, 0);
	xSemCylinder1Finished = xSemaphoreCreateCounting(100, 0);
	xSemCylinder2Finished = xSemaphoreCreateCounting(100, 0);
	xSemBrickManagerStart = xSemaphoreCreateCounting(100, 0);
	xSemBrickManagerFinished = xSemaphoreCreateCounting(100, 0);
	xSemCheckBrickStart = xSemaphoreCreateCounting(100, 0);
	xSemDock1SensorFinished = xSemaphoreCreateCounting(100, 0);
	xSemDock2SensorFinished = xSemaphoreCreateCounting(100, 0);
	xSemDock3SensorFinished = xSemaphoreCreateCounting(100, 0);
	xSemConveyor = xSemaphoreCreateCounting(100, 0);
	xSemBrickEntry = xSemaphoreCreateCounting(100, 0);
	xSemD3 = xSemaphoreCreateCounting(100, 0);

	xTaskCreate(vTaskMenu, "vTaskMenu", 100, NULL, 0, &taskMenu);
	xTaskCreate(vTaskCalibration, "vTaskCalibration", 100, NULL, 0, &taskCalibration);
	xTaskCreate(vTaskCylinderStart, "vTaskCylinderStart", 100, NULL, 0, &taskCylinderStart);
	xTaskCreate(vTaskCylinder1, "vTaskCylinder1", 100, NULL, 0, &taskCylinder1);
	xTaskCreate(vTaskCylinder2, "vTaskCylinder2", 100, NULL, 0, &taskCylinder2);
	xTaskCreate(vTaskConveyor, "vTaskConveyor", 100, NULL, 0, &taskConveyor);
	xTaskCreate(vTaskBrickManager, "vTaskBrickManager", 100, NULL, 0, &taskBrickManager);
	xTaskCreate(vTaskDock1, "vTaskDock1", 100, NULL, 0, &taskDock1);
	xTaskCreate(vTaskDock2, "vTaskDock2", 100, NULL, 0, &taskDock2);
	xTaskCreate(vTaskDock3, "vTaskDock3", 100, NULL, 0, &taskDock3);
	xTaskCreate(vTaskEnterBrick, "vTaskEnterBrick", 100, NULL, 0, &taskEnterBrick);
	xTaskCreate(vTaskP2, "vTaskP2", 100, NULL, 0, &taskP2);
	xTaskCreate(vTaskCheckBrick, "vTaskCheckBrick", 100, NULL, 0, &taskCheckBrick);
}


int main(int argc, char** argv) {
	initialiseHeap();
	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;
	vTaskStartScheduler();
}