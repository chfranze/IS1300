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
  //Test_TraficLights();
  //lightUpLED1();
  //lightUpLED();
	Test_lightUpLEDs();

  //uint8_t leds_to_light[5][3] = { {LED1}, {LED6}, {LED9}, {LED13}, {LED22} };
  //lightUpLED(leds_to_light, 5);
}

void lightUpLED(void) {
    uint8_t data[3] = {0x20, 0x00, 0x00};
    uint16_t byteSize = sizeof(data);
    sendSPIdata(data, byteSize);
}

void Test_lightUpLEDs(void)
{
	uint8_t leds_to_light[5][3] = { {LED1}, {LED6}, {LED9}, {LED13}, {LED22} };
	uint8_t no_leds_to_light[1][3] = { {NO_LEDs} };

    for (int i = 0; i < 3; i++)
    {
    	lightUpLEDs(leds_to_light, 5);
        HAL_Delay(500);
        sendSPIdata(no_leds_to_light, 3);
        HAL_Delay(500);
    }
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

    uint8_t all_leds_to_light[1][3] = { {ALL_LEDs} };
    uint8_t no_leds_to_light[1][3] = { {NO_LEDs} };

    for (int i = 0; i < 3; i++)
    {
        sendSPIdata(all_leds_to_light, 3);
        HAL_Delay(500);
        sendSPIdata(no_leds_to_light, 3);
        HAL_Delay(500);
    }
}

void Test_GPIOLeds(void)
{
    for (int i = 0; i < 3; i++)
    {
    	HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_SET);
        HAL_Delay(500);
        HAL_GPIO_WritePin(LED26_GPIO_Port, LED26_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED25_GPIO_Port, LED25_Pin, GPIO_PIN_RESET);
        HAL_Delay(500);
    }
}

void Test_GPIOButtons(void)
{

}

void Test_LedOn(void)
{

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
