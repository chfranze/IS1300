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
  //Test_lights();
  //Test_Lights2();

    uint8_t SPIData[] = {0x0F, 0x0F, 0x0F};  // 3 bytes of only 1s
    uint8_t SPIData2[] = {0x00, 0x00, 0x00};  // 3 bytes of only 1s


    while(1)
    {
        sendSPIdata(&SPIData[0], 3);
        HAL_Delay(100);
        sendSPIdata(&SPIData2[0], 3);
        HAL_Delay(100);
    }
}

/**
@brief sendSPIdata, uses the SPI buss to control trafic lights
@param void
@return void
*/
void sendSPIdata(uint8_t data[], uint16_t byteSize)
{
    // Clear the buffer
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);  // Wait for 1ms
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_SET);

    // Disable the output
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_SET);

    // Transmit data
    HAL_SPI_Transmit(&hspi3, data, byteSize, 10);

    // Trigger registers
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_SET);
    HAL_Delay(1);  // Wait for 1ms
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_RESET);

    // Enable output
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_RESET);
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
