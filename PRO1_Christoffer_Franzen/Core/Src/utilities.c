/**
 ******************************************************************************
 @brief Utilities, program for controlling lights and more
 @file utilities.c
 @author Christoffer Franzén
 @version 1.0
 @date 18-November-2023
 @brief Contain the functions and definitions related to the lights and more
 ******************************************************************************
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "utilities.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "traffic_light.h"

uint8_t data[3] = { 0x00, 0x00, 0x00 };
uint8_t lastData[3] = { 0x00, 0x00, 0x00 };
uint8_t LED_arr[24][3] = { { LED1 }, { LED2 }, { LED3 }, { LED4 }, { LED5 }, {
		LED6 }, { LED7 }, { LED8 }, { LED9 }, { LED10 }, { LED11 }, { LED12 }, {
		LED13 }, { LED14 }, { LED15 }, { LED16 }, { LED17 }, { LED18 },
		{ LED19 }, { LED20 }, { LED21 }, { LED22 }, { LED23 }, { LED24 } };

/**
 @brief sendSPIdata, uses the SPI3 bus to send data to a 74HC595D shift register, which is used to control traffic lights.
 @param data[] An array of bytes to be sent via SPI.
 @param byteSize The number of bytes in the data array to be sent.
 @return void
 */
void sendSPIdata(uint8_t data[], uint16_t byteSize) {
	// Clear the buffer
	HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_SET);

	// Disable the output
	HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_SET);

	// Transmit data
	HAL_SPI_Transmit(&hspi3, &data[0], byteSize, 10);

	// Trigger registers
	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_RESET);

	// Enable output
	HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_RESET);
}

/**
 @brief lightUpLEDs, sends data to a series of LEDs connected to a 74HC595D shift register.
 @param leds[][3] A two-dimensional array where each row corresponds to an LED and each column corresponds to a byte of the LED's data.
 @param num_leds The number of LEDs in the leds array.
 @return void
 */
void lightUpLEDs(uint8_t leds[][3], size_t num_leds) {
	for (size_t i = 0; i < num_leds; i++) {
		data[0] |= leds[i][0];
		data[1] |= leds[i][1];
		data[2] |= leds[i][2];
	}
}

/**
 @brief ledOn, turns on a specific LED connected to a 74HC595D shift register.
 @param Lednr The number of the LED to be turned on.
 @return void
 */
void ledOn(uint8_t Lednr) {
	if (Lednr >= 1 && Lednr <= 24) {
		data[0] |= LED_arr[Lednr - 1][0];
		data[1] |= LED_arr[Lednr - 1][1];
		data[2] |= LED_arr[Lednr - 1][2];
	}
}

/**
 @brief ledOff, turns off a specific LED connected to a 74HC595D shift register.
 @param Lednr The number of the LED to be turned off.
 @return void
 */
void ledOff(uint8_t Lednr) {
	if (Lednr >= 1 && Lednr <= 24) {
		data[0] &= ~LED_arr[Lednr - 1][0];
		data[1] &= ~LED_arr[Lednr - 1][1];
		data[2] &= ~LED_arr[Lednr - 1][2];
	}
}

/**
 @brief turnOffAllLEDs, turns off all LEDs connected to a 74HC595D shift register.
 @return void
 */
void turnOffAllLEDs() {
	for (uint8_t i = 1; i <= 24; i++) {
		ledOff(i);
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
void toggleLED2(uint8_t Lednr) {
	static uint8_t ledState = 0; // LED state: 0 means off, 1 means on

	// Toggle the LED state
	ledState = !ledState;

	// Set the LED to the new state
	if (ledState) {
		// Turn on the LED
		ledOn(Lednr);
	} else {
		// Turn off the LED
		ledOff(Lednr);
	}
}
