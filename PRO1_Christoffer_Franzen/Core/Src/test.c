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

/**
@brief Test_program, main method for the test code
@param void
@return void
*/
void Test_program(void)
{
  //Test_Led();
  Test_TraficLights();
}

/**
@brief Test_TraficLights, function to test traffic lights
@param void
@return void
*/
void Test_TraficLights(void)
{
    uint32_t SPIData = 0x000001;
    uint32_t shiftData;

    for (int i = 0; i <= 24; i++)
    {
        shiftData = SPIData << i;
        sendSPIdata((uint8_t*)&shiftData, sizeof(shiftData));
        HAL_Delay(200);

        if(i == 24)
        {
            for(int j = 15; j >= 0; j--)
            {
                shiftData = SPIData << j;
                sendSPIdata((uint8_t*)&shiftData, sizeof(shiftData));
                HAL_Delay(200);
            }
        }
    }

    uint8_t data1[3] = {0xff, 0xff, 0xff};
    uint8_t data2[3] = {0x00, 0x00, 0x00};

    for (int i = 0; i < 3; i++)
    {
        sendSPIdata(&data1[0], 3);
        HAL_Delay(500);
        sendSPIdata(&data2[0], 3);
        HAL_Delay(500);
    }
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
