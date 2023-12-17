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
#include "traffic_light.h"
#include "global.h"

const TickType_t pedestrianDelay = pdMS_TO_TICKS(8000);
const TickType_t toggleFreq = pdMS_TO_TICKS(250);
const TickType_t orangeDelay = pdMS_TO_TICKS(5000);
const TickType_t walkingDelay = pdMS_TO_TICKS(10000);
const TickType_t greenDelay = pdMS_TO_TICKS(5000);
const TickType_t redDelayMax = pdMS_TO_TICKS(5000);

/**
 * @brief Initializes a TrafficLight object.
 * @param tl A pointer to the TrafficLight object to be initialized.
 * @return void
 */
void initialize(TrafficLight *tl) {
	tl->north = RED;
	tl->south = RED;
	tl->east = GREEN;
	tl->west = GREEN;
	tl->pedestrianNS = RED;
	tl->pedestrianEW = RED;
	tl->button_pressed_NS = false;
	tl->button_pressed_EW = false;
	for (int i = 0; i < 4; i++) {
		tl->car_present[i] = false;
	}
	updateLEDs(tl);
}

/**
 @brief
 @param
 @return void
 */
void trafficLightStateMachine(TrafficLight *tl) {
	static TickType_t lastChangeTime = 0;
	static TickType_t lastChangeTime2 = 0;
	static TickType_t lastChangeTime3 = 0;
	static TickType_t lastChangeTime4 = 0;

	if ((xTaskGetTickCount() - lastChangeTime) >= greenDelay) {
		bool all_false = true;
		for (int i = 0; i < 4; i++) {
			if (tl->car_present[i]) {
				all_false = false;
				break;
			}
		}

		if (all_false) {
			if (tl->north == RED && tl->south == RED && tl->pedestrianNS == RED
					&& tl->pedestrianEW == RED) {
				tl->east = RED;
				tl->west = RED;
				tl->north = GREEN;
				tl->south = GREEN;
				updateLEDs(tl);
			} else if (tl->east == RED && tl->west == RED
					&& tl->pedestrianNS == RED && tl->pedestrianEW == RED) {
				tl->north = RED;
				tl->south = RED;
				tl->east = GREEN;
				tl->west = GREEN;
				updateLEDs(tl);
			}
		}
		lastChangeTime = xTaskGetTickCount();
	}

	if (tl->button_pressed_NS) {
		// Pedestrian button for North-South has been pressed
		tl->button_pressed_NS = false; // Reset the button state
		lastChangeTime3 = xTaskGetTickCount();

		// if tl->north = tl->south is RED
		if (tl->north == RED && tl->south == RED) {
			tl->pedestrianNS = GREEN;
			updateLEDs(tl);

			// R1.4 The pedestrian signal stays green for walkingDelay ms
			vTaskDelay(walkingDelay);

			tl->pedestrianNS = RED;
			updateLEDs(tl);
		} else {
			ledID = 23;
			vTaskResume(ToggleLEDHandle);

			// R1.3
			while ((xTaskGetTickCount() - lastChangeTime3)
					<= pedestrianDelay - orangeDelay) {
				vTaskDelay(1);
			}

			// Change the state of the traffic lights
			tl->north = tl->south = ORANGE;
			// Update the LEDs
			updateLEDs(tl);

			vTaskDelay(orangeDelay);

			vTaskSuspend(ToggleLEDHandle);

			tl->pedestrianNS = GREEN;
			tl->north = tl->south = RED;
			// Update the LEDs
			updateLEDs(tl);

			// R1.4 The pedestrian signal stays green for walkingDelay ms
			vTaskDelay(walkingDelay - orangeDelay);

			// Change the state of the pedestrian light to RED
			tl->north = tl->south = ORANGE;
			updateLEDs(tl);

			vTaskDelay(orangeDelay);

			tl->north = tl->south = GREEN;
			tl->pedestrianNS = RED;
			updateLEDs(tl);
		}
	} else if (tl->button_pressed_EW) {
		// Pedestrian button for East-West has been pressed
		tl->button_pressed_EW = false; // Reset the button state
		lastChangeTime4 = xTaskGetTickCount();

		if (tl->east == RED && tl->west == RED) {
			tl->pedestrianEW = GREEN;
			updateLEDs(tl);

			// R1.4 The pedestrian signal stays green for walkingDelay ms
			vTaskDelay(walkingDelay);

			tl->pedestrianEW = RED;
			updateLEDs(tl);
		} else {
			ledID = 18;
			vTaskResume(ToggleLEDHandle);

			// R1.3
			while ((xTaskGetTickCount() - lastChangeTime4)
					<= pedestrianDelay - orangeDelay) {
				vTaskDelay(1);
			}

			// Change the state of the traffic lights
			tl->east = tl->west = ORANGE;
			// Update the LEDs
			updateLEDs(tl);

			vTaskDelay(orangeDelay);

			vTaskSuspend(ToggleLEDHandle);

			tl->pedestrianEW = GREEN;
			tl->east = tl->west = RED;
			// Update the LEDs
			updateLEDs(tl);

			// R1.4 The pedestrian signal stays green for walkingDelay ms
			vTaskDelay(walkingDelay - orangeDelay);

			// Change the state of the pedestrian light to RED
			tl->east = tl->west = ORANGE;
			updateLEDs(tl);

			vTaskDelay(orangeDelay);

			tl->east = tl->west = GREEN;
			tl->pedestrianEW = RED;
			updateLEDs(tl);
		}
	} else {
		// No pedestrian

		// R2.7
		if (tl->north == RED && tl->south == RED && tl->pedestrianNS == RED
				&& tl->pedestrianEW == RED) {
			if (isSwitchActive(SW2_GPIO_Port, SW2_Pin)
					|| isSwitchActive(SW4_GPIO_Port, SW4_Pin)) {
				if (isSwitchDeactive(SW1_GPIO_Port, SW1_Pin)
						&& isSwitchDeactive(SW3_GPIO_Port, SW3_Pin)) {
					tl->east = RED;
					tl->west = RED;
					tl->north = GREEN;
					tl->south = GREEN;
					updateLEDs(tl);
				} else if ((xTaskGetTickCount() - lastChangeTime2)
						>= redDelayMax) {
					tl->east = ORANGE;
					tl->west = ORANGE;
					tl->north = ORANGE;
					tl->south = ORANGE;
					updateLEDs(tl);

					vTaskDelay(orangeDelay);

					tl->east = RED;
					tl->west = RED;
					tl->north = GREEN;
					tl->south = GREEN;
					updateLEDs(tl);

					lastChangeTime2 = xTaskGetTickCount();
				}
			}
		}

		// R2.7
		if (tl->east == RED && tl->west == RED && tl->pedestrianNS == RED
				&& tl->pedestrianEW == RED) {
			if (isSwitchActive(SW1_GPIO_Port, SW1_Pin)
					|| isSwitchActive(SW3_GPIO_Port, SW3_Pin)) {
				if (isSwitchDeactive(SW2_GPIO_Port, SW2_Pin)
						&& isSwitchDeactive(SW4_GPIO_Port, SW4_Pin)) {
					tl->north = RED;
					tl->south = RED;
					tl->east = GREEN;
					tl->west = GREEN;
					updateLEDs(tl);
				} else if ((xTaskGetTickCount() - lastChangeTime2)
						>= redDelayMax) {
					tl->north = ORANGE;
					tl->south = ORANGE;
					tl->east = ORANGE;
					tl->west = ORANGE;
					updateLEDs(tl);

					vTaskDelay(orangeDelay);

					tl->north = RED;
					tl->south = RED;
					tl->east = GREEN;
					tl->west = GREEN;
					updateLEDs(tl);

					lastChangeTime2 = xTaskGetTickCount();
				}
			}
		}
	}
}

/**
 @brief This function updates the state of the traffic light system based on the inputs from the pedestrian buttons and car sensors.
 @param tl A pointer to a TrafficLight structure. This structure should contain the current state of the traffic light system.
 @return void
 */
void updateInputs(TrafficLight *tl) {
	// Check if the North-South pedestrian button has been pressed
	if (SW78_hit()) {
		tl->button_pressed_NS = true;
	}
	// Check if the East-West pedestrian button has been pressed
	if (SW56_hit()) {
		tl->button_pressed_EW = true;
	}
	// Check the state of the car sensor in lane 0
	if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_SET) {
		tl->car_present[0] = true;
	} else {
		tl->car_present[0] = false;
	}
	// Check the state of the car sensor in lane 1
	if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_SET) {
		tl->car_present[1] = true;
	} else {
		tl->car_present[1] = false;
	}
	// Check the state of the car sensor in lane 2
	if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_SET) {
		tl->car_present[2] = true;
	} else {
		tl->car_present[2] = false;
	}
	// Check the state of the car sensor in lane 3
	if (HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == GPIO_PIN_SET) {
		tl->car_present[3] = true;
	} else {
		tl->car_present[3] = false;
	}
}

