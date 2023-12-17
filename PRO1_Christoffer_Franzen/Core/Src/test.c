/**
 ******************************************************************************
 @brief Test, test program for project
 @file test.c
 @author Christoffer Franzén
 @version 1.0
 @date 17-November-2023
 @brief Test functions for program
 ******************************************************************************
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "usart.h"
#include "gpio.h"
#include "test.h"
#include "utilities.h"
#include "spi.h"
#include "switch_handlers.h"
#include "traffic_light.h"

/**
 @brief Test_program, main method for the test code
 @param void
 @return void
 */
void test_program(void) {
	test_lightUpLEDs();
	test_traficLights();
	test_GPIOLeds();
	test_GPIOInputs();
	test_ledOn();
}

/**
 @brief Test_lightUpLEDs, function to test lighting up LEDs
 @param void
 @return void
 */
void test_lightUpLEDs(void) {
	uint8_t leds_to_light[5][3] = { { LED1 }, { LED6 }, { LED9 }, { LED13 }, {
			LED22 } };
	uint8_t no_leds_to_light[1][3] = { { NO_LEDs } };

	for (int i = 0; i < 3; i++) {
		lightUpLEDs(leds_to_light, 5);
		HAL_Delay(500);
		lightUpLEDs(no_leds_to_light, 1);
		HAL_Delay(500);
	}
}

/**
 @brief Test_TraficLights, function to test traffic lights
 @param void
 @return void
 */
void test_traficLights(void) {
	uint32_t SPIData = 0x000001;
	uint32_t shiftData;

	for (int i = 0; i <= 24; i++) {
		shiftData = SPIData << i;
		sendSPIdata((uint8_t*) &shiftData, sizeof(shiftData));
		HAL_Delay(200);

		if (i == 24) {
			for (int j = 15; j >= 0; j--) {
				shiftData = SPIData << j;
				sendSPIdata((uint8_t*) &shiftData, sizeof(shiftData));
				HAL_Delay(200);
			}
		}
	}

	uint8_t all_leds_to_light[1][3] = { { ALL_LEDs } };
	uint8_t no_leds_to_light[1][3] = { { NO_LEDs } };

	for (int i = 0; i < 3; i++) {
		sendSPIdata(all_leds_to_light, 3);
		HAL_Delay(500);
		sendSPIdata(no_leds_to_light, 3);
		HAL_Delay(500);
	}
}

/**
 @brief Test_GPIOLeds, function to test GPIO LEDs
 @param void
 @return void
 */
void test_GPIOLeds(void) {
	for (int i = 0; i < 3; i++) {
		HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
		HAL_Delay(500);
	}
}

/**
 @brief Test_GPIOInputs, function to test GPIO inputs
 @param void
 @return void
 */
void test_GPIOInputs(void) {
	while (1) {
		if (SW1_switch()) {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		}

		if (SW2_switch()) {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		}

		if (SW3_switch()) {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		}

		if (SW4_switch()) {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		}

		if (SW56_hit()) {
			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
		}

		if (SW78_hit()) {
			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
		}

		if (SW56_hit() && SW78_hit()) {
			HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
			break;
		}
	}
}

/**
 @brief Test_LedOn, function to test LED on
 @param void
 @return void
 */
void test_ledOn(void) {
	ledOn(1);
	ledOn(7);
	ledOn(14);
	ledOn(19);
	ledOn(23);

	for (int i = 0; i < 10; i++) {
		ledOn(1);
		HAL_Delay(500);
		ledOff(1);
		HAL_Delay(500);
	}
}

/**
 @brief Test_LedOn, function to test LED on
 @param void
 @return void
 */
void test_trafficLightStateMachine() {
	// TODO: Implement
}

