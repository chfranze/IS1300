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
//uint8_t greenDelay = 50;
//uint8_t redDelayMax = 100;

// State enumeration
typedef enum {
	INITIALIZATION,
	BUTTON_PRESSED,
	CAR_SIGNAL_TRANSITION,
	PEDESTRIAN_CROSSING,
} State;

// State enumeration
typedef enum {
    RED,
    ORANGE,
    GREEN
} TrafficLightState;

typedef struct {
    TrafficLightState car;
    TrafficLightState pedestrian;
    bool button_pressed;
} TrafficLight;

void initialize(TrafficLight *tl) {
    tl->car = GREEN;
    tl->pedestrian = RED;
    tl->button_pressed = false;
}


void update(TrafficLight *tl) {
    if (tl->button_pressed) {
        // Handle pedestrian button press
        tl->button_pressed = false;
        tl->car = ORANGE;
        // Wait for orange_delay ms
        tl->car = RED;
        // Wait for pedestrian_delay ms
        tl->pedestrian = GREEN;
        // Wait for walking_delay ms
        tl->pedestrian = RED;
    }

    // Handle car traffic light
    switch (tl->car) {
        case RED:
            // If no cars are waiting, transition to green
            tl->car = GREEN;
            break;
        case ORANGE:
            // Transition to red or green based on previous state
            break;
        case GREEN:
            // If cars are waiting, transition to orange
            tl->car = ORANGE;
            break;
    }
}


// State enumeration
typedef enum {
	Vertical_Green_Horizontal_Red,
	Transition_To_Horizontal_Green,
	Horizontal_Green_Vertical_Red,
	Transition_To_Vertical_Green,
} State2;

TimerHandle_t xTimer = NULL;


void TrafficLightStateMachine(void) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(100); // Delay for 1 second
	State state = Vertical_Green_Horizontal_Red;
	TickType_t greenDelay = pdMS_TO_TICKS(5000); // Set your greenDelay here
	TickType_t redDelayMax = pdMS_TO_TICKS(5000); // Set your redDelayMax here
	TickType_t lastChangeTime = xTaskGetTickCount();
	while (true) {

		if ((xTaskGetTickCount() - lastChangeTime) >= greenDelay &&
					HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET &&
					HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET &&
					HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET &&
					HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == GPIO_PIN_RESET) {
					turnOffAllLEDs();
					state = (state == Vertical_Green_Horizontal_Red) ? Horizontal_Green_Vertical_Red : Vertical_Green_Horizontal_Red;
					lastChangeTime = xTaskGetTickCount();
				}
		else if ((state == Vertical_Green_Horizontal_Red && (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_SET || HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == GPIO_PIN_SET)) ||
				 (state == Horizontal_Green_Vertical_Red && (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_SET || HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_SET))) {
			vTaskDelay(redDelayMax);
		}

		switch (state) {
		case Vertical_Green_Horizontal_Red:
			ledOn(3);
			ledOn(9);
			ledOn(10);
			ledOn(4);


			if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_SET || HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == GPIO_PIN_SET) {
				if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET){
					turnOffAllLEDs();
					state = Horizontal_Green_Vertical_Red;
				}else{
					turnOffAllLEDs();
					state = Transition_To_Horizontal_Green;
				}
			}

			// Make so If there is no active car in any direction the allowed (i.e. green) direction changes every greenDelayms.
            //if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET && count >= (int)greenDelay / xFrequency) {
            //    turnOffAllLEDs();
            //    state = Horizontal_Green_Vertical_Red;
            //    count = 0; // Reset the count
            //}

			break;
		case Transition_To_Horizontal_Green:
			ledOn(2);
			ledOn(8);
			ledOn(10);
			ledOn(4);

			xLastWakeTime = xTaskGetTickCount(); //
			for (int i = 0; i < orangeDelay; i++) {	// Cross walk shall be red pedestrianDelay after the pedestrian pushed the button (R1.3)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
			}

			turnOffAllLEDs();

			state = Horizontal_Green_Vertical_Red;
			break;
		case Horizontal_Green_Vertical_Red:
			ledOn(6);
			ledOn(12);
			ledOn(1);
			ledOn(7);

			if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_SET || HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_SET) {
				if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == GPIO_PIN_RESET){
					turnOffAllLEDs();
					state = Vertical_Green_Horizontal_Red;
				}else{
					turnOffAllLEDs();
					state = Transition_To_Vertical_Green;
				}
			}

			break;
		case Transition_To_Vertical_Green:
			ledOn(5);
			ledOn(11);
			ledOn(1);
			ledOn(7);

			xLastWakeTime = xTaskGetTickCount();
			for (int i = 0; i < orangeDelay; i++) { // The pedestrian signal stays green for walkingDelay (R1.4)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
			}

			turnOffAllLEDs();

			state = Vertical_Green_Horizontal_Red;
			break;
		}
	}
}










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

/**
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

*/

/**
 @brief
 @param
 @param
 @return void
 */
void TrafficLightInitialization2(void) {
	int InitializeLEDs[] = { 3, 9, 13, 14 };

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

/**
void TrafficLightStateMachine2(void) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(100); // Delay for 1 second
	State state = INITIALIZATION;
	while (true) {
		switch (state) {
		case INITIALIZATION:
			TrafficLightInitialization2(); // Set initial state (R1.1)

			ledOn(3);
			ledOn(9);
			ledOn(13);
			ledOn(14);

			if (SW56_hit()) {
				state = BUTTON_PRESSED;
			}
			break;
		case BUTTON_PRESSED:
			xLastWakeTime = xTaskGetTickCount(); //
			for (int i = 0; i < pedestrianDelay; i++) {	// Cross walk shall be red pedestrianDelay after the pedestrian pushed the button (R1.3)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
				toggleLED(i, 18); // Toggle LED 18 (R1.2)
			}

			ledOff(3);
			ledOff(9);

			state = CAR_SIGNAL_TRANSITION;
			break;
		case CAR_SIGNAL_TRANSITION:
			ledOn(2);
			ledOn(8);

			xLastWakeTime = xTaskGetTickCount();
			for (int i = 0; i < orangeDelay; i++) {	// A car signal transitions from red to orange to green, or from green to orange to red, remaining orange for orangeDelay (R1.6)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
				toggleLED(i, 18); // Toggle LED 18 (R1.2)
			}

			ledOff(2);
			ledOff(8);

			state = PEDESTRIAN_CROSSING;
			break;
		case PEDESTRIAN_CROSSING:
			ledOff(13);
			ledOff(14);

			ledOn(7);
			ledOn(1);
			ledOn(16);  // The pedestrian signal must be red when the car signal of each active lane that
			ledOn(15); // crosses the cross walk is either green or orange, and green otherwise (R1.5)

			xLastWakeTime = xTaskGetTickCount();
			for (int i = 0; i < walkingDelay; i++) { // The pedestrian signal stays green for walkingDelay (R1.4)
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
			}

			ledOff(16);
			ledOff(15);
			ledOff(7);
			ledOff(1);

			state = INITIALIZATION;
			break;
		}
	}
}
*/
