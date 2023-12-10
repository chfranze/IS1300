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
#include "switch_handlers.h"

TimerHandle_t pedestrianTimer;

uint8_t toggleFreq = 5;
uint8_t pedestrianDelay = 100;
uint8_t walkingDelay = 100;
uint8_t orangeDelay = 50;

// State enumeration
typedef enum {
	INITIALIZATION,
	BUTTON_PRESSED,
	CAR_SIGNAL_TRANSITION,
	PEDESTRIAN_CROSSING,
} State;

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
void toggleLED(int i, int led) {
	if (i % toggleFreq == 0) {
		ledOn(led);
	} else {
		ledOff(led);
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
			TrafficLightInitialization(); // Set initial state (R1.1)

			ledOn(19);
			ledOn(20);
			ledOn(6);
			ledOn(12);

			if (SW78_hit()) {
				state = BUTTON_PRESSED;
			}
			break;
		case BUTTON_PRESSED:
			xLastWakeTime = xTaskGetTickCount(); //
			for (int i = 0; i < pedestrianDelay; i++) {	// Cross walk shall be red pedestrianDelay after the pedestrian pushed the button (R1.3)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
				toggleLED(i, 23); // Toggle LED 23 (R1.2)
			}

			ledOff(12);
			ledOff(6);

			state = CAR_SIGNAL_TRANSITION;
			break;
		case CAR_SIGNAL_TRANSITION:
			ledOn(11);
			ledOn(5);

			xLastWakeTime = xTaskGetTickCount();
			for (int i = 0; i < orangeDelay; i++) {	// A car signal transitions from red to orange to green, or from green to orange to red, remaining orange for orangeDelay (R1.6)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
				toggleLED(i, 23); // Toggle LED 23 (R1.2)
			}

			ledOff(11);
			ledOff(5);

			state = PEDESTRIAN_CROSSING;
			break;
		case PEDESTRIAN_CROSSING:
			ledOff(12);
			ledOff(6);
			ledOff(20);

			ledOn(10);
			ledOn(4);  // The pedestrian signal must be red when the car signal of each active lane that
			ledOn(22); // crosses the cross walk is either green or orange, and green otherwise (R1.5)

			xLastWakeTime = xTaskGetTickCount();
			for (int i = 0; i < walkingDelay; i++) { // The pedestrian signal stays green for walkingDelay (R1.4)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
			}

			ledOn(12);
			ledOn(6);
			ledOn(20);

			ledOff(10);
			ledOff(4);
			ledOff(22);

			state = INITIALIZATION;
			break;
		}
	}
}

