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

uint8_t toggleFreq = 5;
uint8_t pedestrianDelay = 100;
uint8_t walkingDelay = 100;
uint8_t orangeDelay = 50;
//uint8_t greenDelay = 5000;
//uint8_t redDelayMax = 100;
//const TickType_t toggleFreq = pdMS_TO_TICKS(500);
//const TickType_t pedestrianDelay = pdMS_TO_TICKS(5000);
//const TickType_t walkingDelay = pdMS_TO_TICKS(10000);
//const TickType_t orangeDelay = pdMS_TO_TICKS(2500);
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
 @return void
 */
void TrafficLightStateMachine(TrafficLight *tl) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // Delay for 1 second
    static TickType_t lastChangeTime = 0;
    static TickType_t lastChangeTime2 = 0;

	if ((xTaskGetTickCount() - lastChangeTime) >= greenDelay) {
		bool all_false = true;
		for (int i = 0; i < 4; i++) {
		    if (tl->car_present[i]) {
		        all_false = false;
		        break;
		    }
		}

		if (all_false){
			if(tl->north == RED && tl->south == RED &&  tl->pedestrianNS == RED &&  tl->pedestrianEW == RED){
				tl->east = RED;
				tl->west = RED;
				tl->north = GREEN;
				tl->south = GREEN;
	            updateLEDs(tl);
			}
			else if(tl->east == RED && tl->west == RED &&  tl->pedestrianNS == RED &&  tl->pedestrianEW == RED){
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

        // if tl->north = tl->south is RED
        if(tl->north == RED && tl->south == RED)
        {
            tl->pedestrianNS = GREEN;
            updateLEDs(tl);

            xLastWakeTime = xTaskGetTickCount();
            for (int i = 0; i < walkingDelay; i++) {
                vTaskDelayUntil(&xLastWakeTime, xFrequency);
            }

            tl->pedestrianNS = RED;
            updateLEDs(tl);
        }
        else
        {
            // Change the state of the traffic lights
            tl->north = tl->south = ORANGE;
            // Update the LEDs
            updateLEDs(tl);

            xLastWakeTime = xTaskGetTickCount();
            for (int i = 0; i < orangeDelay; i++) {
                vTaskDelayUntil(&xLastWakeTime, xFrequency);
                toggleLED(i, 23); // Toggle LED 23
            }

            tl->pedestrianNS = GREEN;
            tl->north = tl->south = RED;
            // Update the LEDs
            updateLEDs(tl);

            xLastWakeTime = xTaskGetTickCount();
            for (int i = 0; i < walkingDelay; i++) {
                vTaskDelayUntil(&xLastWakeTime, xFrequency);
            }

            // Change the state of the pedestrian light to RED
            tl->north = tl->south = GREEN;
            tl->pedestrianNS = RED;
        }
    } else if (tl->button_pressed_EW) {
        // Pedestrian button for East-West has been pressed
        tl->button_pressed_EW = false; // Reset the button state

        if(tl->east == RED && tl->west == RED)
        {
            tl->pedestrianEW = GREEN;
            updateLEDs(tl);

            xLastWakeTime = xTaskGetTickCount();
            for (int i = 0; i < walkingDelay; i++) {
                vTaskDelayUntil(&xLastWakeTime, xFrequency);
            }

            tl->pedestrianEW = RED;
            updateLEDs(tl);
        }
        else
        {
            // Change the state of the traffic lights
            tl->east = tl->west = ORANGE;
            // Update the LEDs
            updateLEDs(tl);

            xLastWakeTime = xTaskGetTickCount();
            for (int i = 0; i < orangeDelay; i++) {
                vTaskDelayUntil(&xLastWakeTime, xFrequency);
                toggleLED(i, 18); // Toggle LED 23
            }

            tl->pedestrianEW = GREEN;
            tl->east = tl->west = RED;
            // Update the LEDs
            updateLEDs(tl);

            xLastWakeTime = xTaskGetTickCount();
            for (int i = 0; i < walkingDelay; i++) {
                vTaskDelayUntil(&xLastWakeTime, xFrequency);
            }

            // Change the state of the pedestrian light to RED
            tl->east = tl->west = GREEN;
            tl->pedestrianEW = RED;
        }
    }
    else
    {

    	// No pedestrian

    	// R2.7
    	if(tl->north == RED && tl->south == RED &&  tl->pedestrianNS == RED &&  tl->pedestrianEW == RED){
			if (isSwitchActive(SW2_GPIO_Port, SW2_Pin) || isSwitchActive(SW4_GPIO_Port, SW4_Pin)) {
				if(isSwitchDeactive(SW1_GPIO_Port, SW1_Pin) && isSwitchDeactive(SW3_GPIO_Port, SW3_Pin)){
					tl->east = RED;
					tl->west = RED;
					tl->north = GREEN;
					tl->south = GREEN;
		            updateLEDs(tl);
				}
				else if((xTaskGetTickCount() - lastChangeTime2) >= redDelayMax){
					tl->east = ORANGE;
					tl->west = ORANGE;
		            updateLEDs(tl);

					xLastWakeTime = xTaskGetTickCount(); //
					for (int i = 0; i < orangeDelay; i++) {	// Cross walk shall be red pedestrianDelay after the pedestrian pushed the button (R1.3)
						vTaskDelayUntil(&xLastWakeTime, xFrequency);
					}

					tl->east = RED;
					tl->west = RED;
					tl->north = GREEN;
					tl->south = GREEN;
		            updateLEDs(tl);

		            lastChangeTime2 = xTaskGetTickCount();
					//xLastWakeTime = xTaskGetTickCount(); //
					//for (int i = 0; i < redDelayMax; i++) {	// Cross walk shall be red pedestrianDelay after the pedestrian pushed the button (R1.3)
					//	vTaskDelayUntil(&xLastWakeTime, xFrequency);
					//}
				}
			}
    	}

    	// R2.7
    	if(tl->east == RED && tl->west == RED &&  tl->pedestrianNS == RED &&  tl->pedestrianEW == RED){
			if (isSwitchActive(SW1_GPIO_Port, SW1_Pin) || isSwitchActive(SW3_GPIO_Port, SW3_Pin)) {
				if(isSwitchDeactive(SW2_GPIO_Port, SW2_Pin) && isSwitchDeactive(SW4_GPIO_Port, SW4_Pin)){
					tl->north = RED;
					tl->south = RED;
					tl->east = GREEN;
					tl->west = GREEN;
		            updateLEDs(tl);
				}else if((xTaskGetTickCount() - lastChangeTime2) >= redDelayMax){
					tl->north = ORANGE;
					tl->south = ORANGE;
		            updateLEDs(tl);

					xLastWakeTime = xTaskGetTickCount(); //
					for (int i = 0; i < orangeDelay; i++) {	// Cross walk shall be red pedestrianDelay after the pedestrian pushed the button (R1.3)
						vTaskDelayUntil(&xLastWakeTime, xFrequency);
					}

					tl->north = RED;
					tl->south = RED;
					tl->east = GREEN;
					tl->west = GREEN;
		            updateLEDs(tl);

		            lastChangeTime2 = xTaskGetTickCount();
					//xLastWakeTime = xTaskGetTickCount(); //
					//for (int i = 0; i < redDelayMax; i++) {	// Cross walk shall be red pedestrianDelay after the pedestrian pushed the button (R1.3)
					//	vTaskDelayUntil(&xLastWakeTime, xFrequency);
					//}
				}
			}
    	}
    }
}

/**
 @brief
 @param
 @return void
 */
void updateLEDs(TrafficLight *tl) {
    // Turn off all LEDs
    turnOffAllLEDs();

    // Update the car LEDs
    switch (tl->north) {
        case RED:
            ledOn(10);
            break;
        case GREEN:
            ledOn(12);
            break;
        case ORANGE:
            ledOn(11);
            break;
        default:
            break;
    }

    switch (tl->south) {
        case RED:
            ledOn(4);
            break;
        case GREEN:
            ledOn(6);
            break;
        case ORANGE:
            ledOn(5);
            break;
        default:
            break;
    }

    switch (tl->east) {
        case RED:
            ledOn(1);
            break;
        case GREEN:
            ledOn(3);
            break;
        case ORANGE:
            ledOn(2);
            break;
        default:
            break;
    }

    switch (tl->west) {
        case RED:
            ledOn(7);
            break;
        case GREEN:
            ledOn(9);
            break;
        case ORANGE:
            ledOn(8);
            break;
        default:
            break;
    }

    // Update the pedestrian LEDs
    switch (tl->pedestrianNS) {
        case RED:
            ledOn(20);
            ledOn(19);
            break;
        case GREEN:
            ledOn(22);
            ledOn(21);
            break;
        default:
            break;
    }

    switch (tl->pedestrianEW) {
        case RED:
            ledOn(14);
            ledOn(13);
            break;
        case GREEN:
            ledOn(16);
            ledOn(15);
            break;
        default:
            break;
    }
}

/**
 @brief
 @param
 @return void
 */
void updateInputs(TrafficLight *tl){
	if(SW78_hit()){
		tl->button_pressed_NS = true;
	}
	if(SW56_hit()){
		tl->button_pressed_EW = true;
	}
	if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_SET){
		tl->car_present[0] = true;
	}else{
		tl->car_present[0] = false;
	}
	if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_SET){
		tl->car_present[1] = true;
	}else{
		tl->car_present[1] = false;
	}
	if(HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_SET){
		tl->car_present[2] = true;
	}else{
		tl->car_present[2] = false;
	}
	if(HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == GPIO_PIN_SET){
		tl->car_present[3] = true;
	}else{
		tl->car_present[3] = false;
	}
}






