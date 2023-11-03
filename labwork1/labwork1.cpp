/*| João Cunha 68841 | Marcos Romão 63753 | Henrique Nobre 57499 |*/
#include <iostream>
#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include <C:/Users/Renedito/Downloads/labwork_1_63753_68841_57499/labwork_1_63753_68841_57499/my_interaction_functions/my_interaction_functions.h>
//#include<my_interaction_functions/my_interaction_functions/my_interaction_functions.h>
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

#define SleepTime 20

typedef int Flag;

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

TaskHandle_t TaskLED;
TaskHandle_t taskEmergency;
TaskHandle_t ResumeTask;
TaskHandle_t goto_CS_task;
TaskHandle_t goto_C1_task;
TaskHandle_t goto_C2_task;
TaskHandle_t TaskShowBrick;
TaskHandle_t taskRestartSystem;
TaskHandle_t conveyor_task;

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
xSemaphoreHandle xSemLEDtaskStart;
xSemaphoreHandle xSemLEDtaskFinished;
xSemaphoreHandle xSemFLLEDtaskStart;
xSemaphoreHandle xSemFLLEDtaskFinished;
xSemaphoreHandle xSemToggleConveyor;
xSemaphoreHandle xSemToggleCylinderStart;
xSemaphoreHandle xSemToggleCylinder1;
xSemaphoreHandle xSemToggleCylinder2;
xSemaphoreHandle xSemShowBrickStart;
xSemaphoreHandle xSemShowBrickFinished;
xSemaphoreHandle test;


xQueueHandle mbx_entryBrick;
xQueueHandle mbx_checkedBricks;
xQueueHandle mbx_CS;
xQueueHandle mbx_C1;
xQueueHandle mbx_C2;
xQueueHandle mbx_D1;
xQueueHandle mbx_D2;
xQueueHandle mbx_Conveyor;
xSemaphoreHandle xSemD3;

Flag emergencyCalled = 0;
Flag stop = 0;
Flag simpleMode = 1;
Flag calibrated = 0;
int Front = 1;
int Back = 0;
int blocksRemaining = 0;
int SizeDock1 = 0;
int SizeDock2 = 0;
int SizeDock3 = 0;
int SizeRejected = 0;
time_t Date;

typedef struct {
	int type; // Tipo do tijolo
	time_t entryDate; // Data/hora em que o tijolo foi inserido
} brick;


brick Dock1Records[50];
brick Dock2Records[50];
brick Dock3Records[50];
brick ErrorRecords[50];

struct MovingPartsControl {
	int stop = 0;
	int CylinderStart = 0;
	int Cylinder1 = 0;
	int Cylinder2 = 0;
	int Conveyor = 0;
} MovingParts;

void ConveyorArt() {
	printf(" _____\n");
	printf("/  __ \\\n");
	printf("| /  \\/ ___  _ ____   _____ _   _  ___  _ __\n");
	printf("| |    / _ \\| '_ \\ \\ / / _ \\ | | |/ _ \| '__|\n");
	printf("| \\__/\\ (_) | | | \\ V /  __/ |_| | (_) | |\n");
	printf("\\____/ \\___/|_| |_|\\_/ \\___|\\__, |\\___/|_|\n");
	printf("                             __/ |\n");
	printf("                            |___/\n");
}

void list_delivered_bricks1() {
	printf("\n+------------------------+\n|     TYPE1 BRICKS       |\n+------------------------+\n| SizeDock1: %-10d |\n", SizeDock1);
	for (int i = 0; i < SizeDock1; i++) {
		printf("| Type: %-10d \n|Entry Date: %s\n", Dock1Records[i].type, ctime(&Dock1Records[i].entryDate));
	}
	printf("+------------------------+\n");
}
void list_delivered_bricks2() {
	printf("\n+------------------------+\n|     TYPE2 BRICKS       |\n+------------------------+\n| SizeDock2: %-10d |\n", SizeDock2);
	for (int i = 0; i < SizeDock2; i++) {
		printf("| Type: %-10d\n|Entry Date: %s\n", Dock2Records[i].type, ctime(&Dock2Records[i].entryDate));
	}
	printf("+------------------------+\n");
}
void list_delivered_bricks3() {
	printf("\n+------------------------+\n|     TYPE3 BRICKS       |\n+------------------------+\n| SizeDock3: %-10d |\n", SizeDock3);
	for (int i = 0; i < SizeDock3; i++) {
		printf("| Type: %-10d\n|Entry Date: %s\n", Dock3Records[i].type, ctime(&Dock3Records[i].entryDate));
	}
	printf("+------------------------+\n");
}
void list_Rejected_brick() {
	int array_sizeE = 0;
	array_sizeE = sizeof(ErrorRecords) / sizeof(ErrorRecords[0]);
	printf("\n+------------------------+\n|     REJECTED BRICKS     |\n+------------------------+\n");
	for (int i = 0; i < SizeRejected; i++) {
		printf("| Type: %-10d\n|Entry Date: %s\n", ErrorRecords[i].type, ctime(&ErrorRecords[i].entryDate));
	}
	printf("+------------------------+\n\n\n");
}
void clear_structs() {

	for (int i = 0; i < 50; i++) {

		Dock1Records[i].type = 0;
		Dock1Records[i].entryDate = time(NULL);

		Dock2Records[i].type = 0;
		Dock2Records[i].entryDate = time(NULL);

		Dock3Records[i].type = 0;
		Dock3Records[i].entryDate = time(NULL);

		ErrorRecords[i].type = 0;
		ErrorRecords[i].entryDate = time(NULL);
	}
	SizeDock1 = 0;
	SizeDock2 = 0;
	SizeDock3 = 0;
	SizeRejected = 0;
}
void toggleSimpleMode() {
	if (simpleMode)
		simpleMode = 0;
	else
		simpleMode = 1;
}

// Tasks

void vTaskCylinderStart(void* pvParameters) {
	int pos = 0;
	while (TRUE) {
		xQueueReceive(mbx_CS, &pos, portMAX_DELAY);


		MovingParts.CylinderStart = pos;
		gotoCylinderStart(pos);
		if (!simpleMode)
			printf("Cylinder stopped moving!\n");
		if (pos == 1) {
			xSemaphoreGive(xSemCylinderStartFinished);
		}
		if (pos == 0) {
			xSemaphoreGive(xSemCylinderStartStart);
		}
	}
}
void vTaskCylinder1(void* pvParameters) {
	int pos;
	while (TRUE) {
		xQueueReceive(mbx_C1, &pos, portMAX_DELAY);
		MovingParts.Cylinder1 = pos;
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
		MovingParts.Cylinder2 = pos;
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
	uInt8 p = readDigitalU8(2);
	int cmd;
	while (TRUE) {
		xQueueReceive(mbx_Conveyor, &cmd, portMAX_DELAY);
		if (!simpleMode)
			printf("Task_Conveyor: Received %d\n", cmd);
		switch (cmd) {
		case 0:
			if (!simpleMode)
				printf("Conveyor OFF!\n");
			conveyorOFF();
			MovingParts.Conveyor = 0;
			break;
		case 1:
			if (!simpleMode)
				printf("Conveyor ON!\n");
			conveyorON();
			MovingParts.Conveyor = 1;
			break;
		}
		MovingParts.Conveyor = getBitValue(p, 2);
	}
}
void vTaskLED(void* pvParameters) {
	while (TRUE) {
		xQueueSemaphoreTake(xSemLEDtaskStart, portMAX_DELAY);

		if (!MovingParts.stop) {
			toggleLED();
			vTaskDelay(3000);
			toggleLED();
		}
		while (MovingParts.stop == 1) {
			toggleLED();
			vTaskDelay(250);
		}
	}
}


void vTaskDock1(void* pvParameters) {
	int brickType, i = 0;
	while (TRUE) {
		xQueueReceive(mbx_D1, &brickType, portMAX_DELAY);

		if (!simpleMode)
			printf("Entered Dock1 task!\n");

		while (getCylinder1Sensor() != 1) {}
		if (brickType == 1) {
			xQueueSend(mbx_Conveyor, &Back, portMAX_DELAY);

			if (!simpleMode) {
				printf("Dock1: Brick of type %d received!\n", brickType);
				printf("Dock1: Conveyor OFF!\n");
			}

			xQueueSend(mbx_C1, &Front, portMAX_DELAY);
			xQueueSend(mbx_C1, &Back, portMAX_DELAY);

			xQueueSemaphoreTake(xSemCylinder1Start, portMAX_DELAY);

			blocksRemaining--;
			SizeDock1++;

			if (simpleMode)
				printf("Dock1: Brick of type %d received!\n       Number of bricks in Dock1: %d\n       Number of Bricks remaining: %d\n", brickType, SizeDock1, blocksRemaining);

			if (blocksRemaining > 0)
				xQueueSend(mbx_Conveyor, &Front, portMAX_DELAY);
			if (!simpleMode)
				printf("Blocks remaining: %d\n", blocksRemaining);
		}
		xSemaphoreGive(xSemDock1SensorFinished);
	}
}
void vTaskDock2(void* pvParameters) {
	int brickType, i = 0, j = 0, k = 0;
	while (TRUE) {
		xQueueReceive(mbx_D2, &brickType, portMAX_DELAY);

		if (!simpleMode)
			printf("Entered Dock2 task!\n");

		while (getCylinder2Sensor() != 1) {}

		switch (brickType) {
		case 2:
			xQueueSend(mbx_Conveyor, &Back, portMAX_DELAY);

			xQueueSend(mbx_C2, &Front, portMAX_DELAY);
			xQueueSend(mbx_C2, &Back, portMAX_DELAY);

			xQueueSemaphoreTake(xSemCylinder2Start, portMAX_DELAY);

			blocksRemaining--;
			SizeDock2++;

			if (simpleMode)
				printf("Dock2: Brick of type %d received!\n       Number of bricks in Dock2: %d\n       Number of Bricks remaining: %d\n", brickType, SizeDock1, blocksRemaining);

			if (!simpleMode)
				printf("Blocks remaining: %d\n", blocksRemaining);
			if (blocksRemaining > 0)
				xQueueSend(mbx_Conveyor, &Front, portMAX_DELAY);
			break;
		case 3:
			if (!simpleMode)
				printf("Sending to dock3\n");
			xSemaphoreGive(xSemD3);
			vTaskDelay(500);
			break;
		}
	}
}
void vTaskDock3(void* pvParameters) {
	int i = 0;
	while (TRUE) {
		xQueueSemaphoreTake(xSemD3, portMAX_DELAY);

		if (!simpleMode)
			printf("Entered Dock3 task!\n");

		vTaskDelay(5000);

		while (emergencyCalled) {
			emergencyCalled = 0;
			vTaskDelay(1500);
		}
		blocksRemaining--;
		SizeDock3++;
		if (simpleMode)
			printf("Dock3: Brick of type 3 or Error received!\n       Number of bricks in Dock3: %d\n       Number of Bricks remaining: %d\n", SizeDock1, blocksRemaining);
		if (!simpleMode)
			printf("Blocks Remaining: %d\n", blocksRemaining);
		if (blocksRemaining == 0)
			xQueueSend(mbx_Conveyor, &Back, portMAX_DELAY);
	}
}


void vTaskCheckBrick(void* pvParameters) {

	while (TRUE) {
		int packageType = 1, sensor1 = 0, sensor2 = 0;
		xQueueSemaphoreTake(xSemCheckBrickStart, portMAX_DELAY);
		if (!simpleMode)
			printf("CheckBrick: Reading the Brick\n");
		while (getCylinderStartPos() != 1) {
			if (readSensor1() && !sensor1) {
				packageType++;
				sensor1++;
				if (!simpleMode)
					printf("CheckBrick: Sensor1 Read\n");								//DEBUG
			}
			if (readSensor2() && !sensor2) {
				packageType++;
				sensor2++;
				if (!simpleMode)
					printf("CheckBrick: Sensor2 Read\n");								//DEBUG
			}
		}
		if (!simpleMode)
			printf("CheckBrick: Brick of type %d read!\n", packageType);
		xQueueSend(mbx_checkedBricks, &packageType, portMAX_DELAY);
	}
}
void vTaskEnterBrick(void* pvParameters) {
	int readType, userType, i = 0, j = 0, k = 0, l = 0;
	while (TRUE) {
		xQueueReceive(mbx_entryBrick, &userType, portMAX_DELAY);
		if (!simpleMode)
			printf("EnterBrick: User said brick type %d\n", userType);
		if (!simpleMode)
			printf("EnterBrick: Starting the checkBrick process!\n");
		xSemaphoreGive(xSemCheckBrickStart);
		xQueueSend(mbx_CS, &Front, portMAX_DELAY);;

		// Wait for conveyor to catch brick
		vTaskDelay(1000);

		// Check if there are bricks on the conveyor to toggle
		blocksRemaining++;
		if (blocksRemaining == 1) {
			if (!simpleMode)
				printf("EnterBrick: Brick remaining was 1, sending order to turn on the Conveyor\n");
			xQueueSend(mbx_Conveyor, &Front, portMAX_DELAY);
		}

		xQueueReceive(mbx_checkedBricks, &readType, portMAX_DELAY);
		xQueueSend(mbx_CS, &Back, portMAX_DELAY);

		Date = time(NULL);

		// Check user type with read type to indicate error
		printf("Brick of type %d detected!\n", readType);

		if (readType == userType) {
			printf("The given brick is correct!\n");
			switch (readType) {
			case 1:
				if (!simpleMode)
					printf("Sending order to Dock1\n");
				Dock1Records[i].type = readType;
				Dock1Records[i].entryDate = Date;
				i++;
				xQueueSend(mbx_D1, &readType, portMAX_DELAY);
				break;

			case 2:
				if (!simpleMode)
					printf("Sending order to Dock2\n");
				Dock2Records[j].type = readType;
				Dock2Records[j].entryDate = Date;
				j++;
				xQueueSend(mbx_D1, &readType, portMAX_DELAY);
				xQueueSend(mbx_D2, &readType, portMAX_DELAY);
				break;
			case 3:
				if (!simpleMode)
					printf("Sending order to Dock3\n");
				Dock3Records[k].type = 3;
				Dock3Records[k].entryDate = Date;
				k++;
				xQueueSend(mbx_D1, &readType, portMAX_DELAY);
				xQueueSend(mbx_D2, &readType, portMAX_DELAY);
				break;
			}
		}
		else {
			printf("Error! Wrong type of brick given to the program!\n");
			xSemaphoreGive(xSemLEDtaskStart);
			SizeRejected++;
			ErrorRecords[l].type = readType;
			ErrorRecords[l].entryDate = Date;
			l++;
			readType = 3; // Code for error
			xQueueSend(mbx_D1, &readType, portMAX_DELAY);
			xQueueSend(mbx_D2, &readType, portMAX_DELAY);
		}
		if (!simpleMode)
			printf("Finished delivering Brick!\n");
	}
}
void vTaskBrickManager(void* pvParameters) {
	char cmd;
	int buff;
	while (TRUE) {
		xQueueSemaphoreTake(xSemBrickManagerStart, portMAX_DELAY);
		do {
			printf("\n+------------------------+\n|     BRICK MANAGER       |\n+------------------------+\n");
			printf("| '1' Type 1 (Brick 1) ->D1|\n");
			printf("| '2' Type 2 (Brick 2) ->D2|\n");
			printf("| '3' Type 3 (Brick 3) ->D3|\n");
			printf("| 'x' Exit Menu           |\n");
			printf("+------------------------+\n");
			printf("> ");
			while ((cmd = getchar()) == '\n') {}
			if (!simpleMode)
				printf("Command: %c\n", cmd);
			if (cmd != 'x') {
				buff = cmd - '0';
				printf("Sending the brick of type %d to the given dock!\n", buff);
				xQueueSend(mbx_entryBrick, &buff, portMAX_DELAY);
				xQueueSemaphoreTake(xSemDock1SensorFinished, portMAX_DELAY);
				printf("TESTEST\nTESTEST\n");
			}
		} while (cmd != 'x');
		if (!simpleMode)
			printf("Exiting Brick Menu!\n");
		xSemaphoreGive(xSemBrickManagerFinished);
	}
}


void vTaskCalibration(void* pvParameters) {
	int CSCalibrated = 0, C1Calibrated = 0, C2Calibrated = 0;
	char cmd;
	while (TRUE) {
		xQueueSemaphoreTake(xSemCalibrationStarted, portMAX_DELAY);
		printf("\n+--------------------------+\n");
		printf("|     Calibration Menu     |\n");
		printf("+--------------------------+\n");
		printf("| (q) - CS in              |\n");
		printf("| (a) - CS out             |\n");
		printf("| (w) - C1 in              |\n");
		printf("| (s) - C1 out             |\n");
		printf("| (e) - C2 in              |\n");
		printf("| (d) - C2 out             |\n");
		printf("| (x) - Exit Menu          |\n");
		printf("+--------------------------+\n");
		printf("\nPlease Calibrate the System.\n\n");
		while (!(CSCalibrated == 2) || !(C1Calibrated == 2) || !(C2Calibrated == 2)) {
			printf("> ");

			while ((cmd = getchar()) == '\n') {}

			switch (cmd) {
			case 'q':
				if (!simpleMode)
					printf("Cylinder Start to the front!\n");
				xQueueSend(mbx_CS, &Front, portMAX_DELAY);
				CSCalibrated++;
				break;
			case 'a':
				if (!simpleMode)
					printf("Cylinder Start to the back!\n");
				xQueueSend(mbx_CS, &Back, portMAX_DELAY);
				CSCalibrated++;
				break;
			case 'w':
				if (!simpleMode)
					printf("Cylinder 1 to the front!\n");
				xQueueSend(mbx_C1, &Front, portMAX_DELAY);
				C1Calibrated++;
				break;
			case 's':
				if (!C1Calibrated) {
					printf("Cylinder Uncalibrated! Please send it to the front.\n");
					break;
				}
				if (!simpleMode)
					printf("Cylinder 1 to the back!\n");
				xQueueSend(mbx_C1, &Back, portMAX_DELAY);
				C1Calibrated++;
				break;
			case 'e':
				if (!simpleMode)
					printf("Cylinder 2 to the front!\n");
				xQueueSend(mbx_C2, &Front, portMAX_DELAY);
				C2Calibrated++;
				break;
			case 'd':
				if (!C2Calibrated) {
					printf("Cylinder Uncalibrated! Please send it to the front.\n");
					break;
				}
				if (!simpleMode)
					printf("Cylinder 2 to the back!\n");
				xQueueSend(mbx_C2, &Back, portMAX_DELAY);
				C2Calibrated++;
				break;
			}

			if (cmd == 'x') {
				printf("Leaving Calibration Menu Uncalibrated!\n");
				break;
			}
		}
		calibrated = 1;
		xSemaphoreGive(xSemCalibrationFinished);

	}
}
void vTaskMenu(void* pvParameters) {
	char cmd;
	ConveyorArt();
	printf("| Joao Cunha 68841 | Marcos Romao 63753 | Henrique Nobre 57499 |");
	while (TRUE) {
		do {
			if (!calibrated) {
				xSemaphoreGive(xSemCalibrationStarted);
				xQueueSemaphoreTake(xSemCalibrationFinished, portMAX_DELAY);
				printf("Calibration complete!\n");
			}


			//ConveyorArt();
			printf("\n+------------------------+\n");
			printf("|        MENU            |\n");
			printf("+------------------------+\n");
			printf("| (0) Calibration        |\n");
			printf("| (1) Insert a brick     |\n");
			printf("| (2) Show a list of     |\n");
			printf("|     stored bricks      |\n");
			printf("| (3) Show a list by     |\n");
			printf("|     type of brick      |\n");
			printf("| (4) Clean all Records  |\n");
			printf("|                        |\n");
			printf("| (S) Toggle Simple Mode |\n");
			printf("|    Disables DEBUG info |\n");
			printf("|    Currently: %s      |\n", simpleMode ? "ON " : "OFF");
			printf("|                        |\n");
			printf("| (x) Close the program  |\n");
			printf("+------------------------+\n");
			printf("> ");
			while ((cmd = getchar()) == '\n') {}

			switch (cmd) {
			case '0':
				if (calibrated)
					printf("Calibration already completed!\n");
				else {
					xSemaphoreGive(xSemCalibrationStarted);
					xQueueSemaphoreTake(xSemCalibrationFinished, portMAX_DELAY);
					printf("Calibration complete!\n");
				}
				break;
			case '1':
				if (!simpleMode)
					printf("Brick Manager request sent!\n");
				xSemaphoreGive(xSemBrickManagerStart);
				xQueueSemaphoreTake(xSemBrickManagerFinished, portMAX_DELAY);
				if (!simpleMode)
					printf("Brick Manager complete!\n");
				break;
			case '2':
				printf("*********************\n");
				list_delivered_bricks1();
				list_delivered_bricks2();
				list_delivered_bricks3();
				list_Rejected_brick();
				printf("*********************\n");
				break;
			case '3':
				xSemaphoreGive(xSemShowBrickStart);
				xQueueSemaphoreTake(xSemShowBrickFinished, portMAX_DELAY);
				break;
			case '4':
				clear_structs();
				printf("Cleared all records!\n");
				break;
			case 'S':
				toggleSimpleMode();
			}
		} while (cmd != 'x');
		ConveyorArt();
		printf("\n\nProfessor, merece mais que 18?\nEnding Program. Bye bye!\n");
		break;
	}
}
void vTaskShowBrick(void* pvParameters) {
	char cmd;
	int buff;
	while (TRUE) {
		xQueueSemaphoreTake(xSemShowBrickStart, portMAX_DELAY);
		do {
			printf("\n+--------------------------------+\n| Show Brick Records by type     |\n+--------------------------------+\n");
			printf("| '1' Type 1                     |\n");
			printf("| '2' Type 2                     |\n");
			printf("| '3' Type 3                     |\n");
			printf("| '4' Error                      |\n");
			printf("| 'x' Exit Menu                  |\n");
			printf("+--------------------------------+\n");
			while ((cmd = getchar()) == '\n') {}

			switch (cmd) {
			case '1':
				list_delivered_bricks1();
				break;
			case '2':
				list_delivered_bricks2();
				break;
			case '3':
				list_delivered_bricks3();
				break;
			case '4':
				list_Rejected_brick();
				break;
			}
		} while (cmd != 'x');
		if (!simpleMode)
			printf("Exiting Brick Shower!\n");
		xSemaphoreGive(xSemShowBrickFinished);
	}
}


void vgoto_CS_task(void* pvParameters) {
	while (TRUE)
	{
		xQueueSemaphoreTake(xSemToggleCylinderStart, portMAX_DELAY);
		if (MovingParts.stop) {
			stopCylinderStart();
		}
		else {
			if (getCylinderStartPos != 0) {
				gotoCylinderStart(MovingParts.CylinderStart);
			}
		}
	}
}
void vgoto_C1_task(void* pvParameters) {
	while (TRUE)
	{
		xQueueSemaphoreTake(xSemToggleCylinder1, portMAX_DELAY);
		if (MovingParts.stop) {
			stopCylinder1();
		}
		else {
			//if (getCylinder1Pos != 0) {
			gotoCylinder1(MovingParts.Cylinder1);
			//}
		}
	}
}
void vgoto_C2_task(void* pvParameters) {
	while (TRUE)
	{
		xQueueSemaphoreTake(xSemToggleCylinder2, portMAX_DELAY);
		if (MovingParts.stop) {
			stopCylinder2();
		}
		else {
			//if (getCylinder2Pos != 0) {
			gotoCylinder2(MovingParts.Cylinder2);
			//}
		}
	}
}
void vconveyor_task(void* pvParameters) {
	while (TRUE) {
		xQueueSemaphoreTake(xSemToggleConveyor, portMAX_DELAY);
		if (MovingParts.stop) {
			conveyorOFF();
		}
		else {
			if (blocksRemaining != 0)
				conveyorON();
		}
		xSemaphoreGive(test);
	}
}


void vTaskEmergency(void* pvParameters) {

	// The task being suspended and resumed.
	while (TRUE) {
		// The task suspends itself.
		vTaskSuspend(NULL);
		// The task is now suspended, so will not reach here until the ISR resumes it.
		printf("\n**** EMERGENCY task**** \n");
		// The task suspends all other tasks.
		MovingParts.stop = 1;
		emergencyCalled = 1;

		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleCylinderStart);
		if (!simpleMode)
			printf("Cylinder Start OFF\n");
		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleCylinder1);
		if (!simpleMode)
			printf("Cylinder 1 OFF\n");
		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleCylinder2);
		if (!simpleMode)
			printf("Cylinder 2 OFF\n");
		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleConveyor);
		if (!simpleMode)
			printf("Conveyor OFF \n");

		xQueueSemaphoreTake(test, portMAX_DELAY);


		//printf("Menu Supended\n");
		//vTaskSuspend(taskMenu);
		//printf("Calibration Supended\n");
		//vTaskSuspend(taskCalibration);
		//printf("Conveyor Supended\n");
		//vTaskSuspend(taskConveyor);
		//printf("Dock 1 Supended\n");
		//vTaskSuspend(taskDock1);
		//printf("Dock 2 Supended\n");
		//vTaskSuspend(taskDock2);
		//printf("Dock 3 Supended\n");
		//vTaskSuspend(taskDock3);
		//printf("Cylinder Start Supended\n");
		//vTaskSuspend(taskCylinderStart);
		//printf("Cylinder 1 Supended\n");
		//vTaskSuspend(taskCylinder1);
		//printf("Cylinder 2 Supended\n");
		//vTaskSuspend(taskCylinder2);
		//printf("Enter Brick Supended\n");
		//vTaskSuspend(taskEnterBrick);
		//printf("Brick Manager Supended\n");
		//vTaskSuspend(taskBrickManager);
		//printf("Check Brick Supended\n");
		//vTaskSuspend(taskCheckBrick);
		//printf("P2 Supended\n");
		//vTaskSuspend(taskP2);

		printf("ALL TASKS SUSPENDED!\n");
		//vTaskResume(TaskLED);
		xSemaphoreGive(xSemLEDtaskStart);
	}
}
void vTaskResume(void* pvParameters) {
	// The task being suspended and resumed.
	while (TRUE) {
		// The task suspends itself.
		vTaskSuspend(NULL);
		// The task is now suspended, so will not reach here until the ISR resumes it.
		printf("\n **** RESUME task ****\n");

		MovingParts.stop = 0;

		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleCylinderStart);
		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleCylinder1);
		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleCylinder2);
		//Sleep(SleepTime);
		xSemaphoreGive(xSemToggleConveyor);


		//printf("%d\n", getLEDState());

		if (getLEDState()) {
			toggleLED();
		}

		//vTaskSuspend(TaskLED);

		//printf("%d\n", getLEDState());
		//printf("Menu Restored\n");
		//vTaskResume(taskMenu);
		//printf("Calibration Restored\n");
		//vTaskResume(taskCalibration);
		//printf("Cylinder Start Restored\n");
		//vTaskResume(taskCylinderStart);
		//printf("Cylinder 1 Restored\n");
		//vTaskResume(taskCylinder1);
		//printf("Cylinder 2 Restored\n");
		//vTaskResume(taskCylinder2);
		//printf("Conveyor Restored\n");
		//vTaskResume(taskConveyor);
		//printf("Brick Manager Restored\n");
		//vTaskResume(taskBrickManager);
		//printf("Dock 1 Restored\n");
		//vTaskResume(taskDock1);
		//printf("Dock 2 Restored\n");
		//vTaskResume(taskDock2);
		//printf("Dock 3 Restored\n");
		//vTaskResume(taskDock3);
		//printf("Enter Brick Restored\n");
		//vTaskResume(taskEnterBrick);
		//printf("Check Brick Restored\n");
		//vTaskResume(taskCheckBrick);
		//printf("P2 Restored\n");
		//vTaskResume(taskP2);

		//vTaskSuspend(TaskLED);
		//xQueueSemaphoreTake(xSemLEDtaskFinished, portMAX_DELAY);

	}
}
void vTaskRestartSystem(void* pvParameters) {
	while (TRUE) {
		vTaskSuspend(NULL);
		if (MovingParts.stop == 1) {
			//clean the queue
			if (!simpleMode)
				printf("Clearing the Records!\n");
			clear_structs();
			blocksRemaining = 0;
			calibrated = 0;

			xQueueReset(mbx_entryBrick);
			xQueueReset(mbx_checkedBricks);
			xQueueReset(mbx_CS);
			xQueueReset(mbx_C1);
			xQueueReset(mbx_C2);
			xQueueReset(mbx_D1);
			xQueueReset(mbx_D2);
			xQueueReset(mbx_Conveyor);
			xQueueReset(xSemD3);

			xQueueReset(xSemCalibrationStarted);
			xQueueReset(xSemCalibrationFinished);
			xQueueReset(xSemCylinderStartStart);
			xQueueReset(xSemCylinder1Start);
			xQueueReset(xSemCylinder2Start);
			xQueueReset(xSemCylinderStartFinished);
			xQueueReset(xSemCylinder1Finished);
			xQueueReset(xSemCylinder2Finished);
			xQueueReset(xSemCheckBrickStart);
			xQueueReset(xSemBrickManagerStart);
			xQueueReset(xSemBrickManagerFinished);
			xQueueReset(xSemDock1SensorFinished);
			xQueueReset(xSemDock2SensorFinished);
			xQueueReset(xSemDock3SensorFinished);
			xQueueReset(xSemLEDtaskStart);
			xQueueReset(xSemLEDtaskFinished);
			xQueueReset(xSemFLLEDtaskStart);
			xQueueReset(xSemFLLEDtaskFinished);
			xQueueReset(xSemToggleConveyor);
			xQueueReset(xSemToggleCylinderStart);
			xQueueReset(xSemToggleCylinder1);
			xQueueReset(xSemToggleCylinder2);
			xQueueReset(xSemShowBrickStart);
			xQueueReset(xSemShowBrickFinished);
			xQueueReset(test);

			MovingParts.Conveyor = 0;
			MovingParts.Cylinder1 = 0;
			MovingParts.Cylinder2 = 0;
			MovingParts.CylinderStart = 0;
			//vTaskResume(taskMenu);//restart the execution
			xSemaphoreGive(xSemBrickManagerFinished);
			xSemaphoreGive(xSemCalibrationStarted, portMAX_DELAY);//recalibrate the system
		}
		else {
			printf("You can only use the Dock END key while the system is in an Interrupt!\n");
			break;
		}
	}
}


void switch1_rising_isr(ULONGLONG lastTime) {
	// GetTickCount64() current time in miliseconds
	// since the system has started...
	ULONGLONG time = GetTickCount64();
	if (!simpleMode)
		printf("\nSwitch one RISING detected at time = %llu...\n", time);
	BaseType_t xYieldRequired;
	// Resume the suspended task.
	xYieldRequired = xTaskResumeFromISR(taskEmergency);
}
void switch2_rising_isr(ULONGLONG lastTime) {
	ULONGLONG time = GetTickCount64();
	if (!simpleMode)
		printf("\nSwitch two RISING detected at time = %llu...\n", time);
	BaseType_t xYieldRequired;
	// Resume the suspended task.
	xYieldRequired = xTaskResumeFromISR(ResumeTask);
}
void switch3_rising_isr(ULONGLONG lastTime) {
	ULONGLONG time = GetTickCount64();
	if (!simpleMode)
		printf("\nSwitch three RISING detected at time = %llu...\n", time);
	BaseType_t xYieldRequired;
	xYieldRequired = xTaskResumeFromISR(taskRestartSystem);
}


// Given Functions
void vAssertCalled(unsigned long ulLine, const char* const pcFileName) {
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
static void initialiseHeap(void) {
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

	mbx_checkedBricks = xQueueCreate(20, sizeof(int));
	mbx_entryBrick = xQueueCreate(20, sizeof(int));
	mbx_CS = xQueueCreate(10, sizeof(int));
	mbx_C1 = xQueueCreate(10, sizeof(int));
	mbx_C2 = xQueueCreate(10, sizeof(int));
	mbx_D1 = xQueueCreate(10, sizeof(int));
	mbx_D2 = xQueueCreate(10, sizeof(int));
	mbx_Conveyor = xQueueCreate(10, sizeof(int));
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
	test = xSemaphoreCreateCounting(100, 0);

	xSemD3 = xSemaphoreCreateCounting(100, 0);
	xSemLEDtaskStart = xSemaphoreCreateCounting(100, 0);
	xSemLEDtaskFinished = xSemaphoreCreateCounting(100, 0);
	xSemFLLEDtaskStart = xSemaphoreCreateCounting(100, 0);
	xSemFLLEDtaskFinished = xSemaphoreCreateCounting(100, 0);
	xSemToggleConveyor = xSemaphoreCreateCounting(100, 0);

	xSemToggleCylinderStart = xSemaphoreCreateCounting(100, 0);
	xSemToggleCylinder1 = xSemaphoreCreateCounting(100, 0);
	xSemToggleCylinder2 = xSemaphoreCreateCounting(100, 0);

	xSemShowBrickStart = xSemaphoreCreateCounting(100, 0);
	xSemShowBrickFinished = xSemaphoreCreateCounting(100, 0);

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
	xTaskCreate(vTaskCheckBrick, "vTaskCheckBrick", 100, NULL, 0, &taskCheckBrick);

	xTaskCreate(vTaskShowBrick, "vTaskShowBrick", 100, NULL, 0, &TaskShowBrick);
	xTaskCreate(vTaskLED, "vTaskLED", 100, NULL, 0, &TaskLED);
	xTaskCreate(vgoto_CS_task, "vgoto_CS_task", 100, NULL, 0, &goto_CS_task);
	xTaskCreate(vgoto_C1_task, "vgoto_C1_task", 100, NULL, 0, &goto_C1_task);
	xTaskCreate(vgoto_C2_task, "vgoto_C2_task", 100, NULL, 0, &goto_C2_task);
	//xTaskCreate(vconveyor_task, "vconveyor_task", 100, NULL, 0, &conveyor_task);
	xTaskCreate(vTaskEmergency, "vTaskEmergency", 100, NULL, 0, &taskEmergency);
	xTaskCreate(vTaskResume, "vTaskResume", 100, NULL, 0, &ResumeTask);
	xTaskCreate(vTaskRestartSystem, "vTaskRestartSystem", 100, NULL, 0, &taskRestartSystem);
	xTaskCreate(vconveyor_task, "vconveyor_task", 100, NULL, 0, &conveyor_task);


	attachInterrupt(1, 4, switch1_rising_isr, RISING); //DOCK1 STOP
	attachInterrupt(1, 3, switch2_rising_isr, RISING); //DOCK2 RESUME
	attachInterrupt(1, 2, switch3_rising_isr, RISING); //DOCK END CLEAN ALL
}


int main(int argc, char** argv) {
	initialiseHeap();
	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;
	vTaskStartScheduler();
}