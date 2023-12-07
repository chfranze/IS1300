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

uint8_t toggleFreq = 10;
uint8_t pedestrianDelay = 10;
uint8_t walkingDelay = 10;
uint8_t orangeDelay = 10;

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
	State state = INITIALIZATION;
	while (true) {
		switch (state) {
		case INITIALIZATION:
			//setPedestrianLightRed();
			//setCarSignalGreen();
			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET);


			if(SW78_hit())
			{
				state = BUTTON_PRESSED;
				HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
			}
			break;
		case BUTTON_PRESSED:
		    // Create a timer that waits for orange delay time
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);
			ledOn(24);

		    vTaskDelay(pdMS_TO_TICKS(orangeDelay * 500));

		    ledOff(23);
		    HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		    // When timer is finished, go to state PEDESTRIAN_CROSSING
		    state = INITIALIZATION;
			break;
		case PEDESTRIAN_CROSSING:

			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET);
		    vTaskDelay(pdMS_TO_TICKS(orangeDelay * 500));


			//setCarSignalRed();
			//setPedestrianLightGreen();
			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
			state = CAR_SIGNAL_TRANSITION;
			break;
		case CAR_SIGNAL_TRANSITION:
			//setCarSignalOrange();
			//setCarSignalGreen();
			state = PEDESTRIAN_SIGNAL_CHECK;
			break;
		case PEDESTRIAN_SIGNAL_CHECK:
			//if (isCarSignalGreen() || isCarSignalOrange()) {
			//	setPedestrianLightRed();
			//} else {
			//	setPedestrianLightGreen();
			//}
			state = BUTTON_PRESSED;
			break;
		}
	}
}

