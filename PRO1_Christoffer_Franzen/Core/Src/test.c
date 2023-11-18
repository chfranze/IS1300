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

#include "usart.h"
#include "gpio.h"
#include "test.h"

/**
@brief Test_program, main method for the test code
@param void
@return void
*/
void Test_program(void)
{
  //Test_Led();
}

/**
@brief Test_Led, function to test leds and buttons
@param void
@return void
*/
void Test_Lights(void){

}

/**
@brief Test_Led, function to test leds and buttons
@param void
@return void
*/
void Test_Led(void)
{
	while (1)
	  {
		if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0){
		  HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET); // Led 26
		}else{
		  HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		}

		if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 0){
		  HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET); // Led 26
		}else{
		  HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
		}

		if(HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == 0){
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET); // Led 25
		}else{
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
		}

		if(HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == 0){
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET); // Led 25
		}else{
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
		}

		if(HAL_GPIO_ReadPin(SW56_GPIO_Port, SW56_Pin) == 0){
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET); // Led 25
		}else{
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
		}

		if(HAL_GPIO_ReadPin(SW78_GPIO_Port, SW78_Pin) == 0){
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET); // Led 25
		}else{
		  HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
		}
	  }
}
