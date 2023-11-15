/**
******************************************************************************
@brief Pingpong, functions for Pingpong-program
@file pingpong_functions.c
@author Christoffer Franzén
@version 1.0
@date 07-November-2023
@brief Functions and structures for program Pingpong
******************************************************************************
*/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "Test.h"
#include "gpio.h"
#include "pingpong_functions.h"

/**
@brief Led_on, turn one of the pingpong leds on

@param uint8_t Lednr , number to the Led that is turned on
		Lednr can be 1-8, all other values turns all leds off

@return void
*/
void Led_on(uint8_t Lednr)
{
	uint8_t i;

	for (i=1; i<= 8; i++)
	{
		switch(i){
		case 1 : // Led 1
			if (Lednr==i) HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			break;
		case 2 : // Led 2
			if (Lednr==i) HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			break;
		case 3 : // Led 3
			if (Lednr==i) HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			break;
		case 4 : // Led 4
			if (Lednr==i) HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			break;
		case 5 : // Led 5
			if (Lednr==i) HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);
			break;
		case 6 : // Led 6
			if (Lednr==i) HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_RESET);
			break;
		case 7 : // Led 7
			if (Lednr==i) HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_RESET);
			break;
		case 8 : // Led 8
			if (Lednr==i) HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_RESET);
			break;
		default :
			;
		} // end switch
	} // end for-loop
	return;
} // end function Led_on

/**
@brief Show_led, used to set led number on
@param1 uint8_t Lednr led number
@return void, no return value
*/
void Set_led(uint8_t Lednr)
{
	uint8_t i;

	for (i=1; i<= 8; i++)
	{
		switch(i){
		case 1 : // Led 1
			if (Lednr==i) HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			break;
		case 2 : // Led 2
			if (Lednr==i) HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			break;
		case 3 : // Led 3
			if (Lednr==i) HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			break;
		case 4 : // Led 4
			if (Lednr==i) HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
			break;
		case 5 : // Led 5
			if (Lednr==i) HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
			break;
		case 6 : // Led 6
			if (Lednr==i) HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_SET);
			break;
		case 7 : // Led 7
			if (Lednr==i) HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_SET);
			break;
		case 8 : // Led 8
			if (Lednr==i) HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_SET);
			break;
		default :
			;
		} // end switch
	} // end for-loop
	return;
} // end function Led_on

/**
@brief Show_points, shows points after missed ball
@param1 uint8_t L_points points for left player
@param2 uint8_t R_points points for right player
@return void, no return value
*/
void Show_points(uint8_t L_points, uint8_t R_points)
{
	Led_on(0);

	for (uint8_t i = 1; i <= L_points; i++) {
		Set_led(i);
	}
	for (uint8_t i = 8; i >= 9-R_points; i--) {
		Set_led(i);
	}

	HAL_Delay(500);
	Led_on(0);

	return;
}

/**
@brief L_hit, check if L button is pressed
@param void
@return bool, true if L button pushed, false otherwise
*/
bool L_hit(void)
{ if (HAL_GPIO_ReadPin(L_button_GPIO_Port, L_button_Pin) == 0) return true;
	else return false;
}

/**
@brief R_hit, check if R button is pressed
@param void
@return bool, true if R button pushed, false otherwise
*/
bool R_hit(void)
{ if (HAL_GPIO_ReadPin(R_button_GPIO_Port, R_button_Pin) == 0) return true;
	else return false;
}

/**
@brief Point_Handler, used to handel points
@param1 uint8_t *L_points points for left player
@param2 uint8_t *R_points points for right player
@param3 uint32_t *Speed Speed for bal
@param4 bool *L_Serve
@param5 bool *R_Serve
@return void, no return value
*/
void Point_Handler(uint8_t *L_Points, uint8_t *R_Points, uint32_t *Speed, bool *L_Serve, bool *R_Serve)
{
	if (*L_Points >= 4 || *R_Points >= 4) {
		for (uint8_t j = 0; j < 5; j++) {
			Show_points(*L_Points, *R_Points);
			HAL_Delay(500);
		}
		*R_Points = 0;
		*L_Points = 0;
		*Speed = 500000;
		*L_Serve = true;
		*R_Serve = true;
	} else {
		Show_points(*L_Points, *R_Points);
		*Speed *= 0.8;
	}
}
