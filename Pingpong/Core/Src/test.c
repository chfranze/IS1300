/**
******************************************************************************
@brief test, header for clock-program
@file trst.c
@author Christoffer Franzén
@version 1.0
@date 07-November-2019
@brief Functions and structures for program test'
******************************************************************************
*/

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "Test.h"
#include "gpio.h"
#include "pingpong.h"
#include "pingpong_functions.h"

void Test_program(void)
{
	Test_Led();
	Test_Show_Points();
	Test_buttons();
	Test_Point_Handler();
}

/**
@brief Test_Led, test led
@param void
@return void
*/
void Test_Led(void)
{
	int8_t Lednr;
	/* Loop checking that all leds can be turned on*/
	for (Lednr=1; Lednr<= 8; Lednr++) {
		Led_on(Lednr);
		HAL_Delay(500);
	}
	Led_on(9); // Turn off led 8
	HAL_Delay(1000); // 1000 ms
	return;
}

/**
@brief Test_SHor_Points
@param void
@return void
*/
void Test_Show_Points(void){
	Show_points(1, 1);
	HAL_Delay(1000);
	Show_points(2, 3);
	HAL_Delay(1000);
}

/**
@brief Test_buttons
@param void
@return void
*/
void Test_buttons(void) {
	int8_t j;
	/* Checking buttons */

	j = 4;
	Led_on(j); // Light on

	while (j < 9 && j > 0) {
		if (L_hit() == true) // Wait for left button hit
				{
			j++; // next led to the right
			Led_on(j); // Light on
			HAL_Delay(100); // 100 ms
			while (L_hit() == true)
				; // Wait for button release
			HAL_Delay(100); // 100 ms
		}
		if (R_hit() == true) // Wait for right button hit
				{
			j--; // next led to the left
			Led_on(j); // Light on
			HAL_Delay(100); // 100 ms
			while (R_hit() == true)
				; // Wait for button release
			HAL_Delay(100); // 100 ms
			if (j < 1)
				j = 0; // Start again from left
		}
	}
}

/**
@brief Test_Point_Handler
@param void
@return void
*/
void Test_Point_Handler(void){
	uint32_t Speed; // Ball speed
	uint8_t L_points;
	uint8_t R_points;
	Speed = 500000; // Number of loops
	R_points = 0;
	L_points = 0;
	bool L_Serve = true;
	bool R_Serve = true;

	Point_Handler(&L_points, &R_points, &Speed, &L_Serve, &R_Serve);
	HAL_Delay(1000); // 100 ms

	R_points = 4;
	L_points = 1;

	Point_Handler(&L_points, &R_points, &Speed, &L_Serve, &R_Serve);
	HAL_Delay(1000); // 100 ms

	R_points = 1;
	L_points = 4;

	Point_Handler(&L_points, &R_points, &Speed, &L_Serve, &R_Serve);
	HAL_Delay(1000); // 100 ms
}
