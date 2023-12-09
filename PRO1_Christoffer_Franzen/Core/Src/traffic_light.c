/**
 ******************************************************************************
 @brief Traffic light, program for controlling traffic light
 @file traffic_light.c
 @author Christoffer Franzén
 @version 1.0
 @date 18-November-2023
 @brief Contain the functions and definitions related to the operations of the traffic light
 ******************************************************************************
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "utilities.h"
#include "FreeRTOS.h"
#include "timers.h"

TimerHandle_t pedestrianTimer;

uint8_t toggleFreq = 5;
uint8_t pedestrianDelay = 100;
uint8_t walkingDelay = 100;
uint8_t orangeDelay = 50;

// State enumeration
typedef enum {
	INITIALIZATION,
	BUTTON_PRESSED,
	PEDESTRIAN_CROSSING,
	CAR_SIGNAL_TRANSITION,
	PEDESTRIAN_SIGNAL_CHECK
} State;

// Function prototypes
bool isButtonPressed();
void toggleIndicatorLight();
void setPedestrianLightRed();
void setPedestrianLightGreen();
void setCarSignalGreen();
void setCarSignalOrange();
void setCarSignalRed();
void pedestrianTimerCallback(TimerHandle_t xTimer);

/**
 @brief
 @param
 @param
 @return void
 */
void TrafficLightInitialization(void) {
	int InitializeLEDs[] = { 19, 20, 6, 12 };

	for (int i = 0; i < 4; i++) {
		ledOn(InitializeLEDs[i]);
	}
}

/**
 @brief
 @param
 @param
 @return void
 */
void TrafficLightStateMachine(void) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(100); // Delay for 1 second
	State state = INITIALIZATION;
	while (true) {
		switch (state) {
		case INITIALIZATION:
			TrafficLightInitialization();
			ledOn(19);
			ledOn(20);
			ledOn(6);
			ledOn(12);

			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET);

			if (SW78_hit()) {
				state = BUTTON_PRESSED;
				HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
			}
			break;
		case BUTTON_PRESSED:
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);

			// Start timer
			xLastWakeTime = xTaskGetTickCount(); // Initialise the xLastWakeTime variable with the current time.
			for (int i = 0; i < pedestrianDelay; i++) {
				vTaskDelayUntil(&xLastWakeTime, xFrequency); // Wait for the next cycle.

				// Toggle LED 23
				if (i % toggleFreq == 0) {
					ledOn(23);
				}else{
					ledOff(23);
				}
			}

			ledOff(12);
			ledOff(6);

			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);

			state = CAR_SIGNAL_TRANSITION;
			break;
		case PEDESTRIAN_CROSSING:
			ledOff(12);
			ledOff(6);
			ledOff(20);

			ledOn(10);
			ledOn(4);
			ledOn(22);


			xLastWakeTime = xTaskGetTickCount(); // Initialise the xLastWakeTime variable with the current time.
			for (int i = 0; i < walkingDelay; i++) {
				vTaskDelayUntil(&xLastWakeTime, xFrequency); // Wait for the next cycle.
			}

			ledOn(12);
			ledOn(6);
			ledOn(20);

			ledOff(10);
			ledOff(4);
			ledOff(22);

			state = INITIALIZATION;
			break;
		case CAR_SIGNAL_TRANSITION:
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);

			ledOn(11);
			ledOn(5);

			xLastWakeTime = xTaskGetTickCount(); // Initialise the xLastWakeTime variable with the current time.
			for (int i = 0; i < orangeDelay; i++) {
				vTaskDelayUntil(&xLastWakeTime, xFrequency); // Wait for the next cycle.

				// Toggle LED 23
				if (i % toggleFreq == 0) {
					ledOn(23);
				}else{
					ledOff(23);
				}
			}

			ledOff(11);
			ledOff(5);

			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
			//setCarSignalGreen();
			state = PEDESTRIAN_CROSSING;
			break;
		}
	}
}

